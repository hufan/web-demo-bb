#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "miscinfo.h"
#define MB (1024 * 1024)
#define KB (1024)
Miscinfo::Miscinfo(QWidget *parent)
    :QWidget(parent)
{

    this->setFixedSize(parent->width(),parent->height()-40);
    totalNew = idleNew = totalOld = idleOld = 0;
    cpuPercent = 0;

    memoryPercent = 0;
    memoryAll = 0;
    memoryUse = 0;
    memoryFree = 0;


    timerCPU = new QTimer(this);
    connect(timerCPU, SIGNAL(timeout()), this, SLOT(GetCPU()));

    timerMemory = new QTimer(this);
    connect(timerMemory, SIGNAL(timeout()), this, SLOT(GetMemory()));

    timerStorage = new QTimer(this);
    connect(timerStorage, SIGNAL(timeout()), this, SLOT(GetStorage()));

    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(ReadData()));

    this->Start(100);

}
Miscinfo::~Miscinfo()

{

}
void Miscinfo::initUI()
{

    m_cpuGroupBox = new QGroupBox(this);
    m_cpuGroupBox->setTitle(tr("CPU"));

    m_cpuLable = new QLabel(m_cpuGroupBox);
    m_cpuLable->setText(tr("CPU: "));
    m_cpuProgressBar  = new QProgressBar(m_cpuGroupBox);
    m_cpuProgressBar->setOrientation(Qt::Horizontal);
    m_cpuProgressBar->setMinimum(0);
    m_cpuProgressBar->setMaximum(100);
    m_cpuProgressBar->setValue(50);

    m_cpuHorizontalLayout = new QHBoxLayout(m_cpuGroupBox);
    m_cpuHorizontalLayout->addWidget(m_cpuLable);
    m_cpuHorizontalLayout->addWidget(m_cpuProgressBar);

    m_miscGroupBox  = new QGroupBox(this);
    m_miscGroupBox->setTitle(tr("Memory And Storage"));

    m_memLable = new QLabel(m_miscGroupBox);
    m_memLable->setText(tr("Memory: "));
    m_memProgressBar = new QProgressBar(m_miscGroupBox);
    m_memProgressBar->setOrientation(Qt::Horizontal);
    m_memProgressBar->setMinimum(0);
    m_memProgressBar->setMaximum(100);
    m_memProgressBar->setValue(50);

    m_memusedLable = new QLabel(m_miscGroupBox);
    m_memleftLable = new QLabel(m_miscGroupBox);
    m_memtotalLable = new QLabel(m_miscGroupBox);

    QString memUsed = QString(tr("Used: ")).append("%1").arg(memoryUse);
    QString memFree= QString(tr("Free: ")).append("%1").arg(memoryUse) ;
    QString memTotal= QString(tr("Total: ")).append("%1").arg(memoryUse) ;

    m_memusedLable->setText(memUsed);
    m_memleftLable->setText(memFree);
    m_memtotalLable->setText(memTotal);


    m_storageLable = new QLabel(m_miscGroupBox);
    m_storageLable->setText(tr("Storage: "));
    m_storageProgressBar = new QProgressBar(m_miscGroupBox);
    m_storageProgressBar->setOrientation(Qt::Horizontal);
    m_storageProgressBar->setMinimum(0);
    m_storageProgressBar->setMaximum(100);
    m_storageProgressBar->setValue(50);
    m_storageusedLable = new QLabel(m_miscGroupBox);
    m_storageleftLable = new QLabel(m_miscGroupBox);
    m_storagetotalLable = new QLabel(m_miscGroupBox);
    QString stroageUsed = QString(tr("Used: ")).append("%1").arg(memoryUse);
    QString stroageFree= QString(tr("Free: ")).append("%1").arg(memoryUse) ;
    QString stroageTotal= QString(tr("Total: ")).append("%1").arg(memoryUse) ;

    m_storageusedLable->setText(stroageUsed);
    m_storageleftLable->setText(stroageFree);
    m_storagetotalLable->setText(stroageTotal);


    m_miscLayout = new QGridLayout(m_miscGroupBox);
    m_miscLayout->addWidget(m_memLable,0,0,2,10);
    m_miscLayout->addWidget(m_memProgressBar,0,1,2,10);
    m_miscLayout->addWidget(m_memusedLable,1,3,1,2);
    m_miscLayout->addWidget(m_memleftLable,1,5,1,2);
    m_miscLayout->addWidget(m_memtotalLable,1,7,1,2);
    m_miscLayout->addWidget(m_storageLable,2,0,2,10);
    m_miscLayout->addWidget(m_storageProgressBar,2,1,2,10);
    m_miscLayout->addWidget(m_storageusedLable,3,3,1,2);
    m_miscLayout->addWidget(m_storageleftLable,3,5,1,2);
    m_miscLayout->addWidget(m_storagetotalLable,3,7,1,2);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_cpuGroupBox,0,0,1,1);
    mainLayout->addWidget(m_miscGroupBox,1,0,2,1);
    this->setLayout(mainLayout);

   // retranslateUi(this);

}
void Miscinfo::SetLab()
{

    GetCPU();
    GetMemory();
    GetStorage();
}

