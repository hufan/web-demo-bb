#include "systemcontentwidget.h"

#include <QDebug>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QFile>
#include <QTextCodec>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "mxde.h"
#include "systemcontentwidget.h"
#include "mxde.h"
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
void SystemContentWidget::createSoftwareInfoGroupBox()
{

    m_SoftwareInfoGroupBox = new QGroupBox(this);
    m_SoftwareInfoGroupBox->setTitle(tr("Software Information"));
    m_SoftwareLayout = new QGridLayout(m_SoftwareInfoGroupBox);


    mQtDemoLable = new QLabel(m_SoftwareInfoGroupBox);
    mQtDemoLable->setText(tr("HMI Version :"));
    mQtDemoValueLable = new QLabel(m_SoftwareInfoGroupBox);
    mQtDemoValueLable->setText(infoList.at(0));
    m_SoftwareLayout->addWidget(mQtDemoLable,0,0,1,1);
    m_SoftwareLayout->addWidget(mQtDemoValueLable,0,1,1,1);

    mLinuxLable = new QLabel(m_SoftwareInfoGroupBox);
    mLinuxLable->setText(tr("Linux Version :"));
    mLinuxValueLable = new QLabel(m_SoftwareInfoGroupBox);
    mLinuxValueLable->setText(infoList.at(1));
    m_SoftwareLayout->addWidget(mLinuxLable,1,0,1,1);
    m_SoftwareLayout->addWidget(mLinuxValueLable,1,1,1,1);



    mubootLable = new QLabel(m_SoftwareInfoGroupBox);
    mubootLable->setText(tr("U-Boot Version :"));
    mubootValueLable = new QLabel(m_SoftwareInfoGroupBox);
    mubootValueLable->setText(infoList.at(2));
    m_SoftwareLayout->addWidget(mubootLable,2,0,1,1);
    m_SoftwareLayout->addWidget(mubootValueLable,2,1,1,1);

    mComplierLable = new QLabel(m_SoftwareInfoGroupBox);
    mComplierLable->setText(tr("Complier Version :"));
    mComplierValueLable = new QLabel(m_SoftwareInfoGroupBox);
    mComplierValueLable->setText(infoList.at(3));
    m_SoftwareLayout->addWidget(mComplierLable,3,0,1,1);
    m_SoftwareLayout->addWidget(mComplierValueLable,3,1,1,1);

    mLinuxlogo = new QLabel(m_SoftwareInfoGroupBox);
    mLinuxlogo->setScaledContents(true);
    QPixmap label_pixmap(":/res/images/logo_linux.jpg");
    mLinuxlogo->setPixmap(label_pixmap);
    m_SoftwareLayout->addWidget(mLinuxlogo,0,5,4,4,Qt::AlignRight |Qt::AlignTop);


    m_SoftwareInfoGroupBox->setLayout(m_SoftwareLayout);
}
void SystemContentWidget::createHardwareInfoGroupBox()
{
    m_HardwareInfoGroupBox = new QGroupBox(tr("Hardware Information"));
    m_HardwareLayout = new QGridLayout(m_HardwareInfoGroupBox);

    mManufacturerLable = new QLabel(m_HardwareInfoGroupBox);
    mManufacturerLable->setText(tr("Manufacturer :"));
    mManufacturerValueLable = new QLabel(m_HardwareInfoGroupBox);
    mManufacturerValueLable->setText(infoList.at(4));
    m_HardwareLayout->addWidget(mManufacturerLable,0,0,1,1);
    m_HardwareLayout->addWidget(mManufacturerValueLable,0,1,1,1);

    mBoardLable = new QLabel(m_HardwareInfoGroupBox);
    mBoardLable->setText(tr("Board :"));
    mBoardValueLable = new QLabel(m_HardwareInfoGroupBox);
    mBoardValueLable->setText(infoList.at(5));
    m_HardwareLayout->addWidget(mBoardLable,1,0,1,1);
    m_HardwareLayout->addWidget(mBoardValueLable,1,1,1,1);

    mCPULable = new QLabel(m_HardwareInfoGroupBox);
    mCPULable->setText(tr("CPU :"));
    mCPUValueLable = new QLabel(m_HardwareInfoGroupBox);
    mCPUValueLable->setText(infoList.at(6));
    m_HardwareLayout->addWidget(mCPULable,2,0,1,1);
    m_HardwareLayout->addWidget(mCPUValueLable,2,1,1,1);

    mMemoryLable = new QLabel(m_HardwareInfoGroupBox);
    mMemoryLable->setText(tr("Memory :"));
    mMemortValueLable = new QLabel(m_HardwareInfoGroupBox);
    mMemortValueLable->setText(infoList.at(7));
    m_HardwareLayout->addWidget(mMemoryLable,3,0,1,1);
    m_HardwareLayout->addWidget(mMemortValueLable,3,1,1,1);

    mStorageLable = new QLabel(m_HardwareInfoGroupBox);
    mStorageLable->setText(tr("Storage :"));
    mStorageValueLable = new QLabel(m_HardwareInfoGroupBox);
    mStorageValueLable->setText(infoList.at(8));
    m_HardwareLayout->addWidget(mStorageLable,4,0,1,1);
    m_HardwareLayout->addWidget(mStorageValueLable,4,1,1,1);

    mManufacturerlogo = new QLabel(m_HardwareInfoGroupBox);
    mManufacturerlogo->setScaledContents(true);
    QPixmap label_pixmap(":/res/images/logo_myir.png");
    mManufacturerlogo->setPixmap(label_pixmap);

    m_HardwareLayout->addWidget(mManufacturerlogo,1,5,4,4,Qt::AlignRight |Qt::AlignTop);
    m_HardwareInfoGroupBox->setLayout(m_HardwareLayout);

}
void SystemContentWidget::get_system_cfg()
{
    QString val;
    QFile f(MXDE_BOARD_CFG_PATH);
    QTextCodec *codec = QTextCodec::codecForName("UTF8");

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
         qDebug()  << "Open failed.";

    }
    val = f.readAll();
    f.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonValue value1 = jsonObj.value(QString("board_info"));
    QJsonObject item1 = value1.toObject();
    QJsonValue value2 = item1.value("system");
    QJsonObject item2 = value2.toObject();

    infoList << item2.value("HMI_version").toString();
    infoList << item2.value("linux_version").toString();
    infoList << item2.value("uboot_version").toString();
    infoList << item2.value("gcc_version").toString();
    infoList << item2.value("manufacturer").toString();
    infoList << item2.value("board").toString();
    infoList << item2.value("CPU").toString();
    infoList << item2.value("memory").toString();
    infoList << item2.value("storage").toString();

}
void SystemContentWidget::initUI()
{
    get_system_cfg();
    createSoftwareInfoGroupBox();
    createHardwareInfoGroupBox();

    mainLayout = new QGridLayout;
    mainLayout->addWidget(m_SoftwareInfoGroupBox, 0,0);
    mainLayout->addWidget(m_HardwareInfoGroupBox,1,0);
    this->setLayout(mainLayout);
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

void SystemContentWidget::setDbusProxy(MxDE *mxde)
{
    m_mxde = mxde;
}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

