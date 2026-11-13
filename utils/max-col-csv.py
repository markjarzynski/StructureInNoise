#!/usr/bin/env python3

import numpy as np
import sys

if __name__ == "__main__":

    files = sys.argv[1:]

    # Read 32 score columns after the hash label.
    data = [np.loadtxt(file, delimiter=",", dtype=np.uint64, skiprows=1, usecols=range(1,33)) for file in files]

    data = np.vstack(data)
    data = np.nanmax(data, axis=0)

    output = ""
    with open(files[0], 'r') as f:
        output = f.readline()
        output += f.readline().split(',')[0]
        
    for i in data:
        output += "," + str(i)

    print(output)
