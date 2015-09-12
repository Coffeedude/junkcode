#!/usr/bin/env python

import sys


f = open(sys.argv[1], 'r')

strings = {}

for line in f:
    line = line.rstrip()
    if not line in strings:
        strings[line] = 1
    else:
        strings[line] += 1

for x in strings.keys():
    print "{0:>6}    {1}".format(strings[x], x)

f.close()

