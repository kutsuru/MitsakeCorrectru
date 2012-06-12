#! /usr/bin/python2
# -*- coding: utf-8 -*-
import sys
import subprocess
import random

def mixup_word(word):
    for i in range(random.randint(0, 5)):
        word = random.choice([
            lambda x: "".join(random.sample(x, len(x))),
            ])(word)

    return word

def gen_test(words):
    r = []
    for i in range(len(words) * 10):
        d = random.randint(0, 5)
        word = mixup_word(random.choice(words))
        r.append("approx %s %s\n" % (d, word))

    return r


def main(args):
    files = args[1:]
    comp_refp = "./TextMiningCompiler"
    comp_usp = "../compiler"

    for path in files:
        with open(path, "r") as f:
            lines = f.readlines()

        print "compiling [ref]: ", path
        subprocess.call([comp_refp, path, path + ".ref"])

        # TODO: lock on -c with our compiler!
        print "compiling [us]: ", path
        subprocess.call([comp_usp, path, path + ".us"])

        print "test generation"
        words = map(lambda x: x.split()[0], lines)
        lines = gen_test(words)
        with open(path + ".tst", "w") as f:
            f.writelines(lines)

if __name__ == "__main__":
    main(sys.argv)
