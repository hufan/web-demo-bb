#ifndef TASKINFO_H
#define TASKINFO_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
class Taskinfo : public QWidget
{
    Q_OBJECT
public:
    explicit Taskinfo(QWidget *parent = 0);
    ~Taskinfo();
    void initUI();
    void getTaskInfo();

private:
    QGridLayout     *mainLayout;
    QListWidget     *m_taskListWidget;
    QLabel          *m_totalTaskLable;
    QLabel          *m_runTaskLable;
    QLabel          *m_sleepTaskLable;
    QLabel          *m_zombieTaskLable;
    int number_of_sleep = 0, number_of_run = 0, number_of_zombie = 0;
    int totalProNum = 0; //进程总数
    QString proName; //进程名
    QString proState; //进程状态
    QString proPri; //进程优先级
    QString proMem; //进程占用内存

};
#endif // TASKINFO_H
