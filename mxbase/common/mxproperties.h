#ifndef MXPROPERTIES_H
#define MXPROPERTIES_H

#include <QVariant>
#include <QObject>
#include <QMap>

class MxProperties
{
public:
	explicit MxProperties(const QString &fileName="", const QString &group="");
	explicit MxProperties(const MxProperties &other);

	QVariant value(const QString &key, const QVariant &defaultValue=QVariant());

	bool load(const QString &fileName, const QString &group="");
	
    bool save(const QString &fileName, const QString &group="");

	void set(const QString &key, const QVariant &value);

	bool contains(const QString &key) const;

	QStringList getKeys() const;

protected:
	QMap<QString, QVariant> data;
};

#endif // MXPROPERTIES_H
