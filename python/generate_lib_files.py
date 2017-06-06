#!/usr/bin/python

import os
import sys
import string
import parse_xml

from get_taskname import *

types = {
        "u8" : "uint8_t",
        "u16" : "uint16_t",
        "u24" : "uint24_t",
        "u32" : "uint32_t",
        "str" : "android::String8",
        "buff" : "android::String8",
        "bdaddr" : "uint64_t"
        }

names = {
        "gen" : "general",
        "cm"  : "connect",
        "hfp" : "phone",
        "av"  : "audio",
        "pbdl" : "phonebook",
        "spp" : "spp",
        "msg" : "message",
        "pan" : "pan",
        "hid" : "hid"
        }

	
def create_file_note(fh):
    fh.write('/**\n')
    fh.write('* Copyright @ 2014 - 2015 Suntec Software(Shanghai) Co., Ltd.\n')
    fh.write('* All Rights Reserved.\n')
    fh.write('*\n')
    fh.write('* Redistribution and use in source and binary forms, with or without\n')
    fh.write('* modification, are NOT permitted except as agreed by\n')
    fh.write('* Suntec Software(Shanghai) Co., Ltd.\n')
    fh.write('*\n')
    fh.write('* Unless required by applicable law or agreed to in writing, software\n')
    fh.write('* distributed under the License is distributed on an "AS IS" BASIS,\n')
    fh.write('* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n')
    fh.write('*/\n\n')



def create_hif(cmds,tag):
    filename = "Bt%sTaskPrivate.h" % (tag[0].upper() + tag[1:]) 
    fh = open(filename, 'w')
    create_file_note(fh)
    fh.write("#ifndef __%s_H__\n" % filename.upper()[:-2])
    fh.write("#define __%s_H__\n\n" % filename.upper()[:-2])
    
    for cmd in cmds:
        if (cmd['name'][-3:] == 'REQ') or (cmd['name'][-3:] == 'RES'):        	     
            if cmd['name'].split('_')[1].lower() == tag:
                name = get_taskname(cmd['name'])
                filename = '%s.h' % name[1]
                fh.write('#include "%s"\n' % filename)
        else:
            continue
    
    fh.write("\n#endif\n")    
    fh.close 

def create_header(cmd):
    name = get_taskname(cmd['name'])
    filename = '%s.h' % name[1]
    fh = open(filename, 'w')
    
    create_file_note(fh)
    
    fh.write("#ifndef __%s_H__\n" % (name[0]))
    fh.write("#define __%s_H__\n\n" % (name[0]))
    fh.write('#include "BtTaskPrivate.h"\n\n')
    fh.write("namespace goni {\n\n")
    fh.write("class %s;\n\n" % (name[1]))
    fh.write("typedef android::sp<%s> %sSp;\n" % (name[1], name[1]))
    fh.write("typedef android::wp<%s> %sWp;\n\n" % (name[1], name[1]))

    fh.write("class %s : public BtSingleTask\n{\npublic:\n" % (name[1]))
    fh.write("    typedef std::function<void(%sSp)> CB;\n\npublic:\n" % (name[1]))
    fh.write("    %s(" % (name[1]))
    for param in cmd['params']:
        fh.write("%s %s, " % (types[param['type']], param['name']))
    fh.write("CB cb = nullptr);\n")       
    fh.write("    ~%s();\n\n" % (name[1]))
    fh.write("    virtual void execute();\n")
    fh.write("    virtual bool onConfirm(uint16_t opcode, void* msg);\n")
    fh.write("    virtual bool onIndicate(uint16_t opcode, void* msg);\n")
    fh.write("    virtual void doCallback();\n\n")
    fh.write("    virtual BtTaskBase::TaskOptimization getOptimizeOption(BtTaskBaseSp task);\n\n")
    fh.write("    virtual android::String8 name() const;\n\nprivate:\n")

    fh.write("    CB        m_callback;\n")    
    for param in cmd['params']:
        type = "{:<10}".format(types[param['type']])
        fh.write("    %sm_%s;\n" % (type, param['name']))

    fh.write("};\n\n}\n")
    fh.write("#endif // __%s_H__\n" % (name[0]))
    
    fh.close()
 

def create_source(cmd):
    name = get_taskname(cmd['name'])
    filename = '%s.cpp' % name[1]
    fh = open(filename, 'w')
    
    create_file_note(fh)

    fh.write('#include "%s.h"\n\n' % (name[1]))
    fh.write("namespace goni {\n\n")
    fh.write("%s::%s(" % (name[1], name[1]))                               
    for param in cmd['params']:
        fh.write("%s %s, " % (types[param['type']], param['name']))
    fh.write("CB cb)\n")
    fh.write("    : BtSingleTask(%s)\n" % (cmd['name']))
    for param in cmd['params']:
        fh.write("    , m_%s(%s)\n" % (param['name'], param['name']))        
    fh.write("    , m_callback(cb)\n")      
    
    fh.write("{\n}\n\n")
    fh.write("%s::~%s()\n" % (name[1], name[1]))   
    fh.write("{\n}\n\n")
    fh.write("void %s::execute()\n" % (name[1]))
    fh.write("{\n    %s(" % (name[2]))
    flag = False
    for param in cmd['params']:
        if flag:
           fh.write(", ")
        flag = True
        fh.write("m_%s" % param['name'])
    fh.write(");\n    waitConfirm(true);\n}\n\n")
    
    fh.write("bool %s::onIndicate(uint16_t opcode, void *msg)\n" % (name[1]))
    fh.write("{\n    return false;\n}\n\n")
    
    fh.write("bool %s::onConfirm(uint16_t opcode, void *msg)\n" % (name[1]))
    fh.write("{\n    return false;\n}\n\n")

    fh.write("void %s::doCallback()\n" % (name[1]))
    fh.write("{\n    BtRelier replier(BtService::instance());\n\n    if (m_callback) {\n        m_callback(this);\n    }\n}\n\n")
    
    fh.write("BtTaskBase::TaskOptimization %s::getOptimizeOption(BtTaskBaseSp task)\n" % (name[1]))
    fh.write("{\n    return TaskOptimization_None;\n}\n\n\n")
    
    fh.write("android::String8 %s::name() const\n" % (name[1]))
    fh.write('{\n    return android::String8("%s");\n}\n\n' % name[1])

    fh.write("}")
    fh.close()



if __name__ == '__main__':
    file = sys.argv[1]
    if not os.path.exists(file):
        sys.exit(1)

    cmds = parse_xml.load_xml_file(file)
    for tag in names:
        create_hif(cmds,tag)
    
    for cmd in cmds:
        if (cmd['name'][-3:] != 'REQ') and (cmd['name'][-3:] != 'RES'):        	     
            continue
        create_header(cmd)
        create_source(cmd)
