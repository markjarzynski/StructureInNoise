#!/usr/bin/env python3
"""Join timing medians with best bitplane scores per hash.

Inputs:
- Timing CSV (`timings.csv`) or precomputed `median-timings.csv` style CSV.
- One or more score sources:
  - Directory containing run1..run9 subdirectories, or
  - CSV exported by `aggregate_bitplane_runs.py`.

Output CSV columns:
- hash
- median_timing
- if --best-bitplanes=1 (default):
    - lowest_bitplane_score
    - lowest_bitplane_index
- if --best-bitplanes=N (>1):
    - best1_score, best1_index, ..., bestN_score, bestN_index
"""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Tuple

from aggregate_bitplane_runs import compute_bitplane_stat
from median import compute_row_medians


TimingMap = Dict[str, float]
BitplaneMap = Dict[str, List[int]]


def _canonical_hash_name(label: str) -> str:
    """Normalize labels to improve joins across naming variants.

    Examples:
    - city_3 -> city3
    - city3 -> city3
    - md5.x -> md5
    - iqint2.y -> iqint2
    """
    s = label.strip().lower()

    # Remove lane suffixes often used in per-component outputs.
    for suffix in (".x", ".y", ".z", ".w"):
        if s.endswith(suffix):
            s = s[: -len(suffix)]
            break

    s = s.replace("_", "")
    s = s.replace(".", "")
    return s


