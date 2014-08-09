from __future__ import print_function
from collections import Counter

inputFile = open('testData.txt', 'r')
lines = inputFile.read().split()
counter = Counter(lines);
items = sorted(counter.items(), key=lambda x: int(x[0]))

outputFile = open('testFrequencies.txt', 'w')
for num,frequency in items:
    print(num + '\t'+ str(frequency), file=outputFile)
