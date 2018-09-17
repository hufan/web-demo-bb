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
#include "systemcontentwidget.h"
//#include "mxledindicator.h"
//#include "mxde.h"
#include "constant.h"
#include "mxapplication.h"
#include "servicetab.h"

#include <QObject>
#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSignalMapper>
#include <QDirIterator>
#include <QStringList>
#include <QPalette>
#include "servicetab.h"


SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height ):BaseWidget(parent,obj)
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

}

SystemContentWidget::~SystemContentWidget()
{
}

void SystemContentWidget::initUI()
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setFixedSize(m_width, m_height);
    m_tabWidget->setFocus(Qt::NoFocusReason);
    tab_map.clear();

    if (QDBusConnection::systemBus().isConnected() )
    {
        con_manager = new QDBusInterface(DBUS_CON_SERVICE, DBUS_PATH, DBUS_CON_MANAGER, QDBusConnection::systemBus(), this);
        if (! con_manager->isValid() )
        {
            qDebug() << "con_manager dbus service is not valid!";
        }
    }

    QDBusConnection::systemBus().connect(DBUS_CON_SERVICE, DBUS_PATH, DBUS_CON_MANAGER, "ServicesChanged", this, SLOT(dbsServicesChanged(QList<QVariant>, QList<QDBusObjectPath>, QDBusMessage)));

    getServices();
    setServiceTab();
}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}


void SystemContentWidget::initConnection()
{

}

void SystemContentWidget::display()
{

}


