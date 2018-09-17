#include <QObject>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QByteArray>

#include "mxdesktopfile.h"
#include "mxproperties.h"

MxDesktopFile::MxDesktopFile(const QString &fileName, MxApplication *app)
{
    this->m_app = app;
	this->fileName = fileName;
	
	if(!QFile::exists(fileName)){
		return;
	}

	MxProperties desktop(fileName, "Desktop Entry");
    name = desktop.value("Name","").toString();
    name_zh_CN = desktop.value("Name[zh_CN]", "").toString();
	exec = desktop.value("Exec", "").toString();
	tryExec = desktop.value("TryExec", "").toString();
	icon = desktop.value("Icon", "").toString();
	type = desktop.value("Type", "Application").toString();
	categories = desktop.value("Categories").toString().remove(" ").split(";");
	mimeType = desktop.value("MimeType").toString().remove(" ").split(";");

	if(categories.first().compare("") == 0){
		categories.removeFirst();
	}

    process = new QProcess(this);

//    process->setProcessChannelMode(QProcess::ForwardedChannels);

    QObject::connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
                      this, SLOT(processFinished(int,QProcess::ExitStatus)));

    QObject::connect(process, SIGNAL(error(QProcess::ProcessError)),
                      this, SLOT(processError(QProcess::ProcessError)));

    QObject::connect(process, SIGNAL(started()), this, SLOT(processStarted()));

    QObject::connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processData()));
}

MxDesktopFile::~MxDesktopFile()
{
    if(process!=NULL){
        process->close();
        delete process;
        process = NULL;
    }
}

QString MxDesktopFile::getFileName() const
{
	return fileName;
}

QString MxDesktopFile::getPureFileName() const
{
	return fileName.split("/").last().remove(".desktop");
}

QString MxDesktopFile::getName() const
{
	return name;
}

QString MxDesktopFile::getNameCN() const
{
    return name_zh_CN;
}

QString MxDesktopFile::getExec() const
{
	return tryExec.isEmpty()?exec:tryExec;
}

QString MxDesktopFile::getIcon() const
{
	return icon;
}

QString MxDesktopFile::getType() const
{
	return type;
}

QStringList MxDesktopFile::getCategories() const
{
	return categories;
}

QStringList MxDesktopFile::getMimeType() const
{
	return mimeType;
}


void MxDesktopFile::launch()
{
    qDebug() << getExec() << endl;
    QString  parameter="";
    if(this->m_app){
        parameter += getExec();
        parameter +=" --width ";
        parameter += QString::number(this->m_app->getMainWindowWidth(),10);
        parameter +=" --height ";
        parameter += QString::number(this->m_app->getMainWindowHeight(),10);
        qDebug() << parameter << endl;
        process->start(parameter);
    }
    else{
        process->start(getExec());
    }

//    process->waitForFinished();
}

void MxDesktopFile::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    qDebug() << "Process finished: ";
    emit demoFinished();

    QObject::disconnect(this, SIGNAL(demoStarted()), 0, 0);
    QObject::disconnect(this, SIGNAL(demoFinished()), 0, 0);
}

void MxDesktopFile::processError(QProcess::ProcessError err)
{
    qDebug() << "Process error: " << err;
    if (err == QProcess::Crashed)
        emit demoFinished();
}


void MxDesktopFile::processStarted()
{
    //emit demoStarted();
    QTimer::singleShot(3000, this, SIGNAL(demoStarted()));
}

void MxDesktopFile::processData()
{
    qDebug() << "===== Read from subprocess:";
    QString  outStr = QString(process->readAllStandardOutput());
    qDebug() << outStr;
    if(outStr.contains("MainDialogShown")){
        emit this->demoStarted();
    }
    if(outStr.contains("MainDialogHide")){
        process->close();
        emit this->demoFinished();
    }
}
