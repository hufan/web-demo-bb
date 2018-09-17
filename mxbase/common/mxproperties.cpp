#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QTextCodec>

#include "mxproperties.h"

MxProperties::MxProperties(const QString &fileName, const QString &group)
{
	if(!fileName.isEmpty()){
		load(fileName, group);
	}
}
MxProperties::MxProperties(const MxProperties &other)
{
	this->data = other.data;
}

bool MxProperties::load(const QString &fileName, const QString &group)
{
	QFile file(fileName);
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
	
	if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
		return false;
	}

	data.clear();

	bool groupFound = group.isEmpty();

	QTextStream in(&file);
    in.setCodec(codec);
	while(!in.atEnd()){
        QString line = codec->fromUnicode(in.readLine());
		
		if(line.trimmed().isEmpty()){
			continue;
		}

		if(!group.isEmpty()&&line.trimmed().startsWith("[")){
			QString tmp = line.trimmed().replace("[","").replace("]","");
			groupFound = (group.trimmed().compare(tmp) == 0);
		}

		if(groupFound && line.contains("=")){
			QStringList tmp = line.split("=");
            data.insert(tmp.at(0), tmp.at(1));
		}
	}

	file.close();
	return true;
}
	
bool MxProperties::save(const QString &fileName, const QString &group)
{
	QFile file(fileName);
	
	if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
		return false;
	}

	QTextStream out(&file);
	if(!group.isEmpty()){
		out<<"["+group+"]\n";
	}

	foreach(QString key, data.keys()){
		out<<key<<"="<<data.value(key).toString()<<"\n";
	}

	file.close();
	return true;
}


bool MxProperties::contains(const QString &key) const
{
	return data.contains(key);
}

QVariant MxProperties::value(const QString &key, const QVariant &defaultValue)
{
	return data.value(key, defaultValue);	
}

void MxProperties::set(const QString &key, const QVariant &value)
{
	if(data.contains(key)){
		data.take(key);
	}
	
	data.insert(key, value);
}

QStringList MxProperties::getKeys() const
{
	return data.keys();
}
