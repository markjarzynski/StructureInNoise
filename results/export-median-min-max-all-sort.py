#!/usr/bin/python
import csv
import sys
import numpy as np

csvfile = 'all.csv'

if len(sys.argv) > 1:
    csvfile = sys.argv[1]

results = []
#timings = {}
scores = {}
median_scores = {}
min_scores = {}
max_scores = {}

"""
with open('timings.csv', mode='r') as file:
    csv_reader = csv.reader(file)
    for row in csv_reader:
        timings[row[0]] = row[-1]
"""

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
        min_score = data[0]
        max_score = data[-1]

        if not label in median_scores or median < median_scores[label][0]:
            median_scores[label] = (median, index)

        if not label in min_scores or min_score < min_scores[label][0]:
            min_scores[label] = (min_score, index)

        if not label in max_scores or max_score > max_scores[label][0]:
            max_scores[label] = (max_score, index)

# find the best score among all bit-planes per hash
for key in median_scores:
    results.append((key, min_scores[key][0], median_scores[key][0], max_scores[key][0]))

results.sort()

for index, result in enumerate(results):

    label = result[0]
    label = label.replace("_", "\\_")
    min_score = result[1]
    median_score = result[2]
    max_score = result[3]    

    print(label + " & " + str(min_score) + " & " + str(median_score) + " & " + str(max_score) + " \\\\")
