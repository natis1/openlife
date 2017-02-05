#!/usr/bin/env python3
import os, sys, subprocess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from math import sqrt

def read_csv(filename):
    convert = lambda line : tuple(float(value) for value in line.split(','))
    with open(filename) as csvfile:
        return [convert(line) for line in csvfile]


def find_filenames():
    return ['data/' + filename for _, _, filenames in os.walk('data') for filename in filenames]

def distance(x1, y1, x2, y2):
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)

def get_edges(filename):
    content = read_csv(filename)
    print('%s (%s)' % (filename, len(content)))

    edges = []
    for i, (x1, y1) in enumerate(content):
        def score(value):
            _, (x2, y2) = value
            return distance(x1, y1, x2, y2)
        others = content[:i] + content[i+1:] 
        # Find n nearest neighbors
        neighbors = 1
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

def graph(filename):
    edges    = get_edges(filename)
    networks = inner_networks(edges)
    print('Edge set of %s nodes and %s edges contains %s networks' % (len(content), len(edges), len(networks)))
    print(networks)

    G=nx.Graph()#  G is an empty Graph
    G.add_nodes_from(range(len(content)))
    G.add_edges_from(edges)
    nx.draw_spectral(G, node_color='c', edge_color='k', with_labels=True)
    basename = os.path.splitext(os.path.basename(filename))[0]
    plt.savefig('images/' + basename + '.png')

def build_network_video():
    try:
        filenames = find_filenames() 
        for filename in filenames:
            if filename.endswith('.csv'):
                print('Graphing %s' % filename)
                graph(filename)
    finally:
        os.chdir('images')
        subprocess.run(['ffmpeg', '-f', 'image2', '-r', '1', '-i', 'simulation_%05d.png', '-vcodec', 'mpeg4', '-y', 'network.mp4'])
        subprocess.run(['xdg-open', 'network.mp4'])
        os.chdir('..')

def get_networks(filename):
    content  = read_csv(filename)
    edges    = get_edges(filename)
    return inner_networks(edges)

def calc_average_loc(filename):
    content = read_csv(filename)
    if len(content) == 0: # Cant find the average location of 0 cells
        return 0, 0
    xsum = sum(point[0] for point in content)
    ysum = sum(point[1] for point in content)
    return xsum / len(content), ysum / len(content)

def calc_density(filename):
    content = read_csv(filename)
    print(len(content))
    return len(content)

def calc_num_networks(filename):
    return len(get_networks(filename))

def calc_network_size(filename):
    networks = get_networks(filename)
    sizes    = [len(network) for network in networks]
    if len(sizes) == 0:
        return 0
    return sum(sizes) / len(sizes)

viewsTable = {
    'density'      : calc_density,
    'location'     : calc_average_loc,
    'num_networks' : calc_num_networks,
    'network_size' : calc_network_size}

def main():
    filenames = sorted(find_filenames())

    if len(sys.argv) > 1:         
        view = sys.argv[1] 
        if view in viewsTable:    
            values = [viewsTable[view](filename) for filename in filenames]
            plt.plot(values)
            plt.ylabel(view)
            plt.xlabel('60 iterations')
            plt.show()
            plt.savefig(view + '.png')
        else:
            build_network_video()

if __name__ == '__main__':
    main()
