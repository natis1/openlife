#!/usr/bin/env python3
import os, sys
import networkx as nx
import matplotlib.pyplot as plt

from plot import read_csv
from math import sqrt

def distance(x1, y1, x2, y2):
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)

def graph(filename):

    content = read_csv(filename)

    edges = []
    for i, (x1, y1) in enumerate(content):
        for j, (x2, y2) in enumerate(content[i:]):
            if distance(x1, y1, x2, y2) < 300:
                edges.append((i, j))
    print(edges)

    G=nx.Graph()#  G is an empty Graph
    G.add_nodes_from(range(len(content)))
    G.add_edges_from(edges)
    nx.draw(G, node_color='c',edge_color='k', with_labels=True)
    plt.show()

def main():
    filenames = sys.argv[1:]
    for filename in filenames:
        graph(filename)

if __name__ == '__main__':
    main()

