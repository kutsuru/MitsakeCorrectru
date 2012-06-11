#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import glob
import sys
import os
import time
import subprocess

sys.path.append("lib")

import yaml

from optparse import OptionParser

class Moulette:
    """ The Moulette's blueprint. """

    current_count = 0
    current_fail = 0
    current_succeed = 0

    test_count = 0
    test_fail = 0
    test_succeed = 0

    chapter_count = 0
    chapter_fail = 0
    chapter_succeed = 0

    verbose_mode = False
    info_mode = False
    chapter_mode = None

    # Colors

    OK = "\033[92m SUCCESS \033[0m"
    FAIL = "\033[91m FAILURE \033[0m"
    TIMEOUT = "\033[93m TIMEOUT \033[0m"
    PARSING_ERROR = "\033[93m PARSING ERROR \033[0m"

    # Timeout
    time = 5

    def __init__(self, option):
        self.verbose_mode = option.verbose
        self.info_mode = option.info
        self.chapter_mode = option.chapters
        self.time = option.time

    def __init_chapter__(self):
        self.current_count = 0
        self.current_fail = 0
        self.current_succeed = 0

    def __update_test_count__(self):
        self.test_count += self.current_count
        self.test_fail += self.current_fail
        self.test_succeed += self.current_succeed

    def __indent_output__(self, output):
        line = '\t\t\t'
        for char in output:
            line += char
            if char == '\n':
                print line,
                line = '\t\t\t'
        print

    def intro(self):
        print "Text Mining project's Moulette"
        print "Project checking phase activated ",
        for i in "......":
            sys.stdout.write(i)
            sys.stdout.flush()
            #time.sleep(0.5)
        print ".\n"

    def main(self):
        if self.chapter_mode:
            chapters_list = self.chapter_mode
        else:
            chapters_list = glob.glob("chapter_*")

        self.intro()
        for chapter in chapters_list:
            self.chapter(chapter)

        self.summary(self.test_succeed, self.test_fail, self.test_count)

    def chapter(self, chapter):
        test_files_list = glob.glob(chapter + "/*.yml")
        result = 0
        code = 0
        (path ,chapter_name) = os.path.split(chapter)

        self.__init_chapter__()
        self.chapter_count += 1

        print "\n\n\t>>> Chapter : " + chapter_name

        for test_file in test_files_list:
            code = self.test(test_file)
            if not result:
                result = code

        self.__update_test_count__()

        if result:
            self.chapter_fail += 1
            print "\t" + self.FAIL + " Chapter tested --- " + chapter_name
        else:
            self.chapter_succeed += 1
            print "\t" + self.OK + " Chapter tested --- " + chapter_name

        self.summary(self.current_succeed, self.current_fail, self.current_count)

    def test(self, test_file):
        test = open (test_file, 'r')

        try:
            data = yaml.load(test)
        except:
            print "\t\t" + self.PARSING_ERROR + " File tested --- "\
                    + test_file_name

        test.close()
        (path, test_file_name) = os.path.split(test_file)

        self.current_count += 1

        if data['ref']:
            ref_start_time = time.clock()
            reference_process = subprocess.Popen(['/bin/sh', '-c', data['ref']],
                                    stdin = subprocess.PIPE,
                                    stdout = subprocess.PIPE,
                                    stderr = subprocess.PIPE)
            self.ref_time = time.clock() - ref_start_time

            (ref_stdout, ref_stderr) = reference_process.communicate('')
            ref_rtcode = reference_process.returncode

        else:
            ref_stdout = data['stdout']
            ref_stderr = data['stderr']
            ref_rtcode = data['return']

        if self.info_mode and data['info']:
            print "\t >>> Info: " + data['info']

        process_start_time = time.clock()
        process = subprocess.Popen(['/bin/sh', '-c', data['command']],
                                    stdin = subprocess.PIPE,
                                    stdout = subprocess.PIPE,
                                    stderr = subprocess.PIPE)
        self.process_time = time.clock() - process_start_time

        (test_stdout, test_stderr) = process.communicate('')
        test_rtcode = process.returncode

        result = 0

        if (test_stdout == ref_stdout and test_stderr == ref_stderr and
                test_rtcode == ref_rtcode):

            self.current_succeed += 1
            print "\t\t" + self.OK + " File tested --- " + test_file_name

        else:
            self.current_fail += 1

            print "\t\t" + self.FAIL + " File tested --- " + test_file_name

            if self.verbose_mode:
                print
                if test_stdout != ref_stdout:
                    print "\t\t\t\033[91mBad stdout, unexpected:\
                            \n\t\t\t\033[0m"
                    self.__indent_output__(test_stdout)
                    print "\t\t\t\033[92mExpected:\n\t\t\t\033[0m"
                    self.__indent_output__(ref_stdout)
                if test_stderr != ref_stderr:
                    print "\t\t\t\033[91mBad stderr, unexpected:\
                            \n\t\t\t\033[0m" + test_stderr
                    print "\t\t\t\033[92mExpected:\n\t\t\t\033[0m"\
                            + ref_stderr
                if test_rtcode != ref_rtcode:
                    print "\t\t\t\033[91mBad code return, unexpected:\033[0m\
                            \n\t\t\t" + str(test_rtcode)
                    print "\t\t\t\033[92mExpected:\033[0m\n\t\t\t"\
                            + str(ref_rtcode)
                print

            result = 1

        if self.time:

            if data['ref']:
                print "\t\t\tRef time\t: \t" + str(self.ref_time)

            print "\t\t\tProcess time\t: \t\033[92m" + str(self.process_time) + "\033[0m"

        return result

    def summary(self, test_succeed, test_fail, test_count):
        print "\nSummary:"

        if self.test_count:
            print "\t\033[92m[%i%s][%i/%i]\033[0m tests succeeded this check" % (
                float(test_succeed) / float(test_count) * 100,
                u"\u0025", test_succeed, test_count)
            print "\t\033[91m[%i%s][%i/%i]\033[0m tests failed this check" % (
                float(test_fail) / float(test_count) * 100,
                u"\u0025", test_fail, test_count)
        else:
            print "\tNot a single test was parsed !!!"

# Options handling

def main():
    option_parser = OptionParser()

    option_parser.add_option("-v", "--verbose",
                             action = "store_true",
                             dest = "verbose",
                             default = False,
                             help = "show unexpected & expected output")

    option_parser.add_option("-i", "--info",
                             action = "store_true",
                             dest = "info",
                             default = False,
                             help = "show information about each test")

    option_parser.add_option("-c", "--chapter",
                             action = "append",
                             dest = "chapters",
                             default = None,
                             help = "allow the user to specify chapter(s)")

    option_parser.add_option("-t", "--time",
                             action = "store_true",
                             dest = "time",
                             default = False,
                             help = "display the time process execution")

    (option, args) = option_parser.parse_args()

    # Moulette's creation

    moulette = Moulette(option)
    moulette.main()

if __name__ == '__main__':
    main()
