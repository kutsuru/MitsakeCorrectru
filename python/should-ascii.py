#! /usr/bin/python2.6
# -*- coding: utf-8 -*-
import sys

def main(args):
    used = []
    m = 0
    with open(args[1], "r") as f:
        for line in f:
            word, freq = line.split()
            map(lambda x: x not in used and used.append(x), word)
            m = max(int(freq), m)
    print sorted(used)
    print len(used)
    print "max freq:", m

if __name__ == "__main__":
    main(sys.argv)
