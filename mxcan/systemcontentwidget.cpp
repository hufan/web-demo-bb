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
#include "systemcontentwidget.h"
#include "mxde.h"
#include "constant.h"
#include "mxapplication.h"

#include <QObject>
#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSignalMapper>
#include <QDirIterator>
#include <QStringList>
#include <QPalette>
#include <QLineEdit>

#define CAN_ENABLE      1
#define CAN_DISABLE     0

SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
{
    openFlag = false;

    m_width = width;
    if(m_width <=0){
        m_width = DEFAULT_SCREEN_WIDTH;
    }
    m_height = height;
    if(m_height <= 0){
        m_height = DEFAULT_SCREEN_HEIGHT;
    }

    this->setFixedSize(m_width, m_height);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);
}

SystemContentWidget::~SystemContentWidget()
{
    qDebug() << "SystemContentWidget ..." << m_can_fd;
    m_mxde->callCloseSerialPort(m_can_fd);

}

void SystemContentWidget::SerialPortInit()
{
    m_can_fd = 0;
    QString str = m_mxde->callgetCanList();
    if((str!=NULL) &&(str.length() < 2)){
        return;
    }

    QStringList list = str.split("\n");
    m_can_num = list.count()-1;
    qDebug() << "m_can_num: " << m_can_num ;


    for(int i=0;i<m_can_num;i++)
    {


       m_can_list[i] = strdup(list.at(i).toLocal8Bit().data());

       qDebug("can:%s",m_can_list[i]);
       mCanPortComboBox->addItem(QString(m_can_list[i]));
    }
}


void SystemContentWidget::createSettingGroupBox()
{
    m_SettingGroup = new QGroupBox(tr("Settings"));
    m_SettingLayout = new QGridLayout(m_SettingGroup);
    mCanPort = new QLabel(m_SettingGroup);
    mCanPort->setText(tr("CAN:"));
    mCanPort->setAlignment(Qt::AlignCenter);
    mCanPortComboBox = new QComboBox(m_SettingGroup);
    QHBoxLayout *hLayout1 = new QHBoxLayout(m_SettingGroup);
    hLayout1->addWidget(mCanPort);
    hLayout1->addWidget(mCanPortComboBox);

    mCanBaudRate = new QLabel(m_SettingGroup);
    mCanBaudRate->setText(tr("Baud:"));
    mCanBaudRate->setAlignment(Qt::AlignCenter);
    mCanBaudRateComboBox =  new QComboBox(m_SettingGroup);
    mCanBaudRateComboBox->insertItems(0,QStringList() \
          <<"20000"<<"50000"<<"125000"<< "250000" \
          <<"500000"<<"800000"<<"1000000") ;
    QHBoxLayout *hLayout2 = new QHBoxLayout(m_SettingGroup);
    hLayout2->addWidget(mCanBaudRate);
    hLayout2->addWidget(mCanBaudRateComboBox);

    mCanLoop = new QLabel(m_SettingGroup);
    mCanLoop->setText(tr("Loop:"));
    mCanLoop->setAlignment(Qt::AlignCenter);
    mCanLoopComboBox =  new QComboBox(m_SettingGroup);
    mCanLoopComboBox->insertItems(0,QStringList()<<"OFF"<<"ON");
    QHBoxLayout *hLayout3 = new QHBoxLayout(m_SettingGroup);
    hLayout3->addWidget(mCanLoop);
    hLayout3->addWidget(mCanLoopComboBox);

    QHBoxLayout *hLayout4 = new QHBoxLayout(m_SettingGroup);
    mCanOpenButton = new QPushButton;
    mCanOpenButton->setObjectName("canButton");
    mCanOpenButton->setText(tr("Open"));
    hLayout4->addWidget(mCanOpenButton);
    if(m_width < DEFAULT_SCREEN_WIDTH){
        m_SettingLayout->addLayout(hLayout1,0,0,1,1);
        m_SettingLayout->addLayout(hLayout2,1,0,1,1);
        m_SettingLayout->addLayout(hLayout3,2,0,1,1);
        m_SettingLayout->addLayout(hLayout4,3,0,1,1);

    }
    else
    {
    m_SettingLayout->addLayout(hLayout1,0,0,1,9);
    m_SettingLayout->addLayout(hLayout2,0,9,1,9);
    m_SettingLayout->addLayout(hLayout3,0,18,1,9);
    m_SettingLayout->addLayout(hLayout4,0,27,1,1);
    m_SettingLayout->setColumnMinimumWidth(27,80);
    }

    m_SettingGroup->setLayout(m_SettingLayout);
}

