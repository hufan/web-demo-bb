#ifndef SYSTEMCONTENTWIDGET_H
#define SYSTEMCONTENTWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include "mxde.h"
#include "mxapplication.h"
#include "basewidget.h"
#include <QGroupBox>
class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();
    void createSoftwareInfoGroupBox();
    void createHardwareInfoGroupBox();
    void get_system_cfg();
signals:
    void clickSerialPortCombobox();

public slots:

private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;
    QGridLayout     *mainLayout;
    QGridLayout *m_SoftwareLayout;
    QGridLayout *m_HardwareLayout;
    QGroupBox *m_SoftwareInfoGroupBox;
    QGroupBox *m_HardwareInfoGroupBox;

    QLabel* mQtDemoLable;
    QLabel* mQtDemoValueLable;
    QLabel* mLinuxLable;
    QLabel* mLinuxValueLable;
    QLabel* mComplierLable;
    QLabel* mComplierValueLable;
    QLabel* mubootLable;
    QLabel* mubootValueLable;
    QLabel* mLinuxlogo;

    QLabel* mManufacturerLable;
    QLabel* mManufacturerValueLable;
    QLabel* mBoardLable;
    QLabel* mBoardValueLable;
    QLabel* mCPULable;
    QLabel* mCPUValueLable;
    QLabel* mMemoryLable;
    QLabel* mMemortValueLable;
    QLabel* mStorageLable;
    QLabel* mStorageValueLable;
    QLabel* mManufacturerlogo;

    QStringList infoList;

};

#endif // SYSTEMCONTENTWIDGET_H
