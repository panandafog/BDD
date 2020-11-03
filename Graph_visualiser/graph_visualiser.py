#!./env/bin/python3

from graphviz import render
from os import walk
import re

dots_dir = "../Dot/"
dot_file_pattern = ".*.dot$"

files = []
for (dirpath, dirnames, filenames) in walk(dots_dir):
    files.extend(filenames)
    break

for filename in files:
    if re.match(dot_file_pattern, filename) != None:
        render('dot', 'png', dots_dir + filename)
