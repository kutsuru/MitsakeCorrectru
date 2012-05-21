#! /usr/bin/python2.6
# -*- coding: utf-8 -*-
import sys

class Node(object):
    def __init__(self, letter):
        self.letter = letter
        self.sons = {}
        self.is_word = False

    def add(self, word):
        l = word[0]
        if l not in self.sons:
            self.sons[l] = Node(l)
        if len(word) > 1:
            self.sons[l].add(word[1:])
        else:
            self.is_word = True

    def write(self, word, f):
        f.writelines(["    %s -> %s [label=%s];\n" % (word or "ROOT", word + letter, letter) for letter in self.sons.keys()])
        for (letter, son) in self.sons.items():
            son.write(word + letter, f)

    def count_lonely(self):
        lonely = 1 if len(self.sons) == 1 else 0
        counter = 1

        for sons in self.sons.values():
            count, alones = sons.count_lonely()
            counter += count
            lonely += alones

        return (counter, lonely)


def main(args):
    #words = {}
    path = args[1]

    print "load file"
    root = Node("")
    with open(path, "r") as f:
        lines = f.readlines()
        for l in lines:
            a, b = l.split()
            #words[a] = int(b)
            root.add(a)

    #print "construct node"
    #root = Node("")
    #for w in words.keys():
    #    root.add(w)

    print "count lonely nodes"
    count, lonely = root.count_lonely()
    print "total: %s nodes, lonely: %s nodes, percent: %s" % (count, lonely, lonely / float(count) * 100)

    print "plot"
    with open("out.dot", "w") as f:
        f.write("digraph g {\n")
        root.write("", f)
        f.write("}\n")



if __name__ == "__main__":
    main(sys.argv)
