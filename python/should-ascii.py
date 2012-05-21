#! /usr/bin/python2.6
# -*- coding: utf-8 -*-
import sys

def main(args):
    used = []
    with open(args[1], "r") as f:
        for line in f:
            word, freq = line.split()
            map(lambda x: x not in used and used.append(x), word)
    print sorted(used)
    print len(used)


if __name__ == "__main__":
    main(sys.argv)
