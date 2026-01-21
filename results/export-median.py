#!/usr/bin/python
import csv
import sys
import numpy as np

csvfile = 'all.csv'

if len(sys.argv) > 1:
    csvfile = sys.argv[1]

results = []
timings = {}
scores = {}
median_scores = {}

with open('timings.csv', mode='r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        timings[row[0]] = row[-1]

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
    output = label
    for index in median_scores[label]:
        output += "," + str(median_scores[label][index])
    print(output)
