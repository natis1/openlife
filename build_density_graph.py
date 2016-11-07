#!/usr/bin/env python3
import os
import matplotlib.pyplot as plt

def calc_average_loc(filename):
    convert = lambda line : tuple(float(value) for value in line.split(','))
    with open(filename) as csvfile:
        content = [convert(line) for line in csvfile]
    if len(content) == 0:
        return 0, 0
    xsum = sum(point[0] for point in content)
    ysum = sum(point[1] for point in content)
    return xsum / len(content), ysum / len(content)

# Recursively build list of data files
filenames   = [filename for _, _, filenames in os.walk('data') for filename in filenames]
averagelocs = [calc_average_loc('data/' + filename) for filename in filenames]

print(averagelocs)

plt.plot(averagelocs)
