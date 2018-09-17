#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QTimerEvent>
#include <QLabel>
#include <QTime>

//#include "serialport.h"
//#include "thread.h"
#include "mxde.h"
#define PORTFILENAME "serialport.dat"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showInRecvText(QString str);
    void SerialPortInitSlots();
    void autoScroll();
    void reLoadSerialPort();

    void on_openPushButton_clicked();
    void on_clearRecvPushButton_clicked();
    void on_sendPushButton1_clicked();

    void timeOutEvent();
    void systemTimeUpdate();

    //void on_autoSendCheckBox_clicked();

    //void onSerialRecvData(int uart_fd, const QString &data, int size);
      void onCanRecvData(int can_fd,int can_id,int can_dlc,const QString &data);

private:
    Ui::MainWindow *ui;

    bool openFlag;
    bool timerFlag;
    bool autosendFlag;

    QTimer *timer;
    QTimer *systemTimer;
    QLabel *label;
    QLabel *systemLabel;
    QLabel *sendLable;
    QLabel *recvLable;
    double recvNum;
    double sendNum;
    int m_can_num;
    int m_can_fd;
    QString m_can_name;
    char *m_can_list[5];
    MxDE                *m_mxde;
private:
    void SerialPortInit();


signals:
    void clickSerialPortCombobox();
};

#endif // MAINWINDOW_H
