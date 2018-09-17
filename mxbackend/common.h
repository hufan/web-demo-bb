/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName: common.h 
* Author: Sunny.Guo
* Version: 1.0
* Date: 2017年 01月 19日 星期四 10:12:50 CST
* Description: Common definition         
*
* History:        
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle  
*                                                                          
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/

#ifndef __COMMON_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

#define DEBUG 1

#ifdef DEBUG
#define dbg_printf(fmt, args...) printf(fmt, ##args)
#define dbg_perror(msg) (perror(msg))
#else
#define dbg_printf(fmt, args...)
#define dbg_perror(msg)
#endif

#define MIN(x,y)  (((x)<(y))?(x):(y))
#define MAX(x,y)  (((x)>(y))?(x):(y))

#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

#define MAX_INPUT 		255

#define MXDE_BOARD_CFG_PATH "/usr/share/myir/board_cfg.json"
#define MXDE_DBUS_XML "/usr/share/myir/mxde.xml"
int IsUtf8Format(const char *str);

void board_cfg_init();
void get_cfg_list(char *item_name,char * result);
#endif		// __COMMON_H__
