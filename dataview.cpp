#include "dataview.h"
#include "ui_dataview.h"

#include <QButtonGroup>
#include <QPushButton>
#include <QDebug>

#include "sqlite3_base.h"


dataview::dataview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataview)
{
    ui->setupUi(this);

    QPushButton* button = new QPushButton(this);
    button->setGeometry(QRect(50,55,100,35));
    button->setText("查询");

    QPushButton* button1  = new QPushButton(this);
    button1->setGeometry(QRect(200,55,100,35));
    button1->setText("删除");

    // QPushButton* button_up  = new QPushButton(this);
    // button_up->setGeometry(QRect(340,470,100,35));
    // button_up->setText("上页");

    // QPushButton* button_down  = new QPushButton(this);
    // button_down->setGeometry(QRect(460,470,100,35));
    // button_down->setText("下页");

    model=new QStandardItemModel();

	char **arr_sqldata;
    sql_test=new sqlite3_base();
    sql_test->sqlite3_base_open("RHMI.db");
    sql_test->sqlite3_base_create_tab();
    sql_test->sqlite3_base_insert_data();
    // arr_sqldata=sql_test->sqlite3_base_read_data();
    // qDebug()<<"nrow-"<<sql_test->sqlread_nrow<<"ncolumn-"<<sql_test->sqlread_ncolumn<<endl;
    if((sql_test->sqlread_nrow>0)&& (sql_test->sqlread_ncolumn>0)){
	    qDebug()<<"arr data  ---"<<arr_sqldata[0]<<endl;
	    qDebug()<<"arr data  ---"<<arr_sqldata[1]<<endl;
    }

    sql_test->sqlite3_base_close();
    // delete arr_sqldata;
}

dataview::~dataview()
{
    delete ui;
}
