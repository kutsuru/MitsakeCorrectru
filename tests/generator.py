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


def gen_test(words, path):

    with open(path, "w") as f:
        for i in range(len(words)):
            lines = []
            for j in range(random.randint(3, 10)):
                d = random.randint(0, 5)
                word = mixup_word(random.choice(words))
                lines.append("approx %s %s\n" % (d, word))
            f.writelines(lines)
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

        path_tst = os.path.join(folder, prefix + ".tst")
        path_ref = os.path.join(folder, prefix + ".ref")
        path_us = os.path.join(folder, prefix + ".us")
        path_yml = os.path.join(folder, prefix + ".yml")

        with open(path, "r") as f:
            lines = f.readlines()

        print "compiling [ref]: ", path
        subprocess.call([comp_refp, path, path_ref])

        # TODO: lock on -c with our compiler!
        print "compiling [us]: ", path
        subprocess.call([comp_usp, path, path_us])

        print "test generation"
        words = map(lambda x: x.split()[0], lines)
        lines = gen_test(words, path_tst)

        print "gen yaml"

        d = {
            "ref": "cat %s | %s %s 2>/dev/null" % (prefix + ".tst", app_refp, prefix + ".ref"),
            "command": "cat %s | %s %s 2>/dev/null" % (prefix + ".tst", app_usp, prefix + ".us"),
            "stdout": "",
            "stderr": "",
            "retval": "",
            "info": "",
        }

        with open(path_yml, "w") as f:
            yaml.dump(d, f)


if __name__ == "__main__":
    main(sys.argv)
