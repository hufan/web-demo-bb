#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QString>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QScrollBar>
#include "taskinfo.h"
Taskinfo::Taskinfo(QWidget *parent)
    :QWidget(parent)
{
    this->setFixedSize(parent->width(),parent->height()-40);


}
Taskinfo::~Taskinfo()
{

}
void Taskinfo::initUI()
{
    m_taskListWidget = new QListWidget(this);
    QListWidgetItem *title = new QListWidgetItem(tr("PID")+"\t" + tr("Process") + "\t\t" +
                                                 tr("STAT") + "\t" +
                                                 tr("Priority") + "\t" +
                                                 tr("VSZ"), m_taskListWidget);

    m_taskListWidget->setObjectName("TaskListWidget");
    m_taskListWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:15px;}" );

    m_totalTaskLable = new QLabel(this);
    m_runTaskLable = new QLabel(this);
    m_sleepTaskLable = new QLabel(this);
    m_zombieTaskLable = new QLabel(this);

    mainLayout = new QGridLayout(this);

    mainLayout->addWidget(m_taskListWidget,0,0,10,8);
    mainLayout->addWidget(m_totalTaskLable,2,8,1,2);
    mainLayout->addWidget(m_runTaskLable,4,8,1,2);
    mainLayout->addWidget(m_sleepTaskLable,6,8,1,2);
    mainLayout->addWidget(m_zombieTaskLable,8,8,1,2);
    getTaskInfo();

}
void Taskinfo::getTaskInfo()
{
    QFile tempFile; //用于打开系统文件
    QString tempStr; //读取文件信息字符串
    QString id_of_pro;
    bool ok;
    int find_start = 3;
    int a, b;
    int nProPid; //进程PID

    QDir qd("/proc");
    QStringList qsList = qd.entryList();
    QString qs = qsList.join("\n");
    while (1)
    {
        //获取进程PID
        a = qs.indexOf("\n", find_start);
        b = qs.indexOf("\n", a+1);
        find_start = b;
        id_of_pro = qs.mid(a+1, b-a-1);
        totalProNum++;
        nProPid = id_of_pro.toInt(&ok, 10);
        if(!ok)
        {
            break;
        }

        //打开PID所对应的进程状态文件
        tempFile.setFileName("/proc/" + id_of_pro + "/stat");
        if ( !tempFile.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(this, tr("warning"), tr("The pid stat file can not open!"), QMessageBox::Yes);
            return;
        }
        tempStr = tempFile.readLine();
        if (tempStr.length() == 0)
        {
            break;
        }
        a = tempStr.indexOf("(");
        b = tempStr.indexOf(")");
        proName = tempStr.mid(a+1, b-a-1);
        proName.trimmed(); //删除两端的空格
        proState = tempStr.section(" ", 2, 2);
        proPri = tempStr.section(" ", 17, 17);
        proMem = tempStr.section(" ", 22, 22);
        switch ( proState.at(0).toLatin1() )
        {
        case 'S':   number_of_sleep++; break; //Sleep
        case 'R':   number_of_run++; break; //Running
        case 'Z':   number_of_zombie++; break; //Zombie
        default :   break;
        }

        if (proName.length() >= 10)
        {
            QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, m_taskListWidget);
        }
        else
        {
            if(proName == "mxbackend")
            {
                QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                            proName + "\t" +
                                                            proState + "\t" +
                                                            proPri + "\t" +
                                                            proMem, m_taskListWidget);
            }
            else
                QListWidgetItem *item = new QListWidgetItem(id_of_pro + "\t" +
                                                        proName + "\t\t" +
                                                        proState + "\t" +
                                                        proPri + "\t" +
                                                        proMem, m_taskListWidget);
        }
    }

    QString totalstr = QString(tr("Total Process")).append(": %1").arg(totalProNum-1);
    QString runstr = QString(tr("Running Process")).append(": %1").arg(number_of_run);
    QString sleepstr = QString(tr("Sleep Process")).append(": %1").arg(number_of_sleep);
    QString zombiestr = QString(tr("Zombie Process")).append(": %1").arg(number_of_zombie);
    m_totalTaskLable->setText(totalstr);
    m_runTaskLable->setText(runstr);
    m_sleepTaskLable->setText(sleepstr);
    m_zombieTaskLable->setText(zombiestr);
}
