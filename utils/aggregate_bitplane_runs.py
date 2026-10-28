#!/usr/bin/env python3
"""Aggregate per-bitplane scores across run directories.

Given a directory like:
    results/128x128x128x1/3x3x3x1/

This script reads all `runN/*.txt` files, parses the second line in each file,
and computes min/max/median for each of the 32 bitplane scores per hash.
"""

from __future__ import annotations

import argparse
import csv
import statistics
import sys
from pathlib import Path
from typing import Callable


ScoreList = list[int]
Aggregated = dict[str, ScoreList]


def _parse_result_file(file_path: Path) -> tuple[str, list[int]]:
    """Parse a result file and return (hash_name, 32 scores)."""
    with file_path.open("r", encoding="utf-8") as f:
        lines = f.read().splitlines()

    if len(lines) < 2:
        raise ValueError(f"File has fewer than 2 lines: {file_path}")

    fields = [field.strip() for field in lines[1].split(",") if field.strip() != ""]
    if len(fields) != 33:
        raise ValueError(
            f"Expected 33 comma-separated fields on line 2 (hash + 32 scores), "
            f"got {len(fields)} in {file_path}"
        )

    hash_name = fields[0]
    try:
        scores = [int(value) for value in fields[1:]]
    except ValueError as exc:
        raise ValueError(f"Non-numeric score in {file_path}: {exc}") from exc

    return hash_name, scores


def _stat_func(name: str) -> Callable[[list[int]], int]:
    """Return the aggregation function for a stat name."""
    stat_name = name.lower()
    if stat_name == "min":
        return min
    if stat_name == "max":
        return max
    if stat_name == "median":
        # There are exactly 9 runs, so the median of integer values is an integer.
        return lambda values: int(statistics.median(values))
    raise ValueError(f"Unknown stat '{name}'. Use one of: min, max, median")


def compute_bitplane_stat(base_dir: str | Path, stat: str = "median") -> Aggregated:
    """Compute per-hash, per-bitplane stat across all runs.

    Args:
        base_dir: Directory containing `run1`, `run2`, ... subdirectories.
        stat: One of "min", "max", or "median".

    Returns:
        Dict mapping hash name -> list of 32 aggregated scores.
    """
    root = Path(base_dir)
    if not root.is_dir():
        raise ValueError(f"Not a directory: {root}")

    run_dirs = sorted(path for path in root.iterdir() if path.is_dir() and path.name.startswith("run"))
    expected_runs = 9
    if len(run_dirs) != expected_runs:
        raise ValueError(
            f"Expected exactly {expected_runs} run directories under {root}, found {len(run_dirs)}: "
            f"{', '.join(path.name for path in run_dirs)}"
        )

    per_hash_per_bitplane: dict[str, list[list[int]]] = {}

    for run_dir in run_dirs:
        txt_files = sorted(run_dir.glob("*.txt"))
        if not txt_files:
            raise ValueError(f"No .txt files found in {run_dir}")

        for txt_file in txt_files:
            hash_name, scores = _parse_result_file(txt_file)
            buckets = per_hash_per_bitplane.setdefault(hash_name, [[] for _ in range(32)])
            for bitplane_idx, score in enumerate(scores):
                buckets[bitplane_idx].append(score)

    reducer = _stat_func(stat)
    result: Aggregated = {}
    for hash_name, bitplane_buckets in per_hash_per_bitplane.items():
        aggregated_scores: ScoreList = []
        for idx, values in enumerate(bitplane_buckets):
            if len(values) != expected_runs:
                raise ValueError(
                    f"Expected {expected_runs} values for hash '{hash_name}', "
                    f"bitplane {idx}, found {len(values)}"
                )
            aggregated_scores.append(reducer(values))
        result[hash_name] = aggregated_scores

    return result


def _write_csv(result: Aggregated, output_file: Path) -> None:
    """Write aggregated results as CSV: hash,bit0,...,bit31."""
    header = ["hash"] + [f"bit{i}" for i in range(32)]
    with output_file.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        for hash_name in sorted(result):
            writer.writerow([hash_name] + result[hash_name])


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Aggregate min/max/median per bitplane across run directories"
    )
    parser.add_argument(
        "base_dir",
        help="Directory containing run1..run9, e.g. results/128x128x128x1/3x3x3x1",
    )
    parser.add_argument(
        "--stat",
        default="median",
        choices=["min", "max", "median"],
        help="Aggregation function across runs (default: median)",
    )
    parser.add_argument(
        "--output",
        help="Optional output CSV path. If omitted, prints CSV to stdout.",
    )
    args = parser.parse_args()

    result = compute_bitplane_stat(args.base_dir, args.stat)

    if args.output:
        output_file = Path(args.output)
        _write_csv(result, output_file)
        print(f"Wrote {args.stat} aggregates for {len(result)} hashes to {output_file}")
        return

    writer = csv.writer(sys.stdout)
    writer.writerow(["hash"] + [f"bit{i}" for i in range(32)])
    for hash_name in sorted(result):
        writer.writerow([hash_name] + result[hash_name])


if __name__ == "__main__":
    main()
