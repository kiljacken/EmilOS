#!/usr/bin/python
""" Cog code generation tool.
    http://nedbatchelder.com/code/cog

    Copyright 2004-2005, Ned Batchelder.
"""

import time
start = time.clock()

import sys
import os

sys.path.append(os.path.dirname(__file__))
from cogapp import Cog

ret = Cog().main(sys.argv)

#print "Time: %.2f sec" % (time.clock() - start)
sys.exit(ret)
