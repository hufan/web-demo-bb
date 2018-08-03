#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QtDebug>

#include <QSqlError>
#include <QSqlQuery>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QFile>
#include <QtPlugin>

//#include "sqlite3.h"

#include <QDialog>
#include "QDirModel"
#include <QFileSystemModel>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QButtonGroup>
#include <QVector>
#include <QStackedWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>
//Q_IMPORT_PLUGIN(qsqlite)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    setWindowTitle(tr("StackedWidget"));    //设置窗体的名字
    ui->pushButton->setText("LED");
    ui->pushButton_2->setText("SQLITE3");

    /* ui  */
    m_dataview = new dataview();
    stack = new QStackedWidget(this);
    stack->addWidget(m_dataview);
    stack->setGeometry( QRect(190,-40,600,521));

    //stack->setCurrentWidget(m_dataview);


#if 0
    /*eum*/
    //创建连接
   //  QSqlDatabase database;
//   QSqlDatabase database = QSqlDatabase::addDatabase( "QSQLITE" );//第二个参数可以设置连接名字，这里为default

  // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

 //  database.setDatabaseName( "/home/root/testdatabase.db" );// 设置数据库名与路径, 此时是放在上一个目录

//   if(database.open())
//       {
//           qDebug()<<"Database Opened";

//           QSqlQuery sql_query;
//           QString create_sql = "create table member (id int primary key, name varchar(30), address varchar(30))"; //创建数据表
//           QString insert_sql = "insert into member values(?,?,?)";    //插入数据

//           QString select_all_sql = "select * from member";

//           sql_query.prepare(create_sql); //创建表
//           if(!sql_query.exec()) //查看创建表是否成功
//           {
//               qDebug()<<QObject::tr("Table Create failed");
//               qDebug()<<sql_query.lastError();
//           }
//           else
//           {
//               qDebug()<< "Table Created" ;

//               //插入数据
//               sql_query.prepare(insert_sql);

//               QVariantList GroupIDs;
//               GroupIDs.append(0);
//               GroupIDs.append(1);
//               GroupIDs.append(2);

//               QVariantList GroupNames;
//               GroupNames.append("hsp");
//               GroupNames.append("rl");
//               GroupNames.append("spl");

//               QVariantList GroupAddress;
//               GroupAddress.append("南充");
//               GroupAddress.append("宝鸡");
//               GroupAddress.append("南充");

//               sql_query.addBindValue(GroupIDs);
//               sql_query.addBindValue(GroupNames);
//               sql_query.addBindValue(GroupAddress);

//               if(!sql_query.execBatch())
//               {
//                   qDebug()<<sql_query.lastError();
//               }
//               else
//               {
//                   qDebug()<<"插入记录成功";
//               }

//               //查询所有记录
//               sql_query.prepare(select_all_sql);
//               if(!sql_query.exec())
//               {
//                   qDebug()<<sql_query.lastError();
//               }
//               else
//               {
//                   while(sql_query.next())
//                   {
//                       int id = sql_query.value(0).toInt();
//                       QString name = sql_query.value(1).toString();
//                       QString address = sql_query.value(2).toString();
//                       qDebug()<<QString("ID:%1  Name:%2  Address:%3").arg(id).arg(name).arg(address);
//                   }
//               }
//           }
//       }
//       database.close();
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}
