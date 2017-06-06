#!/usr/bin/python

import os
import sys
import pprint
import xml.etree.ElementTree as ET

def parse_param(l):
    cmd_param = {}

    cmd_param['name'] = l[0].find('Name').text.strip()

    param_type = l[0].find('Type').text

    cmd_param['meaning'] = l[0].find('Meaning').text

    if(param_type == None or cmd_param['name'] == None):
        return None

    cmd_param['type'] = param_type.lower().strip()

    return cmd_param

def load_xml_file(fileName):
    root = ET.parse(fileName).getroot()
    cmdlist = root.find('CmdList')

    all_items = cmdlist.findall('CMD')
    cmds = []
    for item in all_items:
        cmd = {}


        cmd['name'] = item.find('Name').text
        cmd['opcode'] = item.find('OPCode').text
        if cmd['name'].endswith('RES'):
            cmd['opcode'] = "0x%X" % (int(cmd['opcode'], 16) | 0x4000)

        cmd['function'] = item.find('Function').text

        tag = cmd['name'].split('_')[1].lower()
        # if tag =='hid':
            # continue

        cmd_params = []

        params = item.findall('Param')

        count = 0
        while count < len(params):
            h = parse_param(params[count:])
            if(h != None):
                cmd_params.append(h)
            count += 1

        cmd['params'] = cmd_params
        cmds.append(cmd)


    # pp = pprint.PrettyPrinter(indent=4)
    # pp.pprint(cmds)
    return cmds

if __name__ == "__main__":
    file = sys.argv[1]
    if not os.path.exists(file):
        sys.exit(1)

    cmds = load_xml_file(file)
