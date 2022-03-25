#!/usr/bin/python

import json
import argparse
import v4l2
import subprocess

parser = argparse.ArgumentParser(description = 'Set video device settings')
parser.add_argument('--dev', help = 'device human readable name', type = str)
parser.add_argument('--file', help = 'json file', type = str)
parser.add_argument('--list', help = 'list available video devices', action = 'store_true')
args = parser.parse_args()

with open('./vDevSetting.json') as pFile:
    devNames = json.load(pFile)

if args.list:
    for d in devNames.keys():
        print(d)
    exit(0)

if not args.dev:
    parser.print_help()
    exit(0)

res = subprocess.run(['./vDevFind.sh', devNames[args.dev]], stdout = subprocess.PIPE)
devFile = res.stdout.decode('utf-8').strip()

if not args.file:
    args.file = "./info/" + devFile.split("/")[-1] + "_v4l2-ctl.json"

#print('Device file %s' % devFile)
#print('Settings file %s' % args.file)

with open(args.file) as pSet:

    settings = json.load(pSet)

    for s in settings['ctrls']:

        #print('Setting %s = %s' % (s['name'], s['value']))

        res = subprocess.run(['./vCtrlSet.sh', devFile, s['name'], s['value']],
                    stdout = subprocess.PIPE).stdout.decode('utf-8').strip()
        #print('Result %s' % res)

