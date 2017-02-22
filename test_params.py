#!/usr/bin/env python3
import subprocess, os
from collections import namedtuple
from copy import deepcopy

Variance = namedtuple('Variance', ['name', 'begin', 'end', 'step'])

def to_variance(line):
    name, begin, end, step = line.split()
    return Variance(name, int(begin), int(end), int(step))

def read_variances_file(filename):
    with open(filename, 'r') as variancefile:
        return [to_variance(line) for line in variancefile]

def variance_set(variance):
    varSet = []
    n = variance.begin
    while n < variance.end:
        varSet.append((variance.name, n))
        n += variance.step
    return varSet

def generate_variances(filename):
    variances = read_variances_file(filename)
    return (variance_set(variance) for variance in variances)

def read_param_file(filename):
    paramdict = dict()
    with open(filename, 'r') as params:
        for line in params:
            if len(line.split()) == 2:
                name, value = line.split()
                paramdict[name] = value
    return paramdict

def write_params(params, filename):
    with open(filename, 'w') as outfile:
        for param in params.items():
            outfile.write('%s %s\n' % param)

def main():
    defaultParams = read_param_file('params.txt')
    varianceSets  = generate_variances('variances.txt')
    params        = deepcopy(defaultParams) 
    for varSet in varianceSets:
        for name, value in varSet:
            params[name] = value
            write_params(params, '__temp_params__.txt') 
            subprocess.call('./openlifegui __temp_params__.txt', shell=True)
        params = deepcopy(defaultParams)

if __name__ == '__main__':
    main()
