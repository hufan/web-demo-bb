#!/bin/sh
# coding=utf-8   //设置文本格式  
import os,sys  
import json 

from collections import OrderedDict

def cmdline_read():
    str_param='rootfstype='
    len_str_check=len(str_param)
    f = open("/proc/cmdline")
    lines = f.readlines()
    #print lines[0]
    read_param=lines[0].split(' ')
    read_param=sr.split(' ')
    for i in range(0,len(read_param)):
        if read_param[i][0:len_str_check]=="rootfstype=":
            # print read_param[i][len_str_check:]
            rel=read_param[i][len_str_check:]
    # f.close()
    return rel

def get_new_json(filepath):  
    with open(filepath, 'rb') as f:  
        json_data = json.load(f,object_pairs_hook=OrderedDict)  
        json_data["board_info"]["system"]['memory']="512MB"
        json_data["board_info"]["system"]['storage']="4G"
    f.close()  
    return json_data  
      
def rewrite_json_file(filepath,json_data):  
    with open(filepath, 'w') as f:
        json.dump(json_data,f,ensure_ascii=False,indent=2)  
    f.close()  
  
if __name__ == '__main__':
    json_path = '/usr/share/myir/board_cfg.json'
    if 'ext4'==cmdline_read():
        m_json_data = get_new_json(json_path)   
        rewrite_json_file(json_path,m_json_data) 
    else:
        pass
