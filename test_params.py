#!/usr/bin/env python3
import subprocess, shutil, os

import matplotlib.pyplot as plt

from collections import namedtuple, OrderedDict
from pprint      import pprint
from copy        import deepcopy

import plot

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

def get_metrics(iterations=1):
    metrics = OrderedDict()
    for key in plot.view_keys:
        metrics[key] = [] 
    for _ in range(iterations):
        subprocess.call('./openlifecli __temp_params__.txt', shell=True)
        for key in plot.view_keys:
            metrics[key].append(plot.get_last_value(key)) 
        print('.', end='', flush=True)
    print('')
    for key in plot.view_keys:
        try:
            metrics[key] = sum(metrics[key]) / len(metrics[key]) 
        except:
            pass
    return metrics

def clean_output(i):
    datadir = 'output/param_testing/data_%s' % i
    if os.path.exists(datadir):
        shutil.rmtree(datadir)
    shutil.copytree('output/data', datadir)
    filename = 'output/param_testing/params_%s' % i
    if os.path.isfile(filename):
        os.remove(filename)
    shutil.copy('__temp_params__.txt', filename)

def clean_data_dir():
    if os.path.exists('output/data'):
        shutil.rmtree('output/data')
        os.makedirs('output/data')

def print_kv(key, value):
    print('%-18s| %s' % (key, value))

def print_metrics(metrics, name, value):
    print_kv(name, value)
    for k, v in metrics.items():
        print_kv(k, v)
    print('-' * 80)

def plot_metrics(metricDicts, metricKeys=['network_size']):
    for metricName, valueDict in metricDicts.items():
        x = list(valueDict.keys())
        arglist = []
        for key in metricKeys:
            if key != 'location':
                arglist.append(x)
                plotItems = [v[key] for (k, v) in valueDict.items()]
                arglist.append(plotItems)
        plt.plot(*arglist)
        #plt.ylabel(metric)
        #plt.xlabel(metricName)
        plt.show()
        plt.savefig('output/images/metrics.png')

def main():
    clean_data_dir()

    subprocess.call('./build.sh', shell=True)
    defaultParams = read_param_file('params.txt')
    varianceSets  = generate_variances('variances.txt')
    params        = deepcopy(defaultParams) 

    metricDicts = OrderedDict()
    for i, varSet in enumerate(varianceSets):
        for name, value in varSet:
            params[name] = value
            write_params(params, '__temp_params__.txt') 
            metrics = get_metrics(iterations=10)
            print_metrics(metrics, name, value)
            if name not in metricDicts:
                metricDicts[name] = OrderedDict()
            metricDicts[name][value] = metrics
            clean_output(i)

        params = deepcopy(defaultParams)
    plot_metrics(metricDicts, ['network_count', 'network_size'])
    #plot_metrics(metricDicts, [''])

    if os.path.isfile('__temp_params__.txt'):
        os.remove('__temp_params__.txt')

if __name__ == '__main__':
    main()
