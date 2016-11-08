#!/usr/bin/env python3
import os, sys, subprocess
import networkx as nx
import matplotlib.pyplot as plt

from plot import read_csv, find_filenames
from math import sqrt

def distance(x1, y1, x2, y2):
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)

def graph(filename):

    content = read_csv(filename)

    edges = []
    for i, (x1, y1) in enumerate(content):
        for j, (x2, y2) in enumerate(content[i:]):
            if distance(x1, y1, x2, y2) < 100:
                edges.append((i, j))

    G=nx.Graph()#  G is an empty Graph
    G.add_nodes_from(range(len(content)))
    G.add_edges_from(edges)
    nx.draw(G, node_color='c',edge_color='k', with_labels=True)
    basename = os.path.splitext(os.path.basename(filename))[0]
    plt.savefig('images/' + basename + '.png')

def main():
    filenames = find_filenames() 
    for filename in filenames:
        graph(filename)
    os.chdir('images')
    subprocess.run(['ffmpeg', '-f', 'image2', '-r', '1', '-i', 'simulation_%05d.png', '-vcodec', 'mpeg4', '-y', 'network.mp4'])
    os.chdir('..')

if __name__ == '__main__':
    main()

