#!/usr/bin/env python3
import argparse
import csv
from typing import Iterable, List, Optional, Sequence, Tuple, Union

Number = Union[int, float]


def median_low(values: Sequence[Number]) -> Number:
    sorted_values = sorted(values)
    n = len(sorted_values)
    return sorted_values[n // 2] if n % 2 == 1 else sorted_values[n // 2 - 1]


def parse_numeric_cells(cells: Sequence[str]) -> List[Number]:
    numbers: List[Number] = []
    for cell in cells:
        cell = cell.strip()
        if not cell:
            continue
        try:
            numbers.append(int(cell, 0))
        except ValueError:
            try:
                numbers.append(float(cell))
            except ValueError:
                # Ignore non-numeric columns.
                continue
    return numbers


def compute_row_medians(
    csvfile: str,
    start_col: int = 1,
    end_col: Optional[int] = None,
    delimiter: str = ",",
) -> List[Tuple[str, Number]]:
    output: List[Tuple[str, Number]] = []

    with open(csvfile, mode="r", newline="") as file:
        reader = csv.reader(file, delimiter=delimiter)
        for row in reader:
            if not row:
                continue

            label = row[0]
            values = parse_numeric_cells(row[start_col:end_col])
            if not values:
                continue

            output.append((label, median_low(values)))

    return output


def iter_row_median_lines(rows: Iterable[Tuple[str, Number]]) -> Iterable[str]:
    for label, median in rows:
        yield f"{label},{median}"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Export median value for each CSV row."
    )
    parser.add_argument(
        "csvfile",
        help="Input CSV file path.",
    )
    parser.add_argument(
        "--start-col",
        type=int,
        default=1,
        help="First column index (0-based) to consider for median (default: 1).",
    )
    parser.add_argument(
        "--end-col",
        type=int,
        default=None,
        help="Optional exclusive end column index (0-based).",
    )
    parser.add_argument(
        "--delimiter",
        default=",",
        help="CSV delimiter (default: ',').",
    )

    args = parser.parse_args()
    rows = compute_row_medians(
        csvfile=args.csvfile,
        start_col=args.start_col,
        end_col=args.end_col,
        delimiter=args.delimiter,
    )
    for line in iter_row_median_lines(rows):
        print(line)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())