def _is_median_timing_csv(path: Path) -> bool:
    """Detect if the timing file is already in median form: hash,median."""
    with path.open("r", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        first = next(reader, None)

    if not first or len(first) < 2:
        return False

    if first[0].strip().lower() == "hash":
        return False

    try:
        float(first[1].strip())
        return True
    except ValueError:
        return False


def load_timing_medians(path: Path) -> TimingMap:
    """Load median timing per hash from timings.csv or median timings CSV."""
    timings: TimingMap = {}

    if _is_median_timing_csv(path):
        with path.open("r", newline="", encoding="utf-8") as f:
            reader = csv.reader(f)
            for row in reader:
                if len(row) < 2:
                    continue
                label = row[0].strip()
                if not label:
                    continue
                try:
                    timings[label] = float(row[1])
                except ValueError:
                    continue
        return timings

    for label, med in compute_row_medians(str(path), start_col=1):
        timings[label] = float(med)

    return timings


def load_aggregated_scores_from_csv(path: Path) -> BitplaneMap:
    """Load hash -> 32 bitplane scores from aggregate_bitplane_runs CSV output."""
    scores: BitplaneMap = {}
    with path.open("r", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        header = next(reader, None)
        if not header:
            raise ValueError(f"Empty score CSV: {path}")

        first = header[0].strip().lower()
        if first != "hash":
            raise ValueError(
                f"Expected first column header 'hash' in {path}, got '{header[0]}'"
            )

        if len(header) < 33:
            raise ValueError(
                f"Expected 33 columns (hash + 32 bitplanes) in {path}, got {len(header)}"
            )

        for row in reader:
            if len(row) < 33:
                continue
            label = row[0].strip()
            if not label:
                continue
            try:
                bitplanes = [int(value.strip()) for value in row[1:33]]
            except ValueError as exc:
                raise ValueError(f"Invalid bitplane score in {path}: {row}") from exc
            scores[label] = bitplanes

    return scores


def load_scores(score_input: Path, stat: str) -> BitplaneMap:
    """Load aggregated bitplane scores from a directory or aggregate CSV."""
    if score_input.is_dir():
        return compute_bitplane_stat(score_input, stat=stat)

    if score_input.is_file():
        return load_aggregated_scores_from_csv(score_input)

    raise ValueError(f"Score input is neither file nor directory: {score_input}")


def merge_score_inputs(score_inputs: Iterable[Path], stat: str) -> BitplaneMap:
    """Merge multiple score inputs; later inputs override earlier hash keys."""
    merged: BitplaneMap = {}
    for path in score_inputs:
        merged.update(load_scores(path, stat=stat))
    return merged


def find_best_bitplanes(bitplanes: List[int], count: int) -> List[Tuple[int, int]]:
    """Return the best bitplanes as (score, bitplane_index), sorted by score then index."""
    if len(bitplanes) != 32:
        raise ValueError(f"Expected 32 bitplanes, got {len(bitplanes)}")
    if count < 1:
        raise ValueError(f"best bitplane count must be >= 1, got {count}")
    if count > 32:
        raise ValueError(f"best bitplane count must be <= 32, got {count}")

    ordered = sorted((score, idx) for idx, score in enumerate(bitplanes))
    return ordered[:count]


def build_output_rows(
    timings: TimingMap,
    scores: BitplaneMap,
    best_bitplanes: int,
) -> List[Tuple[str, float, List[Tuple[int, int]]]]:
    """Build output rows with exact and canonical name matching."""
    timing_by_canon: Dict[str, Tuple[str, float]] = {}
    for hash_name, timing in timings.items():
        key = _canonical_hash_name(hash_name)
        if key not in timing_by_canon:
            timing_by_canon[key] = (hash_name, timing)

    rows: List[Tuple[str, float, List[Tuple[int, int]]]] = []
    unmatched: List[str] = []

    for score_hash, bitplanes in sorted(scores.items()):
        if score_hash in timings:
            timing_hash = score_hash
            timing_value = timings[score_hash]
        else:
            canon = _canonical_hash_name(score_hash)
            if canon not in timing_by_canon:
                unmatched.append(score_hash)
                continue
            timing_hash, timing_value = timing_by_canon[canon]

        best = find_best_bitplanes(bitplanes, best_bitplanes)
        rows.append((timing_hash, timing_value, best))

    if unmatched:
        print(
            f"Warning: {len(unmatched)} score hashes did not match timings (examples: "
            f"{', '.join(unmatched[:10])})",
            file=sys.stderr,
        )

    # Deduplicate rows by hash if multiple score-hash variants map to one timing hash.
    dedup: Dict[str, Tuple[str, float, List[Tuple[int, int]]]] = {}
    for row in rows:
        name, timing, best = row
        current = dedup.get(name)
        if current is None or best[0][0] < current[2][0][0]:
            dedup[name] = row

    return sorted(dedup.values(), key=lambda x: x[0])


def write_output(
    rows: List[Tuple[str, float, List[Tuple[int, int]]]],
    out_file: Path | None,
    best_bitplanes: int,
) -> None:
    """Write result CSV to file or stdout."""
    output = sys.stdout if out_file is None else out_file.open("w", newline="", encoding="utf-8")
    try:
        writer = csv.writer(output)
        if best_bitplanes == 1:
            writer.writerow(["hash", "median_timing", "lowest_bitplane_score", "lowest_bitplane_index"])
            for name, timing, best in rows:
                low_score, low_idx = best[0]
                writer.writerow([name, timing, low_score, low_idx])
        else:
            header = ["hash", "median_timing"]
            for i in range(1, best_bitplanes + 1):
                header.extend([f"best{i}_score", f"best{i}_index"])
            writer.writerow(header)
            for name, timing, best in rows:
                row = [name, timing]
                for score, idx in best:
                    row.extend([score, idx])
                writer.writerow(row)
    finally:
        if out_file is not None:
            output.close()


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Export hash median timings with lowest bitplane score and index"
    )
    parser.add_argument(
        "timings_csv",
        help="Path to timings.csv or median-timings.csv",
    )
    parser.add_argument(
        "score_inputs",
        nargs="+",
        help=(
            "One or more score inputs: directories containing run1..run9 or "
            "CSV files exported by aggregate_bitplane_runs.py"
        ),
    )
    parser.add_argument(
        "--score-stat",
        choices=["min", "max", "median"],
        default="median",
        help="Aggregation stat for directory score inputs (default: median)",
    )
    parser.add_argument(
        "--output",
        help="Optional output CSV path (default: stdout)",
    )
    parser.add_argument(
        "--best-bitplanes",
        type=int,
        default=1,
        help="Number of best (lowest-score) bitplanes to export per hash (default: 1)",
    )

    args = parser.parse_args()

    if args.best_bitplanes < 1 or args.best_bitplanes > 32:
        parser.error("--best-bitplanes must be between 1 and 32")

    timings = load_timing_medians(Path(args.timings_csv))
    scores = merge_score_inputs([Path(p) for p in args.score_inputs], stat=args.score_stat)
    rows = build_output_rows(timings, scores, best_bitplanes=args.best_bitplanes)

    out_file = Path(args.output) if args.output else None
    write_output(rows, out_file, best_bitplanes=args.best_bitplanes)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
