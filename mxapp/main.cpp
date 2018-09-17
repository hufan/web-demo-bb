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
#include <getopt.h>
#include <unistd.h>

#include "mxmaindialog.h"
#include "mxapplication.h"
//#include "mxsplashscreen.h"

#include <QFile>
#include <QTranslator>
#include <QDesktopWidget>

static const char short_options[] = "w:h";
static const struct option long_options[]={
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"platform",required_argument,NULL,'p'},
    {0,0,0,0}
};

int main(int argc, char *argv[])
{
    static int windowWidth = 0;
    static int windowHeight = 0;

    for(;;){
        int opt_nxt;
        opt_nxt = getopt_long(argc, argv, short_options, long_options, NULL);
        if(opt_nxt < 0){
            break;
        }

        switch(opt_nxt){
            case 0:
                break;
            case 'w':
                qDebug("==%s",optarg);
                windowWidth = atoi(optarg);
                break;
            case 'h':
                qDebug("==%s",optarg);
                windowHeight = atoi(optarg);
                break;
            default:
                break;

        }
    }
//    QApplication a(argc, argv);
    MxApplication app(argc, argv);
    if(app.isRunning()){
        return 0;
    }

//    app.setQuitOnLastWindowClosed(false);

    QTranslator translator;

// load qss file

    QFile qss(":/res/qss/mxde.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();


//    MxSplashScreen *splash = new MxSplashScreen(&app);
//    splash->display();
    if((windowWidth < 320) ||(windowHeight < 240)) {
        windowWidth = QApplication::desktop()->screenGeometry(0).width();
        windowHeight = QApplication::desktop()->screenGeometry(0).height();
    }

    qDebug("=== w= %d h=%d\n",windowWidth, windowHeight);
    MxMainDialog w(&app, 0, windowWidth, windowHeight);

    w.setTranslator(&translator);
//    w.setDbusProxy(mxde);

    w.display();
    app.setMainWindow(&w);

    return app.exec();
}