void Miscinfo::Start(int interval)
{
    timerCPU->start(interval);
    timerMemory->start(interval + 200);
    timerStorage->start(interval + 500);
}

void Miscinfo::Stop()
{
    timerCPU->stop();
    timerMemory->stop();
}

void Miscinfo::GetCPU()
{

    if (process->state() == QProcess::NotRunning) {
        totalNew = idleNew = 0;
        process->start("cat /proc/stat");
    }

}

void Miscinfo::GetMemory()
{

    if (process->state() == QProcess::NotRunning) {
        process->start("cat /proc/meminfo");
    }

}

void Miscinfo::GetStorage()
{
    if (process->state() == QProcess::NotRunning) {
        process->start("df -h");
    }
}
void Miscinfo::ReadData()
{
    while (!process->atEnd()) {
        QString s = QLatin1String(process->readLine());
        if (s.startsWith("cpu")) {
            QStringList list = s.split(" ");
            idleNew = list.at(5).toInt();
            foreach (QString value, list) {
                totalNew += value.toInt();
            }

            int total = totalNew - totalOld;
            int idle = idleNew - idleOld;
            cpuPercent = 100 * (total - idle) / total;
            totalOld = totalNew;
            idleOld = idleNew;
            break;
        } else if (s.startsWith("MemTotal")) {
            s = s.replace(" ", "");
            s = s.split(":").at(1);
            memoryAll = s.left(s.length() - 3).toInt() / KB;
        } else if (s.startsWith("MemFree")) {
            s = s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree = s.left(s.length() - 3).toInt() / KB;
        } else if (s.startsWith("Buffers")) {
            s = s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree += s.left(s.length() - 3).toInt() / KB;
        } else if (s.startsWith("Cached")) {
            s = s.replace(" ", "");
            s = s.split(":").at(1);
            memoryFree += s.left(s.length() - 3).toInt() / KB;
            memoryUse = memoryAll - memoryFree;
            memoryPercent = 100 * memoryUse / memoryAll;
            break;
        }else if(s.startsWith("/dev/root")){
            CheckSize(s, "emmc");
        }else if(s.startsWith("ubi0:rootfs")){
            CheckSize(s, "nand");
        }else if(s.startsWith("/dev/mmcblk1p1")){
            CheckSize(s, "SD卡");
        }else if(s.startsWith("/dev/sda1")){
            CheckSize(s, "U盘");
        }
    }
    m_cpuProgressBar->setValue(cpuPercent);
    m_memProgressBar->setValue(memoryPercent);
    QString memUsed = QString(tr("Used: ")).append("%1MB").arg(memoryUse);
    QString memFree= QString(tr("Free: ")).append("%1MB").arg(memoryFree) ;
    QString memTotal= QString(tr("Total: ")).append("%1MB").arg(memoryAll) ;

    m_memusedLable->setText(memUsed);
    m_memleftLable->setText(memFree);
    m_memtotalLable->setText(memTotal);

}
void Miscinfo::CheckSize(QString result, QString name)
{
    QString dev, use, free, all;
    int percent = 0;
    QStringList list = result.split(" ");
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        QString s = list.at(i).trimmed();
        if (s == "") {
            continue;
        }
        index++;

        if (index == 1) {
            dev = s;
        } else if (index == 2) {
            all = s;
        } else if (index == 3) {
            use = s;
        } else if (index == 4) {
            free = s;
        } else if (index == 5) {
            storagePercent = s.left(s.length() - 1).toInt();
            break;
        }
    }

    if (name.length() > 0) {
        dev = name;
    }

    m_storageProgressBar->setValue(storagePercent);
    QString stroageUsedstr = QString(tr("Used: ")).append("%1B").arg(use);
    QString stroageFreestr= QString(tr("Free: ")).append("%1B").arg(free);
    QString stroageTotalstr= QString(tr("Total: ")).append("%1B").arg(all);
    m_storageusedLable->setText(stroageUsedstr);
    m_storageleftLable->setText(stroageFreestr);
    m_storagetotalLable->setText(stroageTotalstr);

}
