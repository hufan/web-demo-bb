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

    m_support_info_label = new QLabel(this);
    m_support_info_label->setGeometry(0,0,m_width,m_height);
    m_support_info_label->setScaledContents(true);

#if 1

    QSettings           *m_Settings = NULL;
    QString             m_current_language="en";

    m_Settings = new QSettings(QString(MXDE_SETTING_FILE_PATH), QSettings::IniFormat);

    m_Settings->beginGroup("LANGUAGE");
    m_current_language = m_Settings->value("language").toString();
    if(m_current_language.isEmpty()){
        m_current_language = "en";
    }
    m_Settings->endGroup();
    m_Settings->sync();

    QString rs;
    if(m_current_language == "zh_CN"){
        QPixmap label_pixmap(":/res/support-ch.png");
        m_support_info_label->setPixmap(label_pixmap);
    }
    else
    {
        QPixmap label_pixmap(":/res/support-en.png");
        m_support_info_label->setPixmap(label_pixmap);
    }

    m_support_info_label->setText(rs);
#endif
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

