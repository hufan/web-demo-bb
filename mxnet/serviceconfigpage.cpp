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
#include "serviceconfigpage.h"
#include "systemcontentwidget.h"
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QMessageBox>
ServiceConfigPage::ServiceConfigPage(arrayElement ael, QWidget *parent) : QWidget(parent)
{

    m_width = parent->width()-120;
    m_height = parent->height();

    this->setFixedSize(m_width, m_height);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(palette);

    services_item = ael;
    initUI();

}

void ServiceConfigPage::create_config_page()
{
    m_settingGroup = new QGroupBox(this);
    m_settingGroup->setTitle(tr("Setting"));
    m_settingGroup->setObjectName(QStringLiteral("groupBox"));
    m_settingGroup->setGeometry(QRect(0, 0, m_width, m_height/2-20));

    m_okButton = new QPushButton(m_settingGroup);
    m_okButton->setObjectName("netButton");
    m_okButton->setGeometry(QRect(530, 128, 80, 22));
    m_okButton->setText(tr("OK"));

    m_methodlayoutWidget = new QWidget(m_settingGroup);
    m_methodlayoutWidget->setObjectName(QStringLiteral("layoutWidget"));
    m_methodlayoutWidget->setGeometry(QRect(10, 30, 491, 31));

    m_methodgridLayout = new QGridLayout(m_methodlayoutWidget);
    m_methodgridLayout->setSpacing(6);
    m_methodgridLayout->setObjectName(QStringLiteral("gridLayout"));
    m_methodgridLayout->setContentsMargins(0, 0, 0, 0);

    m_methodLable = new QLabel(m_methodlayoutWidget);
    m_methodLable->setObjectName(QStringLiteral("label"));
    m_methodgridLayout->addWidget(m_methodLable, 0, 0, 1, 1);
    m_methodLable->setText(tr("Method"));

    m_methodComboBox = new QComboBox(m_methodlayoutWidget);
    m_methodComboBox->setObjectName(QStringLiteral("methodComBox"));
    m_methodgridLayout->addWidget(m_methodComboBox, 0, 5, 1, 1);


    m_infolayoutWidget = new QWidget(m_settingGroup);
    m_infolayoutWidget->setObjectName(QStringLiteral("layoutWidget1"));
    m_infolayoutWidget->setGeometry(QRect(10, 70, 491, 81));

    m_infoformLayout = new QFormLayout(m_infolayoutWidget);
    m_infoformLayout->setSpacing(6);
    m_infoformLayout->setContentsMargins(11, 11, 11, 11);
    m_infoformLayout->setObjectName(QStringLiteral("formLayout"));
    m_infoformLayout->setContentsMargins(0, 0, 0, 0);


    m_ipLable = new QLabel(m_infolayoutWidget);
    m_ipLable->setObjectName(QStringLiteral("label_3"));
    m_ipLable->setText(tr("IP Address"));
    m_infoformLayout->setWidget(0, QFormLayout::LabelRole, m_ipLable);

    m_ipLineEdit = new QLineEdit(m_infolayoutWidget);
    m_ipLineEdit->setObjectName(QStringLiteral("addressLineEdit"));
    m_ipLineEdit->setFocus(Qt::NoFocusReason);
    m_infoformLayout->setWidget(0, QFormLayout::FieldRole, m_ipLineEdit);


    m_netmaskLable = new QLabel(m_infolayoutWidget);
    m_netmaskLable->setObjectName(QStringLiteral("label_4"));
    m_netmaskLable->setText(tr("Netmask"));
    m_infoformLayout->setWidget(1, QFormLayout::LabelRole, m_netmaskLable);

    m_netmaskLineEdit = new QLineEdit(m_infolayoutWidget);
    m_netmaskLineEdit->setObjectName(QStringLiteral("netmaskLineEdit"));
    m_netmaskLineEdit->setFocus(Qt::NoFocusReason);
    m_infoformLayout->setWidget(1, QFormLayout::FieldRole, m_netmaskLineEdit);


    m_gatewayLable = new QLabel(m_infolayoutWidget);
    m_gatewayLable->setObjectName(QStringLiteral("label_5"));
    m_gatewayLable->setText(tr("Gateway"));
    m_infoformLayout->setWidget(2, QFormLayout::LabelRole, m_gatewayLable);


    m_gatewayLineEdit = new QLineEdit(m_infolayoutWidget);
    m_gatewayLineEdit->setObjectName(QStringLiteral("gatewayLineEdit"));
    m_gatewayLineEdit->setFocus(Qt::NoFocusReason);
    m_infoformLayout->setWidget(2, QFormLayout::FieldRole, m_gatewayLineEdit);

    connect(m_methodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ipv4Method(int)));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(updateConfiguration()));
}
void ServiceConfigPage::create_detail_page()
{
    info_group_box = new QGroupBox(this);
    info_group_box->setTitle(tr("Info"));
    info_group_box->setObjectName(QStringLiteral("groupBox"));
    info_group_box->setGeometry(QRect(0, m_height/2-20, m_width, m_height/2-20));

    label_details_right = new QLabel(info_group_box);
    label_details_right->setObjectName(QStringLiteral("label_details_right"));

    label_details_left = new QLabel(info_group_box);
    label_details_left->setObjectName(QStringLiteral("label_details_left"));

    showServiceDetails();

    horizontalLayout = new QHBoxLayout(info_group_box);
    horizontalLayout->setSpacing(1);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    horizontalLayout->addWidget(label_details_right,0,Qt::AlignTop);
    horizontalLayout->addWidget(label_details_left,0,Qt::AlignTop);
    label_details_left->raise();
    label_details_right->raise();

}
void ServiceConfigPage::initUI()
{
    create_config_page();
    create_detail_page();
#if 1
    objpath = services_item.objpath;
    objmap = services_item.objmap;
    shared::extractMapData(ipv4map, objmap.value("IPv4.Configuration") );

    m_methodComboBox->clear();
    sl_ipv4_method.clear();
    sl_ipv4_method << "DHCP" << "Manual" << "Off";
    sl_ipv4_method_minuscules << "dhcp" << "manual" << "off";
    m_methodComboBox->addItems(sl_ipv4_method);


    const QString s_ip4 = "(?:25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])";

    QRegularExpression rx4("\\s?|^" + s_ip4 + "(?:\\." + s_ip4 + "){3}" + "$");
    QRegularExpressionValidator* lev_4 = new QRegularExpressionValidator(rx4, this);

    m_ipLineEdit->setValidator(lev_4);
    m_netmaskLineEdit->setValidator(lev_4);
    m_gatewayLineEdit->setValidator(lev_4);


    if (! ipv4map.value("Method").toString().isEmpty() ) {
      m_methodComboBox->setCurrentIndex(sl_ipv4_method_minuscules.indexOf(QRegularExpression(ipv4map.value("Method").toString())) );

      if(ipv4map.value("Method").toString() == "dhcp")
      {
          m_ipLineEdit->hide();
          m_netmaskLineEdit->hide();
          m_gatewayLineEdit->hide();
      }
    }
#endif
}

