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
    m_uiled = new ui_led();

    stack = new QStackedWidget(this);
    stack->addWidget(m_dataview);
    stack->addWidget(m_uiled);
    stack->setGeometry( QRect(190,-40,600,521));

    stack->setCurrentWidget(m_uiled);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  stack->setCurrentWidget(m_uiled);
}

void MainWindow::on_pushButton_2_clicked()
{
  stack->setCurrentWidget(m_dataview);
}
