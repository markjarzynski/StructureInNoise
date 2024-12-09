#!/usr/bin/env python
import sys
import csv
import math

def read(filename):
    
    hashed_values = {}

    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in reader:
            hash = row[0]
            values = [int(v) if v != '' else 0 for v in row[1:]]
            values.sort(reverse=True)

            hashed_values = hashed_values | {hash: values}

    return hashed_values 

def dist(a, b):
    #return sum([abs(i-j) for i,j in zip(a,b)])
    return math.sqrt(sum([(i-j)*(i-j) for i, j in zip(a,b)]))   

def usage():
    print(f"Usage: {sys.argv[0]} <results.csv>")

if __name__ == '__main__':
        
    if len(sys.argv) < 3:
        usage()
        exit()

    hashed_values = read(sys.argv[1])
    test = hashed_values[sys.argv[2]]

    print(hashed_values)

    for key, value in hashed_values.items():
        score = dist(value,test)
        score = round(score, 2)
        print(key + ',' + str(score))
