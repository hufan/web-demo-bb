#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <errno.h>
#include <QString>
#include "mxde.h"


#define CAN_ENABLE				1
#define CAN_DISABLE				0
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openFlag = false;
    timerFlag = false;
    autosendFlag = false;
    recvNum = 0;
    sendNum = 0;


    timer = new QTimer;
    label = new QLabel;
    systemLabel = new QLabel;
    systemTimer = new QTimer;
    sendLable = new QLabel;
    recvLable = new QLabel;

    m_mxde = new MxDE(this);
    systemTimer->setInterval(1000);

    //QObject::connect(mxde_session_iface,SIGNAL(sigSerialRecv(int,const QString,int)),this,SLOT(serial_recv_data(int,const QString,int)));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeOutEvent()));
    QObject::connect(systemTimer, SIGNAL(timeout()), this, SLOT(systemTimeUpdate()));
    QObject::connect(ui->recvTextEdit, SIGNAL(textChanged()), this, SLOT(autoScroll()));
    QObject::connect(ui->rateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(reLoadSerialPort()));



    SerialPortInit();

    label->setText(QString::fromUtf8("     RS485状态：　关闭"));
    ui->statusBar->addWidget(label);

    recvLable->setText(QString::fromUtf8("\t接收：") + QString::number(recvNum));
    ui->statusBar->addWidget(recvLable);

    sendLable->setText(QString::fromUtf8("\t发送：") + QString::number(sendNum));
    ui->statusBar->addWidget(sendLable);

    systemLabel->setText(QString::fromUtf8("\t当前时间：") + QTime::currentTime().toString());
    ui->statusBar->addWidget(systemLabel);

    systemTimer->start();

}

MainWindow::~MainWindow()
{

    systemTimer->stop();
    delete ui;

    delete timer;
    delete label;
    delete systemLabel;
    delete systemTimer;
    delete recvLable;
    delete sendLable;
}

void MainWindow::systemTimeUpdate()
{
    recvLable->setText(QString::fromUtf8("\t接收：") + QString::number(recvNum));
    sendLable->setText(QString::fromUtf8("\t发送：") + QString::number(sendNum));
    systemLabel->setText(QString::fromUtf8("\t当前时间：") + QTime::currentTime().toString());
}

void MainWindow::reLoadSerialPort()
{
    if(openFlag)
    {
        on_openPushButton_clicked();
    }
}

// 接收区自动滚屏
void MainWindow::autoScroll()
{
    QTextCursor cursor =  ui->recvTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->recvTextEdit->setTextCursor(cursor);
}

// 显示收到的数据的槽
void MainWindow::showInRecvText(QString str)
{
    recvNum += str.length();
    if(0)
    {
        bool ok = true;
        QString result = QString::number(str.toInt(&ok, 10),16);
        if(ok)
        {
            ui->recvTextEdit->insertPlainText(result.toUtf8());
        }
        else
        {
            result = "";
            for(int i = 0; i < str.length(); ++i)
            {
                result += QString::number(str[i].toLatin1());
            }
            ui->recvTextEdit->insertPlainText(result.toUtf8());
        }
    }
    else
    {
        ui->recvTextEdit->insertPlainText(str.toUtf8());
    }
}

//  初始化槽，用于动态加载当前的串口到combobox
void MainWindow::SerialPortInitSlots()
{
    SerialPortInit();
}

//　初始化函数，获取当前的所有串口
void MainWindow::SerialPortInit()
{

    QString str = m_mxde->callgetCanList();

    QStringList list = str.split("\n");
    m_can_num = list.count()-1;
    qDebug() << "m_can_num: " << m_can_num ;


    for(int i=0;i<m_can_num;i++)
    {


       m_can_list[i] = strdup(list.at(i).toLocal8Bit().data());

       qDebug("can:%s",m_can_list[i]);
       ui->serialPortComboBox->addItem(QString(m_can_list[i]));
    }

}

