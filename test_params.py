#!/usr/bin/env python3
import subprocess, shutil, os
from collections import namedtuple, OrderedDict
from copy import deepcopy

from plot import get_last_value

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
    paramdict = OrderedDict()
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

def print_kv(key, value):
    print('%-10s| %s' % (key, value))

def main():
    if os.path.exists('output/data'):
        shutil.rmtree('output/data')
        os.makedirs('output/data')
    subprocess.call('./build.sh', shell=True)
    defaultParams = read_param_file('params.txt')
    varianceSets  = generate_variances('variances.txt')
    params        = deepcopy(defaultParams) 
    try:
        for i, varSet in enumerate(varianceSets):
            for name, value in varSet:
                params[name] = value
                write_params(params, '__temp_params__.txt') 
                subprocess.call('./openlifecli __temp_params__.txt', shell=True)
                datadir = 'output/param_testing/data_%s' % i
                if os.path.exists(datadir):
                    shutil.rmtree(datadir)
                shutil.copytree('output/data', datadir)
                filename = 'output/param_testing/params_%s' % i
                if os.path.isfile(filename):
                    os.remove(filename)
                shutil.copy('__temp_params__.txt', filename)
                print_kv(name, value)
                print_kv('Entropy', get_last_value('entropy'))
            params = deepcopy(defaultParams)
    finally:
        os.remove('__temp_params__.txt')

if __name__ == '__main__':
    main()
