#ifndef MEMINFO_H
#define MEMINFO_H

#include <QObject>
#include <QWidget>
#include <QProcess>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
class Miscinfo : public QWidget
{
    Q_OBJECT
public:
    explicit Miscinfo(QWidget *parent = 0);
    ~Miscinfo();
    void initUI();

    void SetLab();
    void Start(int interval);
    void Stop();
    void CheckSize(QString result, QString name);


private:

    QGridLayout     *mainLayout;
    QGroupBox       *m_cpuGroupBox;
    QLabel          *m_cpuLable;
    QHBoxLayout     *m_cpuHorizontalLayout;
    QGridLayout     *m_cpuLayout;
    QProgressBar    *m_cpuProgressBar;

    QGroupBox       *m_miscGroupBox;
    QGridLayout     *m_miscLayout;

    QLabel          *m_memLable;
    QProgressBar    *m_memProgressBar;

    QLabel          *m_memusedLable;
    QLabel          *m_storageusedLable;
    QLabel          *m_memleftLable;
    QLabel          *m_storageleftLable;
    QLabel          *m_memtotalLable;
    QLabel          *m_storagetotalLable;
    QHBoxLayout     *m_memHorizontalLayout;

    QLabel          *m_storageLable;
    QProgressBar    *m_storageProgressBar;
    QHBoxLayout     *m_stoHorizontalLayout;





    int totalNew, idleNew, totalOld, idleOld;
    int cpuPercent;

    int memoryPercent;
    int memoryAll;
    int memoryUse;
    int memoryFree;

    int storagePercent;
    int storageAll;
    int storageUse;
    int storageFree;

    QTimer *timerCPU;       //定时器获取CPU信息
    QTimer *timerMemory;    //定时器获取内存信息
    QTimer *timerStorage;    //定时器获取存储信息
    QProcess *process;

private slots:
    void GetCPU();
    void GetMemory();
    void GetStorage();
    void ReadData();
};

#endif // MEMIFNO_H
