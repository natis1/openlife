#!/usr/bin/env python3
import os, sys, subprocess
import networkx as nx
import matplotlib.pyplot as plt

from plot import read_csv, find_filenames
from math import sqrt

def distance(x1, y1, x2, y2):
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)

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

    content = read_csv(filename)

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
    networks = inner_networks(edges)
    print('Edge set of %s edges contains %s networks' % (len(edges), len(networks)))

    G=nx.Graph()#  G is an empty Graph
    G.add_nodes_from(range(len(content)))
    G.add_edges_from(edges)
    nx.draw_spectral(G, node_color='c', edge_color='k', with_labels=True)
    basename = os.path.splitext(os.path.basename(filename))[0]
    plt.savefig('images/' + basename + '.png')

def main():
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

if __name__ == '__main__':
    main()

