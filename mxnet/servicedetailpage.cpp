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
#include "servicedetailpage.h"
#include "systemcontentwidget.h"
#include "serviceitem.h"
#include "constant.h"
#include <QDebug>
ServiceDetailPage::ServiceDetailPage(arrayElement ael,QWidget *parent) : QWidget(parent)
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);

    m_width = parent->width();
    m_height = parent->height();

    label_details_right = new QLabel(this);
    label_details_right->setObjectName(QStringLiteral("label_details_right"));

    label_details_left = new QLabel(this);
    label_details_left->setObjectName(QStringLiteral("label_details_left"));

    services_item = ael;
    initUI();
}
void ServiceDetailPage::initUI()
{

    showServiceDetails();

    if( m_width < DEFAULT_SCREEN_WIDTH)
    {

        verticalLayout = new QVBoxLayout(this);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        verticalLayout->addWidget(label_details_right,0,Qt::AlignTop);
        verticalLayout->addWidget(label_details_left,0,Qt::AlignTop);

    }
    else
    {
        horizontalLayout = new QHBoxLayout(this);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        horizontalLayout->addWidget(label_details_right,0,Qt::AlignTop);
        horizontalLayout->addWidget(label_details_left,0,Qt::AlignTop);

    }
    label_details_left->raise();
    label_details_right->raise();


}

void ServiceDetailPage::updateData(arrayElement ael)
{
    services_item = ael;
    initUI();
}

void ServiceDetailPage::showServiceDetails()
{
    //  Make sure we were sent a valid index, can happen if the comboBox is
    //  cleared and for whatever reason could not be reseeded with entries.


    //  Get the QMap associated with the index stored in an arrayElement
    QMap<QString,QVariant> map = services_item.objmap;

    //  Some of the QVariants in the map are QMaps themselves, create a data structure for them
    QMap<QString,QVariant> submap;


    QString rs =tr("IPv4<br>");
    shared::extractMapData(submap, services_item.objmap.value("IPv4") );
    rs.append(tr("IP Address Acquisition: %1<br>").arg(submap.value("Method").toString()) );
    if(submap.value("Method").toString() == "dhcp")
    {

        rs.append(tr("IP Address: %1<br>").arg(submap.value("Address").toString()));
        rs.append(tr("IP Netmask: %1<br>").arg(submap.value("Netmask").toString()));
        rs.append(tr("IP Gateway: %1<br>").arg(submap.value("Gateway").toString()));
    }
    if(submap.value("Method").toString() == "manual")
    {

        shared::extractMapData(submap, services_item.objmap.value("IPv4.Configuration") );
        rs.append(tr("IP Address: %1<br>").arg(submap.value("Address").toString()));
        rs.append(tr("IP Netmask: %1<br>").arg(submap.value("Netmask").toString()));
        rs.append(tr("IP Gateway: %1<br>").arg(submap.value("Gateway").toString()));
    }

    //  write the text to the right display label
    label_details_left->setText(rs);


    rs = tr("Ethernet<br>");
    shared::extractMapData(submap, services_item.objmap.value("Ethernet") );
    rs.append(tr("Connection Method: %1<br>").arg(submap.value("Method").toString()));
    rs.append(tr("Interface: %1<br>").arg(submap.value("Interface").toString()) );
    rs.append(tr("Device Address: %1<br>").arg(submap.value("Address").toString()) );
    rs.append(tr("MTU: %1<br>").arg(submap.value("MTU").value<quint16>()) );


    //  write the text to the left display label

    label_details_right->setText(rs);


    return;
}
