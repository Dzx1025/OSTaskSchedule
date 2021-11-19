#!/usr/bin/env python
import sys
import os

options = ['FCFS', 'SJF', 'RR', 'HRRN']
if len(sys.argv) >= 2:
    option = sys.argv[1]
    if option in options:
        os.system("echo Use %s Algorithm" % option)
        os.system("cmake -D DEFINE_FCFS=OFF -D DEFINE_SJF=OFF -D DEFINE_RR=OFF -D DEFINE_HRRN=OFF build")
        os.system("cmake -D DEFINE_%s=ON build" % option)
        os.system("cmake --build build")
    else:
        print("Invalid argv: " + option)
else:
    print('Please enter the parameters')
