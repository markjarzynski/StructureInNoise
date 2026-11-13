Structure in Noise
==================

Finding repeating patterns, artifacts and structure in PRNGs and hash functions.


Utilities: Bitplane Aggregation and Timing Join
-----------------------------------------------

The repository includes two helper scripts for combining run outputs with timing data:

- `utils/aggregate_bitplane_runs.py`
- `utils/export_timing_and_best_bitplane.py`

`aggregate_bitplane_runs.py`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aggregates per-hash, per-bitplane scores across `run1`..`run9` and computes one of:

- `min`
- `max`
- `median`

Example (print median aggregation to stdout):

```bash
python utils/aggregate_bitplane_runs.py \
	results/128x128x128x1/3x3x3x1 \
	--stat median
```

Example (write min aggregation to CSV):

```bash
python utils/aggregate_bitplane_runs.py \
	results/128x128x128x1/3x3x3x1 \
	--stat min \
	--output /tmp/agg_3x3_min.csv
```


`export_timing_and_best_bitplane.py`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Produces a CSV with:

- `hash`
- `median_timing`
- for `--best-bitplanes 1` (default):
	- `lowest_bitplane_score`
	- `lowest_bitplane_index`
- for `--best-bitplanes N`:
	- `best1_score`, `best1_index`, ..., `bestN_score`, `bestN_index`

Inputs:

- timings source: `results/timings.csv` or a precomputed `median-timings.csv`
- one or more score sources:
	- run directory (for example `results/128x128x128x1/5x5x5x1/`), or
	- CSV previously exported by `aggregate_bitplane_runs.py`
- optional: `--best-bitplanes N` to export the N best (lowest-score) bitplanes per hash

Example 1: timings.csv + run directory (compute median bitplane scores from runs):

```bash
python utils/export_timing_and_best_bitplane.py \
	results/timings.csv \
	results/128x128x128x1/5x5x5x1 \
	--score-stat median \
	--output results/5x5-timing-bestbit.csv
```

Example 2: median-timings.csv + run directory:

```bash
python utils/median.py results/timings.csv > /tmp/median-timings.csv

python utils/export_timing_and_best_bitplane.py \
	/tmp/median-timings.csv \
	results/128x128x128x1/5x5x5x1 \
	--score-stat median
```

Example 2b: median-timings.csv + run directory + top 3 best bitplanes:

```bash
python utils/export_timing_and_best_bitplane.py \
	/tmp/median-timings.csv \
	results/128x128x128x1/5x5x5x1 \
	--score-stat median \
	--best-bitplanes 3
```

Example 3: timings.csv + previously exported aggregate CSV:

```bash
python utils/aggregate_bitplane_runs.py \
	results/128x128x128x1/5x5x5x1 \
	--stat median \
	--output /tmp/agg_5x5.csv

python utils/export_timing_and_best_bitplane.py \
	results/timings.csv \
	/tmp/agg_5x5.csv \
	--output results/5x5-from-agg.csv
```


