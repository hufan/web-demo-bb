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
#include "servicetab.h"
#include "systemcontentwidget.h"
ServiceTab::ServiceTab(arrayElement ael,QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height());

    splitter = new QSplitter(this);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setHandleWidth(0);
    splitter->setFocusPolicy(Qt::NoFocus);

    contentsWidget = new QListWidget(splitter);
    contentsWidget->setFixedSize(120, parent->height());
    contentsWidget->setFocusPolicy(Qt::NoFocus);
    contentsWidget->setObjectName("infoList");

    contentsWidget->setIconSize(QSize(16, 16));//设置QListWidget中的单元项的图片大小
    contentsWidget->setResizeMode(QListView::Adjust);
    contentsWidget->setViewMode(QListView::ListMode);   //设置QListWidget的显示模式
    contentsWidget->setMovement(QListView::Static);//设置QListWidget中的单元项不可被拖动

    stackedWidget = new QStackedWidget(splitter);//stacked_widget will delete when InfoWidget delete
    stackedWidget->setFocusPolicy(Qt::NoFocus);
    stackedWidget->setAutoFillBackground(true);

    connect(contentsWidget,SIGNAL(currentRowChanged(int)),this,SLOT(setCurrentPage(int)));
    connect(parent,SIGNAL(servicesChange(arrayElement)),this,SLOT(updateContentsPage(arrayElement)));

    services_item = ael;
    initUI();

}
void ServiceTab::initUI()
{
    m_config_page = NULL;
    m_detail_page = NULL;
    m_test_page = NULL;

    m_config_page = new ServiceConfigPage(services_item,this);
    //m_detail_page = new ServiceDetailPage(services_item,this);
    m_test_page = new ServiceTestPage(services_item,this);


    //stackedWidget->addWidget(m_detail_page);
    stackedWidget->addWidget(m_config_page);
    stackedWidget->addWidget(m_test_page);
    stackedWidget->setCurrentIndex(0);

    type_list << tr("IP Info") << tr("Ping Test");
    for(int i = 0;i < type_list.length();i ++) {

        QListWidgetItem *item = new QListWidgetItem(type_list.at(i), contentsWidget);
        item->setSizeHint(QSize(120,36));

    }
    contentsWidget->setCurrentRow(0);

    splitter->addWidget(contentsWidget);
    splitter->addWidget(stackedWidget);
    for(int i = 0; i<splitter->count();i++)
    {
        QSplitterHandle *handle = splitter->handle(i);
        handle->setEnabled(false);
    }

    QVBoxLayout *center_layout = new QVBoxLayout();
    center_layout->addWidget(stackedWidget);
    center_layout->setSpacing(0);
    center_layout->setMargin(0);
    center_layout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *main_layout = new QHBoxLayout();;
    main_layout->addWidget(splitter);
    main_layout->setSpacing(0);
    main_layout->setMargin(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(main_layout);
}

void ServiceTab::setCurrentPage(int item) {


    if (item == 0) {
        qDebug() << "IP Info";
        stackedWidget->setCurrentWidget(m_config_page);

    }
    else if (item == 1) {
        qDebug() << "config page";
        stackedWidget->setCurrentWidget(m_test_page);

    }


}
void ServiceTab::updateContentsPage(arrayElement ael)
{
    if(ael.objpath.path() == services_item.objpath.path())
    {

        //m_detail_page->updateData(ael);
        m_config_page->updateData(ael);
    }

}