void SystemContentWidget:: setServiceTab()
{
    int newidx = 0,inx = 0;
    QString cursvc = QString();

     //if the combobox has any items in it save the nick name of the service we are viewing

    if (m_tabWidget->count() > 0)
    {
        inx = m_tabWidget->currentIndex();
        cursvc = m_tabWidget->tabText(inx);
    }
    // initilize the page2 display widgets
    m_tabWidget->clear();

    for (int row = 0; row < services_list.size(); ++row) {
        QString device_name = getDeviceName(services_list.at(row).objpath);
        ServiceTab *tab = NULL;
        if(tab_map.contains(services_list.at(row).objpath.path()))
        {
            QMap<QString,ServiceTab *>::iterator iter = tab_map.find(services_list.at(row).objpath.path());
            tab = iter.value();
            m_tabWidget->addTab(tab, device_name);
        }
        else
        {
            tab = new ServiceTab(services_list.at(row),this);
            tab_map.insert(services_list.at(row).objpath.path(),tab);
            m_tabWidget->addTab(tab, device_name);
        }
        if (device_name == cursvc)
            newidx = row;
    } // services for loop


    m_tabWidget->setCurrentIndex(newidx);


}
void SystemContentWidget:: deleteServiceTab(const QDBusObjectPath& objpath)
{

        QMapIterator<QString,ServiceTab *> iter(tab_map);

        while(iter.hasNext()){
            iter.next();
            if(iter.key() == objpath.path())
            {
                qDebug()<<"delete";
                ServiceTab *tab =  iter.value();
                int tab_indx = m_tabWidget->indexOf(tab);
                m_tabWidget->removeTab(tab_indx);

                delete tab;
                tab_map.remove(iter.key());
            }
        }

}
void SystemContentWidget::getServices()
{
    QDBusMessage reply = con_manager->call("GetServices");
    if (reply.type() != QDBusMessage::ReplyMessage)
    {
        qDebug() << "GetProperties error!";
        return;
    }

    // call the function to get the map values
    services_list.clear();
    if(getArray(services_list, reply))
    {
        for (int i = 0; i < services_list.size(); ++i) {
          QDBusConnection::systemBus().disconnect(DBUS_CON_SERVICE, services_list.at(i).objpath.path(), "net.connman.Service", "PropertyChanged", this, SLOT(dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage)));
          QDBusConnection::systemBus().connect(DBUS_CON_SERVICE, services_list.at(i).objpath.path(), "net.connman.Service", "PropertyChanged", this, SLOT(dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage)));
        } // for
    }

}
bool SystemContentWidget::getArray(QList<arrayElement>& r_list, const QDBusMessage& r_msg )
{
    // make sure r_msg is a QDBusArgument
    if ( ! r_msg.arguments().at(0).canConvert<QDBusArgument>() ) return false;

    // make sure the QDBusArgument holds an array
    const QDBusArgument &qdb_arg = r_msg.arguments().at(0).value<QDBusArgument>();
    if (qdb_arg.currentType() != QDBusArgument::ArrayType ) return false;

    // iterate over the QDBusArgument pulling array elements out and inserting into
    // an arrayElement structure.
    qdb_arg.beginArray();
    r_list.clear();

    while ( ! qdb_arg.atEnd() ) {
        // make sure the argument is a structure type
        if (qdb_arg.currentType() != QDBusArgument::StructureType ) return false;

        arrayElement ael;
        qdb_arg.beginStructure();
        qdb_arg >> ael.objpath >> ael.objmap;
        qdb_arg.endStructure();
        r_list.append (ael);
    } // while
    qdb_arg.endArray();

  return true;
}
QString SystemContentWidget::getDeviceName(const QDBusObjectPath& objpath)
{
  for (int i = 0; i < services_list.size(); ++i) {
    if (services_list.at(i).objpath == objpath) {
      QMap<QString,QVariant> submap;
      if (services_list.at(i).objmap.value("Type").toString() == "ethernet") {
        shared::extractMapData(submap, services_list.at(i).objmap.value("Ethernet") );
        if (submap.value("Interface").toString().isEmpty() )
          return services_list.at(i).objmap.value("Name").toString();
        else
          return QString(services_list.at(i).objmap.value("Name").toString() + " [%1]").arg(submap.value("Interface").toString() );
      } // if type ethernet

      else if ( services_list.at(i).objmap.value("Type").toString() == "wifi" && services_list.at(i).objmap.value("Name").toString().isEmpty() )
        return tr("[Hidden Wifi]");
      else
        return  services_list.at(i).objmap.value("Name").toString();
    } // if objpath matches
  } // for

  return QString();
}
void SystemContentWidget::dbsServicePropertyChanged(QString property, QDBusVariant dbvalue, QDBusMessage msg)
{
    QString s_path = msg.path();
    QVariant value = dbvalue.variant();
    QString s_state;

    //qDebug() << "dbsServicePropertyChanged";
    // replace the old values with the changed ones.
    for (int i = 0; i < services_list.count(); ++i) {
        if (s_path == services_list.at(i).objpath.path() ) {
            QMap<QString,QVariant> map = services_list.at(i).objmap;
            map.remove(property);
            map.insert(property, value );
            arrayElement ae = {services_list.at(i).objpath, map};
            services_list.replace(i, ae);
            s_state = map.value("State").toString();

            emit this->servicesChange(services_list.at(i));
            break;
        } // if
    } // for

    // update the widgets
    //updateDisplayWidgets();

  return;
}
void SystemContentWidget::dbsServicesChanged(QList<QVariant> vlist, QList<QDBusObjectPath> removed, QDBusMessage msg)
{

     //qDebug() << "dbsServicesChanged";
    // process removed services
    if (! removed.isEmpty() ) {
    for (int i = 0; i < services_list.count(); ++i) {
      if (removed.contains(services_list.at(i).objpath) ) {
        QDBusConnection::systemBus().disconnect(DBUS_CON_SERVICE, services_list.at(i).objpath.path(), "net.connman.Service", "PropertyChanged", this, SLOT(dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage)));
        deleteServiceTab(services_list.at(i).objpath);
        services_list.removeAt(i);

      } // if
     } // for
    } // if we needed to remove something

    // process added or changed servcies
    // Demarshall the raw QDBusMessage instead of vlist as it is easier..
    if (! vlist.isEmpty() ) {
    QList<arrayElement> revised_list;
    if (! getArray(revised_list, msg)) return;

    // merge the existing services_list into the revised_list
    // first find the original element that matches the revised
    for (int i = 0; i < revised_list.size(); ++i) {
      arrayElement revised_element = revised_list.at(i);
      arrayElement original_element = {QDBusObjectPath(), QMap<QString,QVariant>()};
      for (int j = 0; j < services_list.size(); ++j) {
        if (revised_element.objpath == services_list.at(j).objpath) {
          original_element = services_list.at(j);
          break;
        } // if
      } // j for

      // merge the new elementArray into the existing
      if (! original_element.objpath.path().isEmpty()) {
        QMapIterator<QString, QVariant> itr(revised_element.objmap);
        while (itr.hasNext()) {
          itr.next();
          original_element.objmap.insert(itr.key(), itr.value() );
        } // while

        // now insert the element into the revised list
        QDBusConnection::systemBus().disconnect(DBUS_CON_SERVICE, original_element.objpath.path(), "net.connman.Service", "PropertyChanged", this, SLOT(dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage)));
        revised_list.replace(i, original_element);
        QDBusConnection::systemBus().connect(DBUS_CON_SERVICE, revised_element.objpath.path(), "net.connman.Service", "PropertyChanged", this, SLOT(dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage)));

      } // if original element is not empty
    } // i for

    // now copy the revised list to services_list
    services_list.clear();
    services_list = revised_list;
    } // revised_list not emptyservices_list

    setServiceTab();

  return;
}


