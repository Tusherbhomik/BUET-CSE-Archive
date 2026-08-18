"""
Shared utilities for Multilingual Image Editing Error Classifier pipeline.
Upload this as a Kaggle utility script and import in each notebook.
"""

import os
import json
import re
import numpy as np
import torch
from PIL import Image
from pathlib import Path

# ── Path Detection ──
IS_KAGGLE = os.path.exists("/kaggle/working")
OUTPUT_DIR = "/kaggle/working" if IS_KAGGLE else "./output"
INPUT_DIR = "/kaggle/input" if IS_KAGGLE else "./output"

# ── Dataset ──
HF_DATASET_NAME = "timbrooks/instructpix2pix-clip-filtered"
SUBSET_SIZE = 5000
RANDOM_SEED = 42

# ── Translation (MarianMT) ──
TRANSLATION_MODELS = {
    "hi": "Helsinki-NLP/opus-mt-en-hi",
    "bn": "Helsinki-NLP/opus-mt-en-mul",
    "ne": "Helsinki-NLP/opus-mt-en-mul",
}
BACK_TRANSLATION_MODELS = {
    "hi": "Helsinki-NLP/opus-mt-hi-en",
    "bn": "Helsinki-NLP/opus-mt-mul-en",
    "ne": "Helsinki-NLP/opus-mt-mul-en",
}
BLEU_THRESHOLD = 0.65

# Language tags for mul models
LANGUAGE_TAGS = {
    "bn": ">>ben<<",
    "ne": ">>npi<<",
}

# ── Error Taxonomy ──
ERROR_TAXONOMY = {
    0: "Wrong Object",
    1: "Missing Object",
    2: "Extra Object",
    3: "Wrong Attribute",
    4: "Spatial Error",
    5: "Style Mismatch",
    6: "Over-editing",
    7: "Under-editing",
    8: "Artifact/Quality",
    9: "Ambiguous Prompt",
    10: "Failed Removal",
}
NUM_ERROR_CLASSES = 11

# ── Annotation ──
ANNOTATION_MODEL = "llava-hf/llava-v1.6-mistral-7b-hf"

# ── Training ──
BLIP2_MODEL = "Salesforce/blip2-opt-2.7b"
XLM_ROBERTA_MODEL = "xlm-roberta-base"
IMAGE_SIZE = 224
BATCH_SIZE = 8
GRAD_ACCUM_STEPS = 8
EPOCHS = 20
EARLY_STOP_PATIENCE = 3
LR_HEAD = 1e-4
LR_ENCODER = 1e-5

# ── Benchmarking ──
BENCHMARK_MODELS = {
    "instructpix2pix": "timbrooks/instruct-pix2pix",
    "magicbrush": "osunlp/InstructPix2Pix-MagicBrush",
    "ultraedit": "BleachNick/UltraEdit",
}
BENCHMARK_LANGUAGES = ["en", "ne", "bn", "hi"]


# ═══════════════════════════════════════════
# Utility Functions
# ═══════════════════════════════════════════

def get_device():
    """Get best available device with warning if no GPU."""
    if torch.cuda.is_available():
        device = torch.device("cuda")
        print(f"Using GPU: {torch.cuda.get_device_name(0)}")
    else:
        device = torch.device("cpu")
        print("WARNING: No GPU detected, using CPU. This will be slow.")
    return device


def ensure_dirs(*dirs):
    """Create directories if they don't exist."""
    for d in dirs:
        os.makedirs(d, exist_ok=True)


def save_image(image, path):
    """Save a PIL image to disk."""
    ensure_dirs(os.path.dirname(path))
    if isinstance(image, Image.Image):
        image.save(path, quality=95)
    else:
        raise ValueError(f"Expected PIL Image, got {type(image)}")


def load_image(path):
    """Load image from disk as RGB PIL Image."""
    return Image.open(path).convert("RGB")


def image_to_tensor(image, size=IMAGE_SIZE):
    """Convert PIL image to normalized tensor."""
    from torchvision import transforms
    transform = transforms.Compose([
        transforms.Resize((size, size)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406],
                             std=[0.229, 0.224, 0.225]),
    ])
    if isinstance(image, str):
        image = load_image(image)
    return transform(image)


def error_indices_to_names(indices):
    """Convert list of error indices to human-readable names."""
    return [ERROR_TAXONOMY[i] for i in indices if i in ERROR_TAXONOMY]


def multi_hot_to_indices(vector):
    """Convert multi-hot vector to list of active indices."""
    if isinstance(vector, torch.Tensor):
        vector = vector.cpu().numpy()
    return list(np.where(np.array(vector) > 0.5)[0])


