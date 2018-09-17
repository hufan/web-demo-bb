
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
#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H
#include "basewidget.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QList>
#include <QTextEdit>
#include <QTableWidget>
#include <QGridLayout>
#include "mxde.h"
#include "mxledindicator.h"
#include "mxapplication.h"


class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();
    void loadLeds();

signals:
    void click_Button(int index);

public slots:
    void on_click_Button(int index);
    void switchSelectedLedButtoIndex(QString str);
    void onLedBrightnessChanged(const QString &message);
private:
    void createHorizontalGroupBox();
    void getLedNameAndStatus();
    void getBoardLedInfo();

    QTableWidget *m_tableWidget;
    QGridLayout *mainLayout;
    QGroupBox          *horizontalGroupBox;
    QTextEdit             *bigEditor;
    QLabel              *ledLable;
    QList<MxLedIndicator *> leds;
    QStringList          m_ledList;
    int                  m_ledNum;
    QWidget             *m_parent;
    MxApplication        *m_app;
    int                      m_width;
    int                      m_height;
    QStringList ledlocationList;
};

#endif // SYSTEMCONTENTWIDGET_H
