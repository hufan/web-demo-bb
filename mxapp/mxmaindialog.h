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
#ifndef MXMAINDIALOG_H
#define MXMAINDIALOG_H

#include "constant.h"
#include "mxapplication.h"
#include "shadowwidget.h"
#include "homeactionwidget.h"
#include "homecontentwidget.h"
#include "boxactionwidget.h"
#include "boxcontentwidget.h"

#include <QSettings>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QDialog>
#include <QApplication>
#include <QStackedWidget>
#include <QParallelAnimationGroup>

class MxMainDialog : public QDialog
{
    Q_OBJECT

public:
    MxMainDialog(MxApplication *app = 0, QWidget *parent = 0, int w = DEFAULT_SCREEN_HEIGHT, int h = DEFAULT_SCREEN_WIDTH);
    ~MxMainDialog();


    void setApplication(MxApplication *app);
    void display();
    void setTranslator(QTranslator *tr);
    void initConnect();
    void initHomePage();
    void initOtherPage();
    void initAnimation();

    int                 m_width;
    int                 m_height;
    int                 m_default_action_width;
    int                 m_default_content_width;
    int                 m_other_action_width;
    int                 m_other_content_width;
    int                 m_default_action_height;
    int                 m_default_content_height;
    int                 m_other_action_height;
    int                 m_other_content_height;
public slots:

    void OnApplicationClosed();
    void OnCloseBox();
    void OnCurrentPageChanged(int index);
    void OnSystemInfoClicked();
    void OnLanguageChanged(QString language);
    void OnOpenAnimFinished();
    void onCloseAnimFinished();
    void OnDemoStarted();
    void OnDemoFinished();

protected:
    void paintEvent(QPaintEvent *event);

private:
    MxApplication        *m_app;
    QSettings           *m_Settings;
    QTranslator         *m_translator;

    QPixmap             *main_skin_pixmap;
    ShadowWidget        *shadow_widget;
    QParallelAnimationGroup *spreadGroup;
    QParallelAnimationGroup *gatherGroup;

    QString             m_current_language;
    PAGESTATUS          m_status;
    bool                m_statusFlag;

    QStackedWidget      *topStack;
    QStackedWidget      *bottomStack;
    BaseWidget          *default_action_widget;
    BaseWidget          *other_action_widget;
    BaseWidget          *default_content_widget;
    BaseWidget          *other_content_widget;
    HomeActionWidget    *home_action_widget;
    HomeContentWidget   *home_content_widget;
    BoxActionWidget     *box_action_widget;
    BoxContentWidget    *box_content_widget;

};

#endif // MXMAINDIALOG_H
