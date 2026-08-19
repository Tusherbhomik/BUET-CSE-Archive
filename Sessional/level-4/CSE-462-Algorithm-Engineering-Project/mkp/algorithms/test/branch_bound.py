"""
Python wrapper for the C++ Branch & Bound solver.

Serializes the MKP instance to a temp file, invokes the compiled binary,
and parses the result.

Skip instances with n > 150 (too slow within time limit).
"""

import subprocess
import tempfile
import os
from pathlib import Path
import numpy as np
from ...instance import MKPInstance

# Path to compiled binary (same directory as this file's cpp/ subfolder)
_CPP_DIR = Path(__file__).parent / "cpp"
_BINARY = _CPP_DIR / "bb"


def _ensure_binary():
    if not _BINARY.exists():
        raise FileNotFoundError(
            f"B&B binary not found at {_BINARY}. "
            f"Run `make` in {_CPP_DIR} first."
        )


def branch_bound(
    instance: MKPInstance,
    time_limit: float = 60.0,
    n_limit: int = 150,
):
    """
    Run Branch & Bound on the instance.

    Parameters
    ----------
    time_limit : seconds allowed for B&B
    n_limit    : skip instances with n > n_limit

    Returns
    -------
    solution : np.ndarray (n,)  or None if skipped
    profit   : int              or None if skipped
    feasible : bool             or None if skipped
    skipped  : bool
    """
    if instance.n > n_limit:
        return None, None, None, True

    _ensure_binary()

    # Build input string
    lines = []
    lines.append(f"{instance.n} {instance.m}")
    lines.append(" ".join(str(p) for p in instance.profits))
    for i in range(instance.m):
        lines.append(" ".join(str(w) for w in instance.weights[i]))
    lines.append(" ".join(str(c) for c in instance.capacities))
    lines.append(str(time_limit))
    inp = "\n".join(lines) + "\n"

    result = subprocess.run(
        [str(_BINARY)],
        input=inp,
        capture_output=True,
        text=True,
        timeout=time_limit + 10,
    )

    if result.returncode != 0:
        raise RuntimeError(f"B&B binary error: {result.stderr.strip()}")

    profit = None
    solution = None
    for line in result.stdout.splitlines():
        if line.startswith("PROFIT"):
            profit = int(line.split()[1])
        elif line.startswith("SOLUTION"):
            solution = np.array(list(map(int, line.split()[1:])), dtype=np.int64)

    if profit is None or solution is None:
        raise RuntimeError(f"Unexpected B&B output:\n{result.stdout}")

    feasible = bool(np.all(instance.weights @ solution <= instance.capacities))
    return solution, profit, feasible, False
