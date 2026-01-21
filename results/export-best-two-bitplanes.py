#!/usr/bin/python
import csv
import sys
import numpy as np

csvfile = 'all.csv'

if len(sys.argv) > 1:
    csvfile = sys.argv[1]

results = []
scores = {}
median_scores = {}

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

        if not label in median_scores:
            median_scores[label] = {}

        median_scores[label][index] = median

for label in median_scores:

    min_score0 = float('inf')
    min_key0 = 0
    min_score1 = float('inf')
    min_key1 = 0

    for index in median_scores[label]:
        score = median_scores[label][index];
        if score < min_score0:
            min_score1 = min_score0
            min_key1 = min_key1
            min_score0 = score
            min_key0 = index
        elif score < min_score1:
            min_score1 = score
            min_key1 = index

    results.append((label, min_score0, min_score1, min_key0, min_key1))

results.sort()

for index, result in enumerate(results):

    label = result[0]
    score0 = result[1]
    score1 = result[2]
    bitplane0 = result[3]
    bitplane1 = result[4]

    print(f'{label},{bitplane0},{bitplane1},{score0},{score1}')