def parse_error_labels_from_text(text):
    """
    Parse VLM output to extract error labels.
    Tries JSON first, falls back to regex.
    Returns dict with: error_labels (list[int] multi-hot), error_types (list[str]),
                       has_error (bool), confidence (float)
    """
    result = {
        "error_labels": [0] * NUM_ERROR_CLASSES,
        "error_types": [],
        "has_error": False,
        "confidence": 0.0,
    }

    # Try JSON parsing
    try:
        json_match = re.search(r'\{[^{}]*\}', text, re.DOTALL)
        if json_match:
            parsed = json.loads(json_match.group())

            if "error_types" in parsed:
                error_types = parsed["error_types"]
                if isinstance(error_types, list):
                    taxonomy_lower = {v.lower(): k for k, v in ERROR_TAXONOMY.items()}
                    for et in error_types:
                        et_lower = et.strip().lower()
                        if et_lower in taxonomy_lower:
                            idx = taxonomy_lower[et_lower]
                            result["error_labels"][idx] = 1
                            result["error_types"].append(ERROR_TAXONOMY[idx])

            if "has_error" in parsed:
                result["has_error"] = bool(parsed["has_error"])
            if "confidence" in parsed:
                result["confidence"] = float(parsed["confidence"])

            if any(result["error_labels"]):
                result["has_error"] = True
            return result
    except (json.JSONDecodeError, ValueError, KeyError):
        pass

    # Fallback: regex matching against taxonomy names
    text_lower = text.lower()
    taxonomy_lower = {v.lower(): k for k, v in ERROR_TAXONOMY.items()}
    for name_lower, idx in taxonomy_lower.items():
        if name_lower in text_lower:
            result["error_labels"][idx] = 1
            result["error_types"].append(ERROR_TAXONOMY[idx])
            result["has_error"] = True

    if result["has_error"]:
        result["confidence"] = 0.5  # lower confidence for regex fallback

    return result


def compute_multilabel_metrics(y_true, y_pred, y_prob=None):
    """
    Compute comprehensive multi-label classification metrics.
    y_true, y_pred: np arrays of shape (n_samples, n_classes)
    y_prob: optional, predicted probabilities for AUC-ROC
    """
    from sklearn.metrics import (
        f1_score, hamming_loss, accuracy_score, roc_auc_score,
        precision_score, recall_score, classification_report
    )

    metrics = {}
    metrics["macro_f1"] = f1_score(y_true, y_pred, average="macro", zero_division=0)
    metrics["weighted_f1"] = f1_score(y_true, y_pred, average="weighted", zero_division=0)
    metrics["micro_f1"] = f1_score(y_true, y_pred, average="micro", zero_division=0)
    metrics["hamming_loss"] = hamming_loss(y_true, y_pred)
    metrics["exact_match"] = accuracy_score(y_true, y_pred)
    metrics["precision_macro"] = precision_score(y_true, y_pred, average="macro", zero_division=0)
    metrics["recall_macro"] = recall_score(y_true, y_pred, average="macro", zero_division=0)

    # Per-class F1
    per_class_f1 = f1_score(y_true, y_pred, average=None, zero_division=0)
    metrics["per_class_f1"] = {
        ERROR_TAXONOMY.get(i, f"class_{i}"): float(f) for i, f in enumerate(per_class_f1)
    }

    # AUC-ROC if probabilities provided
    if y_prob is not None:
        try:
            metrics["auc_roc_macro"] = roc_auc_score(
                y_true, y_prob, average="macro", multi_class="ovr"
            )
        except ValueError:
            metrics["auc_roc_macro"] = None

    return metrics


def compute_clip_similarity(img1, img2, model, processor):
    """Compute CLIP image-image similarity score."""
    with torch.no_grad():
        inputs1 = processor(images=img1, return_tensors="pt").to(model.device)
        inputs2 = processor(images=img2, return_tensors="pt").to(model.device)
        feat1 = model.get_image_features(**inputs1)
        feat2 = model.get_image_features(**inputs2)
        feat1 = feat1 / feat1.norm(dim=-1, keepdim=True)
        feat2 = feat2 / feat2.norm(dim=-1, keepdim=True)
        similarity = (feat1 * feat2).sum(dim=-1).item()
    return similarity


def compute_dino_score(img1, img2, model, processor):
    """Compute DINO image-image similarity score."""
    with torch.no_grad():
        inputs1 = processor(images=img1, return_tensors="pt").to(model.device)
        inputs2 = processor(images=img2, return_tensors="pt").to(model.device)
        feat1 = model(**inputs1).last_hidden_state[:, 0]
        feat2 = model(**inputs2).last_hidden_state[:, 0]
        feat1 = feat1 / feat1.norm(dim=-1, keepdim=True)
        feat2 = feat2 / feat2.norm(dim=-1, keepdim=True)
        similarity = (feat1 * feat2).sum(dim=-1).item()
    return similarity


def concat_images_side_by_side(img1, img2, target_height=384):
    """Concatenate two images side by side for VLM input."""
    if isinstance(img1, str):
        img1 = load_image(img1)
    if isinstance(img2, str):
        img2 = load_image(img2)

    # Resize to same height
    ratio1 = target_height / img1.height
    ratio2 = target_height / img2.height
    img1 = img1.resize((int(img1.width * ratio1), target_height), Image.LANCZOS)
    img2 = img2.resize((int(img2.width * ratio2), target_height), Image.LANCZOS)

    # Concatenate
    total_width = img1.width + img2.width
    combined = Image.new("RGB", (total_width, target_height))
    combined.paste(img1, (0, 0))
    combined.paste(img2, (img1.width, 0))
    return combined