void ServiceConfigPage::updateData(arrayElement ael)
{
    services_item = ael;
    //initUI();
#if 1
    objpath = services_item.objpath;
    objmap = services_item.objmap;
    shared::extractMapData(ipv4map, objmap.value("IPv4.Configuration") );

    if (! ipv4map.value("Method").toString().isEmpty() ) {
      m_methodComboBox->setCurrentIndex(sl_ipv4_method_minuscules.indexOf(QRegularExpression(ipv4map.value("Method").toString())) );

      if(ipv4map.value("Method").toString() == "dhcp")
      {
          m_ipLineEdit->hide();
          m_netmaskLineEdit->hide();
          m_gatewayLineEdit->hide();
      }
    }
#endif
    showServiceDetails();

}
void ServiceConfigPage::ipv4Method(int idx)
{

    if (idx == 1 ) {

        m_ipLineEdit->show();
        m_netmaskLineEdit->show();
        m_gatewayLineEdit->show();
    }
    else {

        m_ipLineEdit->hide();
        m_netmaskLineEdit->hide();
        m_gatewayLineEdit->hide();
    }

    return;
}
void ServiceConfigPage::updateConfiguration()
{

    QString s;
    QStringList sl;
    QList<QVariant> vlist;
    QMap<QString,QVariant> dict;
    QList<QLineEdit*> lep;
    QStringList slp;

    if((m_methodComboBox->currentIndex() == 1) )

    {
        if( m_ipLineEdit->text().isEmpty() || m_netmaskLineEdit->text().isEmpty())
        {
            qDebug() << "ip or netmask is empty! " ;
            return;
        }
    }
    QDBusInterface* iface_serv = new QDBusInterface(DBUS_CON_SERVICE, services_item.objpath.path(), "net.connman.Service", QDBusConnection::systemBus(), this);

    // ipv4
    vlist.clear();
    vlist << "AutoConnect";
    bool setAutoConnect = true;
    vlist << setAutoConnect;
    iface_serv->callWithArgumentList(QDBus::AutoDetect, "SetProperty", vlist);
    // Only update if an entry has changed.
    if ((m_methodComboBox->currentIndex() != sl_ipv4_method_minuscules.indexOf(ipv4map.value("Method").toString()) )	|
            (m_ipLineEdit->text() != ipv4map.value("Address").toString() )      	|
            (m_netmaskLineEdit->text() != ipv4map.value("Netmask").toString() )				|
            (m_gatewayLineEdit->text() != ipv4map.value("Gateway").toString()) )			{

        vlist.clear();
        lep.clear();
        slp.clear();
        dict.clear();

        if (m_methodComboBox->currentIndex() >= 0) {
            vlist << "IPv4.Configuration";
            dict.insert("Method", sl_ipv4_method_minuscules.at(m_methodComboBox->currentIndex()) );
            lep << m_ipLineEdit << m_netmaskLineEdit << m_gatewayLineEdit;
            slp << "Address" << "Netmask" << "Gateway";

            for (int i = 0; i < lep.count(); ++i) {
                s = lep.at(i)->text();
                s = s.simplified(); // really should not be needed with the validator
                if (s.isEmpty() ) break;
                dict.insert(slp.at(i), s);
            } // for

            vlist << QVariant::fromValue(QDBusVariant(dict) );
            iface_serv->callWithArgumentList(QDBus::AutoDetect, "SetProperty", vlist);

            m_ipLineEdit->clear();
            m_netmaskLineEdit->clear();
            m_gatewayLineEdit->clear();
        } // if there is a valid index
    } // if ipv4 changed

    // cleanup
    iface_serv->deleteLater();

    switch(QMessageBox::warning(this,"Warning",tr("Modifying the IP will cause the MEasy HMI Web Server to be unusable. Do you need to reboot?"),
        QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel))
    {
    case QMessageBox::Ok:
        QProcess::execute("reboot");
        break;
    case QMessageBox::Cancel:
        qDebug("Nothing to do!");
        break;
    default:
        break;
    }
    return;

}
void ServiceConfigPage::showServiceDetails()
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
