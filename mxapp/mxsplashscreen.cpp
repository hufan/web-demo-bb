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
#include "mxsplashscreen.h"
#include "mxmaindialog.h"

#include <QPixmap>
#include <QRect>
#include <QFile>
#include <QDesktopWidget>
#include <QTimer>
#include <QProgressBar>
#include <QTranslator>
#include <QStyleOptionProgressBarV2>
#include <QDebug>

MxSplashScreen::MxSplashScreen(QApplication *app, QWidget *parent):
    QSplashScreen(parent),app(app)
{
    this->setPixmap(QPixmap(""));
    this->setCursor((Qt::BusyCursor));
    this->showMessage("hello", Qt::AlignBottom);
    m_progress = 0;
    QTimer *timer = new QTimer(app);
//
//    progressbar = new QProgressBar(this);
//    progressbar->setGeometry(0,400,380,20);
//    progressbar->setAlignment(Qt::AlignCenter);
//    progressbar->setValue(0);
//    progressbar->setMaximum(100);
//    progressbar->setEnabled(true);

    m_btChinese = new QPushButton(this);
    m_btEnglish = new QPushButton(this);

    connect(m_btChinese, SIGNAL(clicked()), this, SLOT(onChineseClicked()));
    connect(m_btEnglish, SIGNAL(clicked()), this, SLOT(onEnglishClicked()));

//    connect( timer, SIGNAL(timeout()), this, SLOT(setProgress()) );
//           timer->start(25); // 2秒单触发定时器
}

void MxSplashScreen::drawContents(QPainter *painter)
{
    QSplashScreen::drawContents(painter);
 //    this->progressbar->render(painter);
/*
    // Method 1 to display progressbar in splashscreen
    QStyleOptionProgressBarV2 pbstyle;
    pbstyle.initFrom(this);
    pbstyle.state = QStyle::State_Enabled;
    pbstyle.textVisible = false;

    pbstyle.minimum = 0;
    pbstyle.maximum = 100;
    pbstyle.progress = m_progress;
    pbstyle.invertedAppearance = false;
    pbstyle.rect = QRect(0,265,380,19);

    style()->drawControl(QStyle::CE_ProgressBar,&pbstyle, painter, this);
*/
}

void MxSplashScreen::display()
{
//    QPixmap *bgSplash = new QPixmap;
//    bgSplash->load(":/res/images/myir/mx_de_main_background.jpg");
    this->showMessage(QObject::tr("你好"),Qt::AlignCenter);
    QDesktopWidget *desk = QApplication::desktop();
    QRect deskRect = desk->availableGeometry();

     this->setGeometry(deskRect);
//   this->setFixedSize(480,272);
//   this->setFixedSize(800,480);
    this->move((deskRect.width()-this->width() )/2, (deskRect.height()-this->height())/2);
    this->show();
}

void MxSplashScreen::closeEvent(QCloseEvent *event)
{

}

void MxSplashScreen::resizeEvent(QResizeEvent *event)
{
        QPixmap pixmap = QPixmap(":/res/images/myir/mx_de_main_background.jpg").scaled(this->size());
        QPalette palette(this->palette());
        palette.setBrush(this->backgroundRole(),
                QBrush(pixmap.scaled(this->size(),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation)));
        this->setPalette(palette);

        QRect splashRect = this->geometry();
        int buttonWidth = splashRect.width()/10;
        if(buttonWidth<90){
            buttonWidth = 90;
        }
        int buttonHight = splashRect.height()/12;
        if(buttonHight<30)
        {
            buttonHight = 30;
        }
        int buttonBottom = splashRect.height()*4/5;
        int buttonChineseLeft = splashRect.width()/2-2*buttonWidth;
        int buttonEnglishLeft = splashRect.width()/2+buttonWidth;

        qDebug()<<buttonWidth<<buttonHight<<buttonBottom<<buttonChineseLeft<<endl;
        qDebug()<<buttonEnglishLeft<<endl;

        m_btChinese->setText(tr("简体中文"));
        m_btChinese->setGeometry(buttonChineseLeft,buttonBottom,buttonWidth,buttonHight);
        m_btEnglish->setText(tr("English"));
        m_btEnglish->setGeometry(buttonEnglishLeft,buttonBottom,buttonWidth,buttonHight);
}

void MxSplashScreen::onChineseClicked()
{

    QTranslator translator;
    translator.load(":/translations/i18n_zh.qm");
    qApp->installTranslator(&translator);

    MxMainDialog *window = new MxMainDialog(app,this);
//    window->setPalette(colorForLanguage(checkBox->text()));

//    window->installEventFilter(this);
//    mainWindowForCheckBoxMap.insert(checkBox, window);
    close();
    window->display();
}

void MxSplashScreen::onEnglishClicked()
{
    QTranslator translator;
    translator.load(":/translations/i18n_en.qm");
    qApp->installTranslator(&translator);

    MxMainDialog *window = new MxMainDialog(app,this);
//    window->setPalette(colorForLanguage(checkBox->text()));

//    window->installEventFilter(this);
//    mainWindowForCheckBoxMap.insert(checkBox, window);
    close();
    window->display();
}