void SystemContentWidget::createSendGroupBox()
{
    m_SendGroup = new QGroupBox(tr("Send"));
    m_SendLayout = new QGridLayout(m_SendGroup);

    m_SendCanID = new QLabel(m_SendGroup);
    m_SendCanID->setText(tr("can id(hex):"));
    m_SendTextEdit0 = new QLineEdit(m_SendGroup);
    m_SendTextEdit0->setPlaceholderText("123");

    m_SendCanData = new QLabel(m_SendGroup);
    m_SendCanData->setText(tr("can data(hex):"));
    m_SendTextEdit1 = new QLineEdit(m_SendGroup);
    m_SendTextEdit1->setPlaceholderText("01 11 22 EF FF");
    m_SendPushButton = new QPushButton(m_SendGroup);
    m_SendPushButton->setObjectName("canButton");
    m_SendPushButton->setText(tr("Send"));

    QVBoxLayout *vLayout = new QVBoxLayout(m_SendGroup);
    vLayout->addWidget(m_SendPushButton);

    m_SendLayout->addWidget(m_SendCanID, 0,0,1,1);
    m_SendLayout->addWidget(m_SendTextEdit0, 0,1,1,98);
    m_SendLayout->addWidget(m_SendCanData, 1,0,1,1);
    m_SendLayout->addWidget(m_SendTextEdit1,1,1,1,98);

    m_SendLayout->addLayout(vLayout,1,100,1,1,Qt::AlignBottom);
    m_SendLayout->setColumnMinimumWidth(100,80);

    m_SendGroup->setLayout(m_SendLayout);
}

void SystemContentWidget::createRecvGroupBox()
{
    m_RecvGroup = new QGroupBox(tr("Received"));
    m_RecvLayout = new QGridLayout(m_RecvGroup);

    m_ClearPushButton = new QPushButton(m_RecvGroup);
    m_ClearPushButton->setObjectName("canButton");
    m_ClearPushButton->setText(tr("Clear"));
    m_RecvTextEdit1 = new QTextEdit(m_RecvGroup);
    m_RecvTextEdit1->setReadOnly(true);
    m_RecvTextEdit1->setFocusPolicy(Qt::NoFocus);
    QVBoxLayout *vLayout = new QVBoxLayout(m_RecvGroup);
    vLayout->addWidget(m_ClearPushButton);

    m_RecvLayout->addWidget(m_RecvTextEdit1,0,0,2,99);
    m_RecvLayout->addLayout(vLayout,1,100,1,1,Qt::AlignBottom);
    m_RecvLayout->setColumnMinimumWidth(100,80);
    m_RecvGroup->setLayout(m_RecvLayout);

}

void SystemContentWidget::initUI()
{
    createSettingGroupBox();
    createSendGroupBox();
    createRecvGroupBox();

    mainLayout = new QGridLayout;
    if(m_width < DEFAULT_SCREEN_WIDTH){
        m_SettingGroup->setObjectName("microGroupBox");
        m_SendGroup->setObjectName("microGroupBox");
        m_RecvGroup->setObjectName("microGroupBox");
        m_SendPushButton->setObjectName("microButton");
        m_ClearPushButton->setObjectName("microButton");
        mCanPort->setObjectName("microblackLabel");
        mCanBaudRate->setObjectName("microblackLabel");
        mCanLoop->setObjectName("microblackLabel");
        mCanPortComboBox->setObjectName("microComboBox");
        mCanBaudRateComboBox->setObjectName("microComboBox");
        mCanLoopComboBox->setObjectName("microComboBox");
        m_SendLayout->setColumnMinimumWidth(100,50);
        m_RecvLayout->setColumnMinimumWidth(100,50);
        m_SettingGroup->setMaximumWidth(150);
        mainLayout->addWidget(m_SettingGroup, 0,0,2,1);
        mainLayout->addWidget(m_SendGroup,0,1,1,4);
        mainLayout->addWidget(m_RecvGroup,1,1,1,4);
    }
    else
    {
        mainLayout->addWidget(m_SettingGroup, 0,0,1,1);
        mainLayout->addWidget(m_SendGroup,1,0,4,1);
        mainLayout->addWidget(m_RecvGroup, 5,0,4,1);
    }

    SerialPortInit();
    this->setLayout(mainLayout);
}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}

