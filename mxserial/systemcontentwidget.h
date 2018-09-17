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
#include <QComboBox>

#include "mxde.h"
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
    void setCurrentConfigure(QStringList list);
    void initUI();
    void initConnection();
    void display();
    void closeDevice();

signals:
    void clickSerialPortCombobox();

public slots:
    void showInRecvText(QString str);
    void SerialPortInitSlots();
    void autoScroll();
    void reLoadSerialPort();

    void on_openPushButton_clicked();
    void on_clearPushButton_clicked();
    void on_sendPushButton_clicked();

    void timeOutEvent();
    void systemTimeUpdate();

    void onSerialRecvData(int uart_fd, const QString &data, int size);


private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    bool openFlag;
    bool timerFlag;

    QTimer *timer;
    QTimer *systemTimer;
    QLabel *label;
    QLabel *systemLabel;
    QLabel *sendLabel;
    QLabel *recvLabel;
    double  recvNum;
    double  sendNum;

    int  m_serial_num;
    int  m_serial_fd;

    char *m_serial_name[5];

    void SerialPortInit();

    QGridLayout     *mainLayout;
    QGroupBox       *m_SettingGroup;
    QGridLayout     *m_SettingLayout;
    QLabel          *mSerialPort;
    QComboBox       *mSerialPortComboBox;
    QLabel          *mSerialBaudRate;
    QComboBox       *mSerialBaudRateComboBox;
    QLabel          *mSerialCheckBit;
    QComboBox       *mSerialCheckBitComboBox;
    QLabel          *mSerialDataBit;
    QComboBox       *mSerialDataBitComboBox;
    QLabel          *mSerialStopBit;
    QComboBox       *mSerialStopBitComboBox;
    QPushButton     *mSerialOpenButton;

    QGroupBox       *m_SendGroup;
    QGridLayout     *m_SendLayout;
    QTextEdit       *m_SendTextEdit1;
    QPushButton     *m_SendPushButton;

    QGroupBox       *m_RecvGroup;
    QGridLayout     *m_RecvLayout;
    QTextEdit       *m_RecvTextEdit1;
    QPushButton     *m_ClearPushButton;

    void            createSettingGroupBox();
    void            createSendGroupBox();
    void            createRecvGroupBox();

};

#endif // SYSTEMCONTENTWIDGET_H
