/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName:
* Author: Sunny.Guo
* Version: 1.0
* Date:
* Description:
*
* History:
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle
*
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#ifndef CONSTANT_H
#define CONSTANT_H
#include <QString>

typedef enum{
    CACHE,
    PACAKAGE,
    COOKIES,
    BROWSER,
    TRACE,
    BIGFILE,
} CLEANERTAB;

struct AutoData
{
    QString appPath;
    QString appName;
    QString appComment;
    QString iconName;
    QString appStatus;
};

typedef enum{
    BUTTON_ENTER,
    BUTTON_LEAVE,
    BUTTON_PRESSED,
    BUTTON_DISABLE,
    BUTTON_FIRST,
    BUTTON_SCALED,
}BUTTONSTATUS;

typedef enum{
    HOMEPAGE,
    BOXPAGE,
    SYSTEMPAGE,
    SETTINGPAGE,
    ABOUTPAGE,
}PAGESTATUS;

#define DEFAULT_SCREEN_WIDTH                    800
#define DEFAULT_SCREEN_HEIGHT                   480

#define DEFAULT_ACTION_HEIGHT(x)            ((x)*5/8)
#define DEFAULT_CONTENT_HEIGHT(x)            ((x)*3/8)

#define OTHER_ACTION_HEIGHT(x)              ((x)*1/8)
#define OTHER_CONTENT_HEIGHT(x)              ((x)*7/8)

#define DEFAULT_BOX_ICON_SIZE(x)             ((x)*1/7)
#define DEFAULT_BOX_ICON_PAD(x)              ((x)*3/80)
#define DEFAULT_BOX_ICON_GRID_SIZE(x)        ((x-20)/4)


#endif // CONSTANT_H