void SystemContentWidget::setDbusProxy(MxDE *mxde)
{
    m_mxde = mxde;
}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}


void SystemContentWidget::initConnection()
{

//    connect(timer, SIGNAL(timeout()), this, SLOT(timeOutEvent()));
//    connect(systemTimer, SIGNAL(timeout()), this, SLOT(systemTimeUpdate()));
    connect(m_RecvTextEdit1, SIGNAL(textChanged()), this, SLOT(autoScroll()));
    connect(mCanBaudRateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reLoadSerialPort()));
    connect(mCanPortComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reLoadSerialPort()));
    connect(mCanLoopComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reLoadSerialPort()));
    connect(m_SendPushButton,SIGNAL(clicked()),this, SLOT(on_sendPushButton_clicked()));
    connect(m_ClearPushButton,SIGNAL(clicked()),this, SLOT(on_clearPushButton_clicked()));
    connect(mCanOpenButton, SIGNAL(clicked()), this, SLOT(on_openPushButton_clicked()));
}

void SystemContentWidget::display()
{

}

void SystemContentWidget::showInRecvText(QString str)
{
    recvNum += str.length();
    if(0)
    {
        bool ok = true;
        QString result = QString::number(str.toInt(&ok, 10),16);
        if(ok)
        {
            m_RecvTextEdit1->insertPlainText(result.toUtf8());
        }
        else
        {
            result = "";
            for(int i = 0; i < str.length(); ++i)
            {
                result += QString::number(str[i].toLatin1());
            }
            m_RecvTextEdit1->insertPlainText(result.toUtf8());
        }
    }
    else
    {
        m_RecvTextEdit1->insertPlainText(str.toUtf8());
    }
}

void SystemContentWidget::SerialPortInitSlots()
{
    SerialPortInit();
}

void SystemContentWidget::autoScroll()
{
    QTextCursor cursor = m_RecvTextEdit1->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_RecvTextEdit1->setTextCursor(cursor);
}

