#!/usr/bin/python

import json
import argparse
import subprocess
import os

dirExec = os.path.dirname(os.path.realpath(__file__)) + '/'

parser = argparse.ArgumentParser(description = 'Set video device settings')
parser.add_argument('--dev', help = 'device human readable name', type = str)
parser.add_argument('--file', help = 'json file', type = str)
parser.add_argument('--list', help = 'list available video devices', action = 'store_true')
parser.add_argument('--default', help = 'set controls to default values', action = 'store_true')
args = parser.parse_args()

with open(dirExec + './vDevSetting.json') as pFile:
    devNames = json.load(pFile)

if args.list:
    for d in devNames.keys():
        print(d)
    exit(0)

if not args.dev:
    parser.print_help()
    exit(0)

res = subprocess.run([dirExec + './vDevFind.sh', devNames[args.dev]], stdout = subprocess.PIPE)
devFile = res.stdout.decode('utf-8').strip()

if not args.file:
    args.file = dirExec + './info/' + devFile.split('/')[-1] + '_v4l2-ctl.json'

#print('Device file %s' % devFile)
#print('Settings file %s' % args.file)

pSet = open(args.file)
settings = json.load(pSet)
pSet.close()

for c in settings['ctrls'].keys():

    #print('Setting %s = %s' % (s['name'], s['value']))

    if args.default:
        v = settings['ctrls'][c]['default']
    else:
        v = settings['ctrls'][c]['value']

    res = subprocess.run([dirExec + './vCtrlSet.sh', devFile, c, v],
                stdout = subprocess.PIPE,
            ).stdout.decode('utf-8').strip()
                #stderr = subprocess.DEVNULL,
    #print('Result %s' % res)

