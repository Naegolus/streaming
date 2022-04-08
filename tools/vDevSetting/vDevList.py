#!/usr/bin/python

import json
import argparse
import subprocess
import os
import glob

dirExec = os.path.dirname(os.path.realpath(__file__)) + '/'

devs = glob.glob('/dev/video*')

with open(dirExec + './vDevSetting.json') as pFile:
    devNames = json.load(pFile)

for d in devs:

    res = subprocess.run(['v4l2-ctl', '-D', '-d', d, '--all'], stdout = subprocess.PIPE)
    info = res.stdout.decode('utf-8').strip()

    if 'Format Meta' in info:
       continue

    idx = info.find('Bus info')
    idx = info.find(':', idx) + 1
    idxEnd = info.find('\n', idx)

    busInfo = info[idx:idxEnd].strip()

    print('%-20s' % d, end = '')
    print('%-30s' % busInfo, end = '')

    for n in devNames.keys():
        if devNames[n] == busInfo:
            print('%-15s' % n, end = '')
            break

    print('')

