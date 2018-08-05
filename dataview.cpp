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
    ui->tableView->setModel(model);

	char **arr_sqldata;
    sql_test=new sqlite3_base();
    sql_test->sqlite3_base_open("RHMI.db");
    sql_test->sqlite3_base_create_tab();
    sql_test->sqlite3_base_insert_data();
    arr_sqldata=sql_test->sqlite3_base_read_data();

//     qDebug()<<"nrow-"<<sql_test->sqlread_nrow<<"ncolumn-"<<sql_test->sqlread_ncolumn<<endl;
//    if((sql_test->sqlread_nrow>0)&& (sql_test->sqlread_ncolumn>0)){
//        for(int i=0;i<(sql_test->sqlread_nrow+1)*sql_test->sqlread_ncolumn;i++){
//            qDebug()<<"arr i  ---"<<i<<"data"<<arr_sqldata[i]<<endl;
//        }
//    }

    sql_test->sqlite3_base_close();

/*第1种方式显示*/
    /*disaple*/
    model->setColumnCount(sql_test->sqlread_ncolumn);
    // 显示标题
    for(int i=0;i<sql_test->sqlread_ncolumn;i++){
        model->setHeaderData(i,Qt::Horizontal,arr_sqldata[i]);
    }
    // read sql data
    for(int i=0;i<sql_test->sqlread_nrow;i++){
        for(int j=0;j<sql_test->sqlread_ncolumn;j++){
            model->setItem(i,j,new QStandardItem(arr_sqldata[sql_test->sqlread_ncolumn*i+j+sql_test->sqlread_ncolumn]));
//            qDebug()<<"hufan i  ---"<<i<<"data"<<arr_sqldata[sql_test->sqlread_ncolumn*i+j+sql_test->sqlread_ncolumn]<<endl;
        }
    }

/*第二种方式显示*/
//    QStandardItem* item0 = new QStandardItem(tr("小明"));
//    QStandardItem* item1 = new QStandardItem(tr("男"));
//    QStandardItem* item2 = new QStandardItem(tr("20"));
//    QStandardItem* item3 = new QStandardItem(tr("180"));
//    QList<QStandardItem*> items;
//    items << item1 << item2 << item3;
//    model->appendRow(items);


}

dataview::~dataview()
{
    delete ui;
}
