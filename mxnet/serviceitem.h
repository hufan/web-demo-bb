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
#ifndef SERVICEITEM_H
#define SERVICEITEM_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QDBusInterface>

struct arrayElement
{
  QDBusObjectPath objpath;
  QMap<QString,QVariant> objmap;
};
namespace shared {
class ServiceItem : public QObject
{
    Q_OBJECT
public:
    arrayElement ael;
    explicit ServiceItem(QObject *parent = 0);


signals:

public slots:

private:

};
bool extractMapData(QMap<QString,QVariant>&,const QVariant&);
}
#endif // SERVICEITEM_H
