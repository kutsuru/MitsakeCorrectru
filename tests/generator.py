#! /usr/bin/python2
# -*- coding: utf-8 -*-
import sys
import subprocess
import random
import os

sys.path.append("lib")
import yaml

def mixup_word(word):
    for i in range(random.randint(0, 5)):
        word = random.choice([
            lambda x: "".join(random.sample(x, len(x))),
            ])(word)

    return word


def gen_test(words, folder, prefix, app_refp, app_usp):

    i = 0
    l = len(words)
    step = 1000

    while (i * step < l):
        print "test-id:", i
        lines = []
        for j in range(step):
            d = random.randint(0, 3)
            word = mixup_word(random.choice(words))
            lines.append("approx %s %s\n" % (d, word))

        with open(os.path.join(folder, prefix + str(i) + ".tst"), "w") as f:
            f.writelines(lines)

        dic = {
            "ref": "cat %s | %s %s 2>/dev/null" % (prefix + str(i) + ".tst", app_refp, prefix + ".ref"),
            "command": "cat %s | %s %s 2>/dev/null" % (prefix + str(i) + ".tst", app_usp, prefix + ".us"),
            "stdout": "",
            "stderr": "",
            "retval": "",
            "info": "",
        }

        with open(os.path.join(folder, prefix + str(i) + ".yml"), "w") as f:
            yaml.dump(dic, f)

        i += 1

    return None


def main(args):
    files = args[1:]
    comp_refp = "./TextMiningCompiler"
    comp_usp = "../compiler"

    app_refp = "../../TextMiningApp"
    app_usp = "../../app"

    for path in files:
        prefix = ".".join(path.split(".")[:-1])
        folder = "chapter_%s" % prefix

        try:
            os.mkdir(folder)
        except Exception as e:
            print "mkdir %s failed, exception: %s" % (folder, e)

        path_ref = os.path.join(folder, prefix + ".ref")
        path_us = os.path.join(folder, prefix + ".us")

        with open(path, "r") as f:
            lines = f.readlines()

        print "compiling [ref]: ", path
        subprocess.call([comp_refp, path, path_ref])

        # TODO: lock on -c with our compiler!
        print "compiling [us]: ", path
        subprocess.call([comp_usp, path, path_us])

        print "test generation"
        words = map(lambda x: x.split()[0], lines)
        gen_test(words, folder, prefix, app_refp, app_usp)


if __name__ == "__main__":
    main(sys.argv)