void SystemContentWidget::reLoadSerialPort()
{
    if(openFlag)
    {
        on_openPushButton_clicked();
    }
}
void SystemContentWidget::setCurrentConfigure(QStringList list)
{
    QString rateStr = strdup(list.at(2).toLocal8Bit().data());
    QString loopStr = strdup(list.at(3).toLocal8Bit().data());
    qDebug() << "setCurrentConfigure";
    qDebug() << "rate: " << rateStr;
    qDebug() << "loop: " << loopStr;

    mCanBaudRateComboBox->setCurrentText(rateStr);
    mCanLoopComboBox->setCurrentText(loopStr);
    QMessageBox::information(this, tr("Warning"), tr("Can had opened!Now use the opened configuration!"));

}
void SystemContentWidget::on_openPushButton_clicked()
{
    if(!openFlag)
    {
        QString canPortStr = mCanPortComboBox->currentText().isEmpty() ? "" : mCanPortComboBox->currentText();
        QString rateStr = mCanBaudRateComboBox->currentText().isEmpty() ? "" : mCanBaudRateComboBox->currentText();
        QString loopStr = mCanLoopComboBox->currentText().isEmpty() ? "" : mCanLoopComboBox->currentText();

        m_can_name = canPortStr;

        qDebug() << "Can: " << canPortStr;
        qDebug() << "rate: " << rateStr;
        qDebug() << "loop: " << loopStr;

        if(canPortStr == "")
        {
            QMessageBox::information(this, tr("Warning"), tr("Do not find any CAN!"));
            return;
        }
        QString can_configure;
        int ret = m_mxde->callsetCanPort(canPortStr, rateStr.toInt(), CAN_ENABLE, loopStr,can_configure);
        if(ret == 100){
            QStringList list = can_configure.split(" ");
            m_can_fd = list.at(1).toInt();
            qDebug() << "callsetCanPort ret: " << can_configure;
            qDebug() << "open ret: " << m_can_fd;
            setCurrentConfigure(list);
            mCanOpenButton->setText(tr("Close"));
            openFlag = true;
        }
        else{

            m_can_fd = m_mxde->callopenCanPort(canPortStr);
            qDebug() << "open ret: " << m_can_fd;
            if(-1 == m_can_fd)
            {
                perror("open error");
                qDebug() << "open error errno: " <<  errno;
                if(errno == 13)
                {
                    QMessageBox::information(this, tr("Error"), tr("Do not have access permission!"));
                }
                else
                {
                    QMessageBox::information(this, tr("Error"), tr("Failed to open CAN port!"));
                }
                m_mxde->callcloseCanPort(canPortStr,m_can_fd);
            }
            else
            {

                mCanOpenButton->setText(tr("Close"));
                openFlag = true;
    //            label->setText(QString::fromUtf8("     串口状态：　打开"));
                sendNum = 0;
                recvNum = 0;

            }

        }

    }
    else    // 关闭串口
    {
        m_mxde->callcloseCanPort(m_can_name, m_can_fd);
        m_can_fd = 0;
        mCanOpenButton->setText(tr("Open"));
        openFlag = false;
       // label->setText(QString::fromUtf8("     串口状态：　关闭"));

        //autosendFlag = false;
        qDebug() << "closed ...";
    }
}
void SystemContentWidget::closeDevice()
{
    qDebug() << "closeDevice ..." << m_can_fd;
    m_mxde->callcloseCanPort(m_can_name,m_can_fd);
    m_can_fd = 0;
}
void SystemContentWidget::on_clearPushButton_clicked()
{
    m_RecvTextEdit1->clear();
}

void SystemContentWidget::on_sendPushButton_clicked()
{
    if(openFlag)
    {
        bool ok = true;
        QString sendStr;
        char buf[50] = {0};
        int n = 0;

        QString idStr = m_SendTextEdit0->displayText();
        QByteArray id = idStr.toLatin1();

        QString dataStr = m_SendTextEdit1->displayText();
        QStringList list = dataStr.split(" ");
        int data_num = list.count();

        for(int i=0; i<data_num; i++)
        {
            n+=sprintf(buf+n,"%s", strdup(list.at(i).toLocal8Bit().data()));
        }

        sendStr.sprintf("%s+%s", id.data(), buf);
        qDebug() << sendStr;

        m_mxde->callCanWrite(m_can_fd, sendStr, sendStr.length());

    }
    else
    {
        QMessageBox::information(this, tr("Warning"), tr("Please open CAN first!"));
    }
}

void SystemContentWidget::timeOutEvent()
{

}

void SystemContentWidget::systemTimeUpdate()
{

}

void SystemContentWidget::onCanRecvData(int can_fd, int can_id, int can_dlc, const QString &data)
{

    if(m_can_fd == can_fd)
    {
        qDebug() << "onCanRecvData ... ";
        recvNum += data.length();

        QByteArray tmp= data.toLatin1();
        char * t= tmp.data();
        //struct can_frame  *can_read_frame= (struct *)&t;

        //QString can_id;
        m_RecvTextEdit1->insertPlainText("can_id: 0x"+QString::number(can_id, 16)+"\n");
        m_RecvTextEdit1->insertPlainText("can_dlc: "+QString::number(can_dlc, 10)+"\n");
        m_RecvTextEdit1->insertPlainText("can_data: "+data+"\n");
    }
}
