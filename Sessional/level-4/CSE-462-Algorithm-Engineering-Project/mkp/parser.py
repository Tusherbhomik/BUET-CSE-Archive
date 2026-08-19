"""
Parsers for MKP benchmark file formats.

OR-Library format (chubeas/, gk/, sac94/):
    Line 1: n  m  seed_or_optimum
    Next tokens: n profits
    Next tokens: m*n weights  (row i = constraint i, n values each)
    Last tokens: m capacities

All formats share the same layout; the 3rd header token is a random seed
for chubeas and the known optimum for gk and sac94.
"""

from pathlib import Path
from typing import List
import numpy as np
from .instance import MKPInstance


def _tokens(path: str) -> List[float]:
    """Read all whitespace-separated numeric tokens from a file."""
    with open(path) as f:
        return [float(t) for t in f.read().split()]


def parse_orlibrary(path: str) -> MKPInstance:
    """Parse an OR-Library format file (chubeas or gk)."""
    tok = _tokens(path)
    idx = 0

    n = int(tok[idx]); m = int(tok[idx+1]); extra = int(tok[idx+2])
    idx += 3

    profits = np.array(tok[idx:idx+n], dtype=np.int64)
    idx += n

    weights = np.array(tok[idx:idx+m*n], dtype=np.int64).reshape(m, n)
    idx += m * n

    capacities = np.array(tok[idx:idx+m], dtype=np.int64)

    name = Path(path).stem
    # For gk files the 3rd value is the known optimum; for chubeas it's a seed.
    # We store it as known_optimum (for chubeas it will be a seed value, not useful).
    known_optimum = extra if "gk" in str(path).lower() else -1

    return MKPInstance(n=n, m=m, profits=profits, weights=weights,
                       capacities=capacities, name=name,
                       known_optimum=known_optimum)


def parse_sac94(path: str) -> MKPInstance:
    """Parse a SAC94 format file (sac94/ subdirectories).

    Same token layout as OR-Library; the 3rd header value is the known optimum.
    """
    tok = _tokens(path)
    idx = 0

    n = int(tok[idx]); m = int(tok[idx+1]); known_optimum = int(tok[idx+2])
    idx += 3

    profits = np.array(tok[idx:idx+n], dtype=np.int64)
    idx += n

    weights = np.array(tok[idx:idx+m*n], dtype=np.int64).reshape(m, n)
    idx += m * n

    capacities = np.array(tok[idx:idx+m], dtype=np.int64)

    name = Path(path).stem
    return MKPInstance(n=n, m=m, profits=profits, weights=weights,
                       capacities=capacities, name=name,
                       known_optimum=known_optimum)


def _is_sac94(path: str) -> bool:
    return "sac94" in str(path)


def parse_instance(path: str) -> MKPInstance:
    """Auto-detect format and parse."""
    if _is_sac94(path):
        return parse_sac94(path)
    return parse_orlibrary(path)


def load_dataset_dir(directory: str) -> List[MKPInstance]:
    """Load all .dat files from a directory (non-recursive).

    Skips the combined summary file whose stem matches the directory name
    (e.g. OR10x250/OR10x250.dat) — that file bundles all instances together
    and is not a valid individual benchmark instance.
    """
    dir_path = Path(directory)
    instances = []
    for p in sorted(dir_path.glob("*.dat")):
        if p.stem == dir_path.name:   # skip OR10x250/OR10x250.dat etc.
            continue
        try:
            instances.append(parse_instance(str(p)))
        except Exception as e:
            print(f"[parser] skipping {p.name}: {e}")
    return instances


def load_all_instances(base_dir: str) -> List[MKPInstance]:
    """Recursively load all .dat files from base_dir."""
    instances = []
    for p in sorted(Path(base_dir).rglob("*.dat")):
        try:
            instances.append(parse_instance(str(p)))
        except Exception as e:
            print(f"[parser] skipping {p}: {e}")
    return instances
