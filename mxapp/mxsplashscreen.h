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
#ifndef MXSPLASHSCREEN_H
#define MXSPLASHSCREEN_H

#include <QObject>
#include <QWidget>
#include <QSplashScreen>
#include <QApplication>
#include <QProgressBar>
#include <QPushButton>

class MxSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
   explicit MxSplashScreen(QApplication *app, QWidget *parent = 0);
   QProgressBar *progressbar;
   QPushButton *m_btChinese;
   QPushButton *m_btEnglish;
   QApplication *app;
   void display();

public slots:
// method 2 to display progressbar in splashscreen
   void setProgress(){
   m_progress += 2;
   if(m_progress > 100){
        m_progress = 100;
    }
   if(m_progress < 0 ){
        m_progress = 0;
    }
   update();

   this->progressbar->setValue(m_progress);
   this->progressbar->setTextVisible(false);
   this->app->processEvents();
   }

   void onChineseClicked();
   void onEnglishClicked();

protected:
   void drawContents(QPainter *painter);
   void closeEvent(QCloseEvent *event);
   void resizeEvent(QResizeEvent *event);

private:
   int m_progress;

};

#endif // MXSPLASHSCREEN_H
