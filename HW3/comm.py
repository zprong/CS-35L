#!/usr/bin/python

import random,sys,argparse,locale,string
from optparse import OptionParser

class comm:
    def __init__(self, args, shows_file1, shows_file2,
                 shows_both, is_unsorted):
        self.trPhrases = {} # phrases already traversed
        self.trArr = [] # empty array for appending strings
        self.isSorted1 = True
        self.isSorted2 = True
        if args[0] == "-":
            self.lines1 = sys.stdin.readlines()
        else:
            f1 = open(args[0], 'r')
            self.lines1 = f1.readlines()
            f1.close()

        if args[1] =="-":
            self.lines2 = sys.stdin.readlines()
        else:
            f2 = open(args[1], 'r')
            self.lines2 = f2.readlines()
            f2.close()
            
        self.shows_file1 = shows_file1 # False if -1 
        self.shows_file2 = shows_file2 # False if -2
        self.shows_both = shows_both   # False if -3
        self.is_unsorted = is_unsorted 
    
    def getFromFile1(self):
        return self.lines1[0]

    def isSorted(self):
        sortedVer1 = sorted(self.lines1)
        sortedVer2 = sorted(self.lines2)
        for i in range(len(sortedVer1)):
            if sortedVer1[i] != self.lines1[i]:
                self.isSorted1 = False
                break # Not equal!!!!!!
            else:
                continue
        for j in range(len(sortedVer2)):
            if sortedVer2[j] != self.lines2[j]:
                self.isSorted2 = False
                break
            else:
                continue
    
    def compareStrings(self): 
        count1 = 0
        count2 = 0
        while (count1 < len(self.lines1) and count2 < len(self.lines2)):
            if self.lines1[count1] < self.lines2[count2]:
                self.trArr.append(self.lines1[count1])
                self.trPhrases.update({self.lines1[count1]:1})
                count1 += 1
            elif self.lines1[count1] > self.lines2[count2]:
                self.trArr.append(self.lines2[count2])
                self.trPhrases.update({self.lines2[count2]:2})
                count2 += 1
            else: # the strings are equal
                self.trArr.append(self.lines1[count1])
                self.trPhrases.update({self.lines1[count1]:3})
                count1 += 1
                count2 += 1
        while count1 < len(self.lines1): # print out remaining lines
            self.trArr.append(self.lines1[count1])
            self.trPhrases.update({self.lines1[count1]:1})
            count1 += 1
        while count2 < len(self.lines2):
            self.trArr.append(self.lines2[count2])
            self.trPhrases.update({self.lines2[count2]:2})
            count2 +=1

    def printCommand(self):
        for trPhrase in self.trArr:
            if self.trPhrases[trPhrase] == 1 and self.shows_file1 == True:
                sys.stdout.write(trPhrase)
                continue
            elif self.trPhrases[trPhrase] == 2 and self.shows_file2 == True:
                if self.shows_file1 == True:
                    sys.stdout.write("\t")
                sys.stdout.write(trPhrase)
                continue
            # value of the key is 3
            elif self.trPhrases[trPhrase] == 3 and self.shows_both == True:
                if self.shows_file1 == True:
                    sys.stdout.write("\t")
                if self.shows_file2 == True:
                    sys.stdout.write("\t")
                sys.stdout.write(trPhrase)
                continue
                           
            
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog[OPTION]...FILE
Replicate the POSIX comm command."""
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-u", "--unsorted", action="store_true",
                      dest="is_unsorted", default=False)
    parser.add_option("-1", action="store_false", dest="shows_file1",
                      default=True)
    parser.add_option("-2", action="store_false", dest="shows_file2",
                      default=True)
    parser.add_option("-3", action="store_false", dest="shows_both",
                      default=True)
    options, args = parser.parse_args(sys.argv[1:])

    shows_file1 = options.shows_file1
    shows_file2 = options.shows_file2
    shows_both = options.shows_both
    is_unsorted = options.is_unsorted
    if len(args) != 2:
        parser.error("wrong number of operands")
    input_file1 = args[0]
    input_file2 = args[1]

    printer = comm(args, shows_file1, shows_file2, shows_both, is_unsorted)
    printer.isSorted()
    if printer.isSorted1 == False:
        print ("comm: file 1 is not in sorted order")
    else:
        if printer.isSorted2 == False:
            print ("comm: file 2 is not in sorted order")
        else:
            printer.compareStrings()
            printer.printCommand()

if __name__== "__main__":
    main()
