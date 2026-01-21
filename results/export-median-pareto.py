#!/usr/bin/python
import csv
import sys
import numpy as np

csvfile = 'all.csv'

if len(sys.argv) > 1:
    csvfile = sys.argv[1]

results = []
timings = {}
median_timings = {}
scores = {}
median_scores = {}

# read timings
with open('timings.csv', mode='r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        label = row[0]
        for index, col in enumerate(row[1:-1]):
            if not label in timings:
                timings[label] = {}
            if not index in timings[label]:
                timings[label][index] = []
            timings[label][index].append(col)

# find the median timing
for label in timings:
    for index in timings[label]:
        data = np.array(timings[label][index], dtype=np.float32)
        data = np.sort(data)
        median = data[data.size // 2] if data.size % 2 == 1 else data[data.size // 2 - 1]

        if not label in median_timings:
            median_timings[label] = median

with open(csvfile, mode='r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        label = row[0]
        for index, col in enumerate(row[1:-1]):
            if not label in scores:
                scores[label] = {}
            if not index in scores[label]:
                scores[label][index] = []
            scores[label][index].append(col)

# find the median score per bit-plane
for label in scores:
    for index in scores[label]:
        data = np.array(scores[label][index], dtype=np.uint64)
        data = np.sort(data)
        median = data[data.size // 2] if data.size % 2 == 1 else data[data.size // 2 - 1]

        if not label in median_scores or median < median_scores[label][0]:
            median_scores[label] = (median, index)

# find the best score among all bit-planes per hash
for key in median_scores:
    if key in median_timings:
        results.append((float(median_timings[key]), median_scores[key][0], median_scores[key][1], key))

results.sort()

prev_score = float("infinity")
for index, result in enumerate(results):

    timing = result[0]
    score = result[1]
    bitplane = result[2]
    label = result[3]

    if score < prev_score:
        print(f"{timing:.3f},{score},{bitplane},{label}")
        prev_score = score

