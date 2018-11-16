#include "systemcontentwidget.h"

#include <QDebug>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QFile>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
//#include <QTimer>
#include <iostream>

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>

#include "systemcontentwidget.h"
//#include "mxde.h"
#include "constant.h"
#include "mxapplication.h"
SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
{
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

    sql_test=new sqlite3_base();
    sql_test->sqlite3_base_open("/home/myir/MYIR_RHMI.db");
    sql_test->sqlite3_base_create_tab();

}
SystemContentWidget::~SystemContentWidget()
{
   // m_mxde->callCloseSerialPort(m_serial_fd);
}

int SystemContentWidget::init_sqlite()
{
    model=new QStandardItemModel();
    m_tableview->setModel(model);


}
void SystemContentWidget::initUI()
{
    mainLayout = new QGridLayout();

    m_Grbox_Group = new QGroupBox(tr("Sqlite3"));
    m_Grbox_Layout = new QGridLayout(m_Grbox_Group);

    m_insert_Button = new QPushButton();
    //m_insert_Button->setObjectName("sqlite3_insert");
    m_insert_Button->setObjectName("serialButton");
    m_insert_Button->setText(tr("Insert_sqlite3"));
//     m_insert_Button->setMinimumSize(10,10);

    m_delete_Button = new QPushButton();
    m_delete_Button->setObjectName("serialButton");
    m_delete_Button->setText(tr("Delete_sqlite3"));
//    m_delete_Button->setMinimumSize(10,10);

    m_update_Button = new QPushButton();
    m_update_Button->setObjectName("serialButton");
    m_update_Button->setText(tr("Update_sqlite3"));
//    m_update_Button->setMinimumSize(30,30);

    m_tableview = new QTableView();
    m_tableview->setObjectName("serialButton");
//    resizeRowsToContents();
    m_tableview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QHBoxLayout *hLayout1 = new QHBoxLayout(m_Grbox_Group);
    hLayout1->setContentsMargins(40,10,40,10);
//    hLayout1->setMargin(40);
    hLayout1->setSpacing(120);
//    hLayout1->addStretch();
    hLayout1->addWidget(m_insert_Button);
//    hLayout1->addStretch();
    hLayout1->addWidget(m_delete_Button);
//    hLayout1->addStretch();
    hLayout1->addWidget(m_update_Button);
//    hLayout1->addStretch();

    QHBoxLayout *hLayout2 = new QHBoxLayout(m_Grbox_Group);
    hLayout2->addWidget(m_tableview);

//     QGridLayout *grid = new QGridLayout();
    m_Grbox_Layout->addLayout(hLayout1,0,0,1,10);
    m_Grbox_Layout->addLayout(hLayout2,1,0,1,10);
    m_Grbox_Group->setLayout(m_Grbox_Layout);
    mainLayout->addWidget(m_Grbox_Group, 0,0);
    this->setLayout(mainLayout);

    connect(m_insert_Button, SIGNAL(clicked()), this, SLOT(clickInsertData_sqlite()));
    connect(m_delete_Button, SIGNAL(clicked()), this, SLOT(clickDeleteData_sqlite()));
    connect(m_update_Button, SIGNAL(clicked()), this, SLOT(clickupdateData_sqlite()));
    connect(m_tableview,SIGNAL(clicked(QModelIndex)),this,SLOT( get_id_sqlite() ));  /*tableview Select*/

/* init sqlite3 */
    init_sqlite();
    display();
}

void SystemContentWidget::initConnection()
{

}

void SystemContentWidget::display()
{
    char **arr_sqldata;

//    sql_test=new sqlite3_base();
//    sql_test->sqlite3_base_open("/home/myir/MYIR_RHMI.db");
//    sql_test->sqlite3_base_create_tab();
//    sql_test->sqlite3_base_insert_data();
    arr_sqldata=sql_test->sqlite3_base_read_data();

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
            }
        }

    /*第二种方式显示*/
    //    QStandardItem* item0 = new QStandardItem(tr("小明"));
    //    QStandardItem* item1 = new QStandardItem(tr("男"));
    //    QList<QStandardItem*> items;
    //    items << item1 << item2 << item3;
    //    model->appendRow(items);
}

int SystemContentWidget::clickInsertData_sqlite()
{
    sql_test->sqlite3_base_insert_data();
    display();
}
int SystemContentWidget::clickupdateData_sqlite()
{
    /*clear tableview*/
     model->clear();
     m_tableview->reset();
     display();
}
int SystemContentWidget::clickDeleteData_sqlite()
{
    sql_test->sqlite3_base_delete_data(get_id_sqlite());
    model->clear();
    m_tableview->reset();
    display();
}
int SystemContentWidget::get_id_sqlite()
{
    int curRow=m_tableview->currentIndex().row();//选中行
    QAbstractItemModel *modessl = m_tableview->model();
    QModelIndex indextemp = modessl->index(curRow,0);//遍历第一行的0列
    //这个是一个单元格的值。tostring()----ok
    QVariant datatemp = modessl->data(indextemp);
    return  datatemp.toInt();
}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}

//void SystemContentWidget::setDbusProxy(MxDE *mxde)
//{
//    m_mxde = mxde;
//}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

