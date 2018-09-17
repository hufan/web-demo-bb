#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>

#include "mxapplication.h"
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

#include <QtSql>
#include <QtDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QtPlugin>
#include <QStyledItemDelegate>

#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>


#include "sqlite3_base.h"



class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
//    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
//    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();

    int init_sqlite();

    QTimer *testTimer;

    QStandardItemModel *model;
    sqlite3_base *sql_test;

signals:
    void clickSerialPortCombobox();

//public slots:
//    void timerDone();

private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    QStringList infoList;

    QPushButton          *m_insert_Button;
    QPushButton          *m_delete_Button;
    QPushButton          *m_update_Button;

    QGroupBox            *m_Grbox_Group;
    QGroupBox            *m_Grbox_Group2;
    QGridLayout          *m_Grbox_Layout;
    QGridLayout          *mainLayout;

    QTableView           *m_tableview;

};

#endif // SYSTEMCONTENTWIDGET_H