// 打开串口，且开启线程读取串口
void MainWindow::on_openPushButton_clicked()
{
    if(!openFlag)
    {
        QString canPortStr = ui->serialPortComboBox->currentText().isEmpty() ? "" : ui->serialPortComboBox->currentText();
        QString rateStr = ui->rateComboBox->currentText().isEmpty() ? "" : ui->rateComboBox->currentText();
        QString loopStr = ui->loopComboBox->currentText().isEmpty() ? "" : ui->loopComboBox->currentText();
        m_can_name = canPortStr;

        qDebug() << "port: " << canPortStr;
        qDebug() << "rate: " << rateStr;
        qDebug() << "loopStr: " << loopStr;
        if(canPortStr == "")
        {
            QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("未找到CAN"));
            return;
        }


        int ret = m_mxde->callsetCanPort(canPortStr,rateStr.toInt(),CAN_ENABLE,loopStr);
        if(ret < 0)
        {
            QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("CAN setting error!"));
            return;
        }


        m_can_fd = m_mxde->callopenCanPort(canPortStr);
        qDebug() << "open ret: " << m_can_fd;
        if(-1 == m_can_fd)
        {
            perror("open error");
            qDebug() << "open error : " <<  errno;
            if(errno == 13)
            {
                QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("请使用root权限"));
            }
            else
            {
                QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("CAN异常"));
            }
            m_mxde->callcloseCanPort(canPortStr,m_can_fd);
        }
        else
        {

            ui->openPushButton->setText(QString::fromUtf8("关闭CAN"));
            openFlag = true;
            label->setText(QString::fromUtf8("     CAN状态：　打开"));
            sendNum = 0;
            recvNum = 0;

        }
    }
    else    // 关闭串口
    {
        m_mxde->callcloseCanPort(m_can_name,m_can_fd);
        m_can_fd = 0;
        ui->openPushButton->setText(QString::fromUtf8("打开CAN"));
        openFlag = false;
        label->setText(QString::fromUtf8("     CAN状态：　关闭"));

        autosendFlag = false;
        qDebug() << "closed ...";
    }
}

// 清空接收区
void MainWindow::on_clearRecvPushButton_clicked()
{
    ui->recvTextEdit->clear();
}

// 发送，清空发送区
void MainWindow::on_sendPushButton1_clicked()
{
    if(openFlag)
    {
        bool ok = true;

        QString sendStr;
        char buf[50] = {0};
        int n = 0;

        QString idStr = ui->idEdit->displayText();
        QByteArray id = idStr.toLatin1();

        QString dataStr = ui->dataEdit->displayText();
        QStringList list = dataStr.split(" ");
        int data_num = list.count();

        for(int i = 0; i < data_num; i++ )
        {
            n += sprintf(buf+n,"%s",strdup(list.at(i).toLocal8Bit().data()));
        }

        sendStr.sprintf("%s#%s",id.data(),buf);
        qDebug() << sendStr;
        m_mxde->callCanWrite(m_can_fd,sendStr,sendStr.length());
#if 0
        sendStr = ui->sendTextEdit1->toPlainText();

        if(ok)
        {
            sendNum += sendStr.length();
            m_mxde->callCanWrite(m_can_fd,sendStr,sendStr.length());

        }
        else
        {

            QString result;
            sendStr = ui->sendTextEdit1->toPlainText();
            for(int i = 0; i < sendStr.length(); ++i)
            {
                QString tmpStr = QString::number(sendStr[i].toLatin1(), 16);
                result += tmpStr;
            }
            sendNum += sendStr.length();
            m_mxde->callCanWrite(m_can_fd,sendStr,sendStr.length());

        }
        ui->sendTextEdit1->clear();
#endif
    }
    else
    {
        QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("请打开CAN"));
    }
}



void MainWindow::timeOutEvent()
{
    qDebug() << "time out ... ";
    on_sendPushButton1_clicked();
}


 void MainWindow::onCanRecvData(int can_fd, int can_id,int can_dlc,const QString &data)
 {
    qDebug() << "onCanRecvData ... ";
    recvNum += data.length();

    QByteArray tmp= data.toLatin1();
    char * t= tmp.data();
    //struct can_frame  *can_read_frame= (struct *)&t;

    //QString can_id;
    ui->recvTextEdit->insertPlainText("can_id: 0x"+QString::number(can_id, 16)+"\n");
    ui->recvTextEdit->insertPlainText("can_dlc: "+QString::number(can_dlc, 10)+"\n");
    ui->recvTextEdit->insertPlainText("can_data: "+data+"\n");

 }
