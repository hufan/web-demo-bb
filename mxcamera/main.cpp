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
#include "frminput.h"

#include <QApplication>
#include <QSettings>
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
//    if(argc == 7){
//        qDebug("%s %s %s %s %s %s %s ", argv[0], argv[1], argv[2],argv[3],argv[4]),argv[5],argv[6];
//    }

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
    MxApplication app(argc, argv);
    if(app.isRunning()){
        return 0;
    }


//    MxDE        *mxde = new MxDE(&app);

    QTranslator translator;

// load qss file

    QFile qss(":/res/qss/mxde.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    QSettings           *m_Settings = NULL;
    QString             m_current_language="en";

    m_Settings = new QSettings(QString(MXDE_SETTING_FILE_PATH), QSettings::IniFormat);

    m_Settings->beginGroup("LANGUAGE");
    m_current_language = m_Settings->value("language").toString();
    if(m_current_language.isEmpty()){
        m_current_language = "en";
    }
    m_Settings->endGroup();
    m_Settings->sync();

    if(m_current_language == "zh_CN"){
        translator.load(QString(":/res/translation/mxapp_zh_CN.qm"));
    }
    else
    {
        translator.load(QString(":/res/translation/mxapp_en.qm"));
    }

    app.installTranslator(&translator);

    if((windowWidth < 320) ||(windowHeight < 240)) {
        windowWidth = QApplication::desktop()->screenGeometry(0).width();
        windowHeight = QApplication::desktop()->screenGeometry(0).height();
    }

    qDebug("=MXLED:== w= %d h=%d\n",windowWidth, windowHeight);
    frmInput::Instance()->Init("bottom", "silvery", 12, 10);
    MxMainDialog w(&app, 0, windowWidth, windowHeight);
    w.setApplication(&app);
//    w.setDbusProxy(mxde);


    w.display();
    w.raise();
    w.activateWindow();
    app.setMainWindow(&w);

    return app.exec();
}
