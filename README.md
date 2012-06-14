MitsakeCorrectru
================

Spell Checker in C++,
tightly packed tries implementation and adaptation:
    http://aclweb.org/anthology-new/W/W09/W09-1505.pdf

Description
-----------

Two Tries implementation:
  * the dummy one: C++, no optimization, used by the compiler
  * the packed one: using a tight C representation,
                    mmaped from the compiler version

moulinette:
  generator.py dic1.txt dic2.txt...

1. compile each dict with the reference and our implementation as long as the
  ref binaries are in the test folder.
2. Then build a test file by scrambling the dictionnary words
3. Generate a yaml test file and the dict test folder

  moulinette.py

Run the test suite

* -t to time the executions
* -v to activate verbose mode

Build & Use
-----------

    # generate the TextMiningCompiler and TextMiningApp
    make

    # generate the documentation
    make doc

    # generate the test suite (using the txt files in the tests folder)
    make gen

    # run the test suite
    make check

    # bench the test suite
    make time

    # basic test to check that the chain is working
    make test

TODO
----

* precomupte the offset shifting when starting the application
* compress the tree (not simply the trie representation) using the LinkLetter
* add more precise tests
* optimize more

