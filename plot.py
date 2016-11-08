#!/usr/bin/env python3
import os, sys
import numpy as np
import matplotlib.pyplot as plt

def read_csv(filename):
    convert = lambda line : tuple(float(value) for value in line.split(','))
    with open(filename) as csvfile:
        return [convert(line) for line in csvfile]


def calc_average_loc(filename):
    content = read_csv(filename)
    if len(content) == 0: # Cant find the average location of 0 cells
        return 0, 0
    xsum = sum(point[0] for point in content)
    ysum = sum(point[1] for point in content)
    return xsum / len(content), ysum / len(content)

def calc_density(filename):
    return len(read_csv(filename)) / 4 

def find_filenames():
    return ['data/' + filename for _, _, filenames in os.walk('data') for filename in filenames]

viewsTable = {
    'density'  : calc_density,
    'location' : calc_average_loc}

def main():
    # Recursively build list of data files
    filenames = find_filenames()

    if len(sys.argv) > 1:         # Ignore first argument, which is filepath of script
        view = sys.argv[1] # View type string is second argument
        if view in viewsTable:    
            values = [viewsTable[view](filename) for filename in filenames]
        else:
            raise ValueError("Unknown view: %s" % view)
        plt.plot(values)
        plt.ylabel(view)
        plt.xlabel('60 iterations')
        plt.show()

if __name__ == '__main__':
    main()
