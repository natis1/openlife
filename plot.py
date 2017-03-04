#!/usr/bin/env python3
import os, sys, subprocess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from math import sqrt, log # Seriously? lol

def read_csv(filename):
    convert = lambda line : tuple(float(value) for value in line.split(','))
    with open(filename) as csvfile:
        return [convert(line) for line in csvfile]

def find_filenames():
    return ['output/data/' + filename for _, _, filenames in os.walk('output/data') for filename in filenames]

def distance(x1, y1, x2, y2):
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)

def get_edges(points):
    edges = []
    for i, (x1, y1) in enumerate(points):
        def score(value):
            _, (x2, y2) = value
            return distance(x1, y1, x2, y2)
        others = points[:i] + points[i+1:] 
        # Find n nearest neighbors
        neighbors = 2
        nearest   = sorted(enumerate(others), key=score)[:neighbors]
        for node in nearest:
            edges.append((i, node[0]))
    return edges

def inner_networks(edges):
    networks = []
    for a, b in edges:
        inserted = False
        for network in networks:
            if a in network:
                network.add(b)
                inserted = True
            elif b in network:
                network.add(a)
                inserted = True
        if not inserted:
            networks.append(set())
            networks[-1].add(a)
            networks[-1].add(b)
    return networks

def get_networks(points):
    edges    = get_edges(points)
    return inner_networks(edges)

def calc_num_networks(points):
    return len(get_networks(points))

def calc_network_size(points):
    networks = get_networks(points)
    sizes    = [len(network) for network in networks]
    if len(sizes) == 0:
        return 0
    return sum(sizes) / len(sizes)

def calc_average_loc(points):
    if len(points) == 0: # Cant find the average location of 0 cells
        return 0, 0
    xsum = sum(point[0] for point in points)
    ysum = sum(point[1] for point in points)
    return xsum / len(points), ysum / len(points)

def calc_density(points):
    return len(points)

def calc_area(points):
    x_min = 0
    x_max = 0
    y_min = 0
    y_max = 0
    for x, y in points:
        if x < x_min: x_min = x
        if x > x_max: x_max = x
        if y < y_min: y_min = y
        if y > y_max: y_max = y
    return abs(x_max - x_min) * abs(y_max - y_min)

boltz_k = 1.38064852 * 10**-23 
plank_constant = 6.62607004 * 10**-34
pi = 3.14159265359

def calc_entropy(points):
    # Formula for entropy:
    # S = Nk[ln( (V/N) * ((4pi*mU) / (3Nh^2))^(3/2) ) +  (5/2)]
    #     where k = boltzmann's constant
    #           h = plank's constant
    #           U = internal energy (Will be ignored) 
    #           Not clear what m is, but I think it's an atomic mass constant used with internal energy, meaning we ignore it
    #
    # (Since we aren't using units/dimensional analysis, we should be able to drop the constants, too, but I wont for now)
    # Our "simplified" version of the equation:
    #
    # S = Nk[ln( (V/N) * ((4pi) / (3Nh^2))^(3/2) ) +  (5/2)]
    #
    # http://hyperphysics.phy-astr.gsu.edu/hbase/Therm/entropgas.html
    # http://hyperphysics.phy-astr.gsu.edu/hbase/thermo/inteng.html#c2
    N = len(points)
    V = calc_area(points) # assuming a depth of one unit since we are in 2D space

    return (N * boltz_k * (
                log( (V/N) * 
                    ((4*pi) / (3*N*plank_constant**2))**(3/2) ) 
              + (5/2)))

# Entropy calculation without boltzmann and plank constant
def calc_entropy_2(points):
    N = len(points)
    V = calc_area(points) # assuming a depth of one unit since we are in 2D space
    return N * log(V / (N**2))

viewsTable = {
    'density'       : calc_density,
    'location'      : calc_average_loc,
    'network_count' : calc_num_networks,
    'network_size'  : calc_network_size,
    'area'          : calc_area,
    'entropy'       : calc_entropy,
    'entropy2'      : calc_entropy_2}

def get_values(view):
    filenames = sorted(find_filenames())
    return [viewsTable[view](read_csv(filename)) for filename in filenames]

def get_last_value(view):
    lastFile = sorted(find_filenames())[-1]
    return viewsTable[view](read_csv(lastFile))

view_keys = viewsTable.keys()

def main():
    filenames = sorted(find_filenames())

    if len(sys.argv) > 1:         
        view = sys.argv[1] 
        if view in viewsTable:    
            values = [viewsTable[view](read_csv(f)) for f in filenames]
            plt.plot(values)
            plt.ylabel(view)
            plt.xlabel('60 iterations')
            plt.savefig('output/images/%s.png' % view)
        else:
            raise ValueError('Unknown argument: %s\nValid arguments are:\n%s' % (view, '\n'.join(viewsTable.keys())))
    else:
        raise ValueError('Expected more arguments')

if __name__ == '__main__':
    main()
