#include "systemcontentwidget.h"

#include <QDebug>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QVBoxLayout>
#include <QLabel>
#include <QSettings>
#include "systemcontentwidget.h"

#include "constant.h"
#include "mxapplication.h"
#include "miscinfo.h"
#include "taskinfo.h"
SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
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
   // m_mxde->callCloseSerialPort(m_serial_fd);

}
void SystemContentWidget::initUI()
{
    m_tableWidget = new QTabWidget(this);
    m_tableWidget->setFixedSize(m_width, m_height);
    m_tableWidget->setFocus(Qt::NoFocusReason);

    m_miscinfo = new Miscinfo(this);
    m_miscinfo->SetLab();
    m_miscinfo->Start(1000);
    m_miscinfo->initUI();
    m_taskinfo = new Taskinfo(this);
    m_taskinfo->initUI();
    m_tableWidget->addTab(m_miscinfo,tr("Performance Info"));
    m_tableWidget->addTab(m_taskinfo,tr("Process Info"));

}

void SystemContentWidget::initConnection()
{

}

void SystemContentWidget::display()
{

}
void SystemContentWidget::setApplication(MxApplication *app)
{
    m_app = app;
}


void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

