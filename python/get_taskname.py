#!/usr/bin/python

import os
import sys
import string


def get_taskname(filename):
    
    name = filename.split('_')
    name[-1] = 'TASK'
    newname = []
    newname.append(''.join(name))
    
    index = 0
    for w in name:
        w = w.lower()
        name[index] = w[0].upper() + w[1:]
        index += 1
    newname.append(''.join(name))
    
    temp = filename.split('_')
    index = 0
    for w in temp:
        w = w.lower()
        temp[index] = w[0].upper() + w[1:]
        index += 1  
    newname.append(''.join(temp))
    
    return newname

    

if __name__ == "__main__":
    print get_taskname('BT_GEN_POWER_ON_REQ')
