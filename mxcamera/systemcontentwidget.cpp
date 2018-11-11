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
#include <QTimer>
#include <iostream>
#include <QDateTime>
#include <qpainter.h>

#include "systemcontentwidget.h"
#include "constant.h"
#include "mxapplication.h"


#define JPEG_QUALITY  70

SystemContentWidget::SystemContentWidget(QWidget *parent, MxApplication *obj, int width , int height )
            :BaseWidget(parent,obj)
{
    
    pause=0;
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

    /*init camera  and time */
//    testTimer = new QTimer(this);
//    connect( testTimer, SIGNAL(timeout()),this, SLOT(timerDone()) );
//    testTimer->start( 100 );

}
SystemContentWidget::~SystemContentWidget()
{
    m_camera_v4l2->captureStop();
    m_camera_v4l2->deviceUninit();
    m_camera_v4l2->deviceClose();
}

void SystemContentWidget::createSettingGroupBox()
{
    m_SettingGroup = new QGroupBox(tr("Settings"));
    m_SettingLayout = new QGridLayout(m_SettingGroup);

    mCameraDev = new QLabel(m_SettingGroup);
    mCameraDev->setText(tr("Port:"));

    mCameraDevComBox = new QComboBox();
    mCameraDevComBox->setFixedWidth(120);
    mCameraDevComBox->insertItems(0,QStringList() \
          <<"/dev/video1"<<"/dev/video1") ;

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(mCameraDev);
    hLayout1->addWidget(mCameraDevComBox);

    mCameraResolution = new QLabel();
    mCameraResolution->setText(tr("Baud:"));
    mCameraResolutionComBox =  new QComboBox();
    mCameraResolutionComBox->insertItems(0,QStringList() \
          <<"176x144"<<"352x288"<<"160x120"<< "320x240" \
          <<"640x480"<<"1280x720") ;
    mCameraResolutionComBox->setFixedWidth(120);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(mCameraResolution);
    hLayout2->addWidget(mCameraResolutionComBox);

    m_preview_Button = new QPushButton();
    m_preview_Button->setObjectName("serialButton");
    m_preview_Button->setText(tr("Preview"));
    QHBoxLayout *hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(m_preview_Button);

    m_photograph_Button = new QPushButton();
    m_photograph_Button->setObjectName("serialButton");
    m_photograph_Button->setText(tr("Photograph"));
    QHBoxLayout *hLayout4 = new QHBoxLayout();
    hLayout4->addWidget(m_photograph_Button);

    m_savephoto_Button = new QPushButton();
    m_savephoto_Button->setObjectName("serialButton");
    m_savephoto_Button->setText(tr("Save"));
    QHBoxLayout *hLayout5 = new QHBoxLayout(m_SettingGroup);
    hLayout5->addWidget(m_savephoto_Button);

    if(m_width < DEFAULT_SCREEN_WIDTH){
//        m_SettingLayout->addLayout(hLayout1,0,0,1,1);
//        m_SettingLayout->addLayout(hLayout2,1,0,1,1);
//        m_SettingLayout->addLayout(hLayout3,2,0,1,1);
//        m_SettingLayout->addLayout(hLayout4,3,0,1,1);
//        m_SettingLayout->addLayout(hLayout5,4,0,1,1);
//        m_SettingLayout->addLayout(hLayout6,5,0,1,1);
    }
    else
    {
//        m_SettingLayout->addLayout(hLayout4,0,31,1,10);
//        m_SettingLayout->addLayout(hLayout5,0,41,1,10);
//        m_SettingLayout->addLayout(hLayout6,1,43,1,8);
    }

    m_SettingLayout->addLayout(hLayout1,0,0,1,1);
    m_SettingLayout->addLayout(hLayout2,0,2,1,2);
    m_SettingLayout->addLayout(hLayout3,0,5,1,2);
    m_SettingLayout->addLayout(hLayout4,0,10,1,2);
    m_SettingLayout->addLayout(hLayout5,0,40,1,2);
    m_SettingGroup->setLayout(m_SettingLayout);


}
void SystemContentWidget::createSendGroupBox()
{
    m_SendGroup = new QGroupBox(tr("Preview"));
    m_SendLayout = new QGridLayout(m_SendGroup);

//    m_SendTextEdit1 = new QTextEdit(m_SendGroup);

    m_show_pic = new QLabel();
    m_show_pic->setText(tr("photo show "));

//    m_SendPushButton = new QPushButton(m_SendGroup);
//    m_SendPushButton->setObjectName("serialButton");
//    m_SendPushButton->setText(tr("Send"));

//    QVBoxLayout *vLayout = new QVBoxLayout(m_SendGroup);
//    vLayout->addWidget(m_SendPushButton);

    m_SendLayout->addWidget(m_show_pic,0,0,2,99);
//    m_SendLayout->addLayout(vLayout,1,100,1,1,Qt::AlignBottom);
//    m_SendLayout->setColumnMinimumWidth(100,80);

    m_SendGroup->setLayout(m_SendLayout);
}
void SystemContentWidget::initUI()
{
//   return ;

    mainLayout = new QGridLayout;

    createSettingGroupBox();
    createSendGroupBox();

#if 0
    mainLayout = new QGridLayout();
    m_Grbox_Group = new QGroupBox("Camera");
    m_Grbox_Layout = new QGridLayout(m_Grbox_Group);


    m_SettingGroup = new QGroupBox(tr("Settings"));
    m_SettingLayout = new QGridLayout(m_SettingGroup);

    mCameraResolution = new QLabel(m_SettingGroup);
    mCameraResolution->setText(tr("resolution:"));

    mCameraResolutionComBox = new QComboBox(m_SettingGroup);
    mCameraResolutionComBox->insertItems(0,QStringList() \
          <<"176x144"<<"352x288"<<"160x120"<< "320x240" \
          <<"640x480"<<"1280x720") ;

    QHBoxLayout *hLayout1 = new QHBoxLayout(m_SettingGroup);
    hLayout1->addWidget(mCameraResolution);
    hLayout1->addWidget(mCameraResolutionComBox);


    m_SettingLayout->addLayout(hLayout1,0,0,1,10);
    m_SettingGroup->setLayout(m_SettingLayout);

    mainLayout->addWidget(m_SettingGroup, 0,0);
    this->setLayout(mainLayout);
#endif

//    m_SettingGroup = new QGroupBox(tr("Settings"));
//    m_SettingLayout = new QGridLayout(m_SettingGroup);
//    mSerialPort = new QLabel(m_SettingGroup);
//    mSerialPort->setText(tr("Port:"));
//    mSerialPortComboBox = new QComboBox(m_SettingGroup);
//    QHBoxLayout *hLayout1 = new QHBoxLayout(m_SettingGroup);
//    hLayout1->addWidget(mSerialPort);
//    hLayout1->addWidget(mSerialPortComboBox);

//    mSerialBaudRate = new QLabel(m_SettingGroup);
//    mSerialBaudRate->setText(tr("Baud:"));
//    mSerialBaudRateComboBox =  new QComboBox(m_SettingGroup);
//    mSerialBaudRateComboBox->insertItems(0,QStringList() \
//          <<"300"<<"600"<<"1200"<< "2400" \
//          <<"4800"<<"9600"<<"19200"<<"38400" \
//          <<"57600"<<"115200") ;
//    QHBoxLayout *hLayout2 = new QHBoxLayout(m_SettingGroup);
//    hLayout2->addWidget(mSerialBaudRate);
//    hLayout2->addWidget(mSerialBaudRateComboBox);

//    m_SettingLayout->addLayout(hLayout1,0,0,1,10);
//    m_SettingLayout->addLayout(hLayout2,0,11,1,10);
//    m_SettingGroup->setLayout(m_SettingLayout);

//    mainLayout->setColumnStretch(0, 1);
//    mainLayout->setColumnStretch(1, 1);
    mainLayout -> setRowStretch(0, 1);
    mainLayout -> setRowStretch(1, 4);

    mainLayout->addWidget(m_SettingGroup, 0,0);
    mainLayout->addWidget(m_SendGroup,1,0);

    this->setLayout(mainLayout);
//    return ;

#if 0
    m_show_pic = new QLabel(m_Grbox_Group);
    m_show_pic->setText(tr("photo show "));

    m_photograph_Button = new QPushButton();
    m_photograph_Button->setObjectName("serialButton");
    m_photograph_Button->setText(tr("Photograph"));
//     m_insert_Button->setMinimumSize(10,10);

    m_preview_Button = new QPushButton();
    m_preview_Button->setObjectName("serialButton");
    m_preview_Button->setText(tr("Preview"));

    m_savephoto_Button = new QPushButton();
//    m_delete_Button->setObjectName("sqlite3_delet");
    m_savephoto_Button->setObjectName("serialButton");
    m_savephoto_Button->setText(tr("Save"));

    QHBoxLayout *hLayout1 = new QHBoxLayout(m_Grbox_Group);
//    hLayout1->setContentsMargins(5,5,5,5);
//    hLayout1->setMargin(40);
    hLayout1->setSpacing(10);
//    hLayout1->addStretch();
    hLayout1->addWidget(mCameraResolution);
//    hLayout1->addStretch();
    hLayout1->addWidget(mCameraResolutionComBox);
//    hLayout1->addStretch();
    hLayout1->addWidget(m_preview_Button);
//    hLayout1->addStretch();
    hLayout1->addWidget(m_photograph_Button);
//    hLayout1->addStretch();q
    hLayout1->addWidget(m_savephoto_Button);
    // hLayout1->addWidget(m_update_Button);
//    hLayout1->addStretch();

    QHBoxLayout *hLayout2 = new QHBoxLayout(m_Grbox_Group);
    hLayout2->addWidget(m_show_pic);

//     QGridLayout *grid = new QGridLayout();
    m_Grbox_Layout->addLayout(hLayout1,0,0,1,10);
    m_Grbox_Layout->addLayout(hLayout2,1,0,1,10);
    m_Grbox_Group->setLayout(m_Grbox_Layout);
    mainLayout->addWidget(m_Grbox_Group, 0,0);
    this->setLayout(mainLayout);
#endif

    connect(m_photograph_Button, SIGNAL(clicked()), this, SLOT(click_Photograph()));
    connect(m_preview_Button, SIGNAL(clicked()), this, SLOT(camera_preview_v4l2()));
    connect(m_savephoto_Button, SIGNAL(clicked()), this, SLOT(click_SavePhoto()));
    //    connect(m_delete_Button, SIGNAL(clicked()), this, SLOT(clickDeleteData_sqlite()));


    /* v4l2 camera */
    char name_device[16] = "/dev/video2";

    m_camera_v4l2 = new v4l2_camera();
    m_camera_v4l2->deviceName = name_device;
    m_camera_v4l2->fps=15;
    m_camera_v4l2->width=320;
    m_camera_v4l2->height=240;
    m_camera_v4l2->deviceOpen();
    m_camera_v4l2->deviceInit();
    m_camera_v4l2->captureStart();

    //sleep(2);
    /*init camera  and time */
    cameraTimer = new QTimer(this);
    connect( cameraTimer, SIGNAL(timeout()),this, SLOT(timerDone()) );
    cameraTimer->start( 200 );
}

int SystemContentWidget::click_Photograph()
{
    pause=1;
    QImage image = QImage((unsigned char *)m_camera_v4l2->buffers[m_camera_v4l2->buf.index].start, m_camera_v4l2->width,m_camera_v4l2->height, QImage::Format_RGB888);
    image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
    m_show_pic->setScaledContents(true);
    m_show_pic->setPixmap(QPixmap::fromImage(image));
    return 0;
}

int SystemContentWidget::camera_preview_v4l2()
{
  pause=0;
  return 0;
}
int SystemContentWidget::click_SavePhoto()
{

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMdd-hhmmsszzz");
    QString filepatch;
    QString fileName;
    QString all_save_info;
    char *photo_save;

    filepatch="/home/root/";
    fileName=".jpg";

    all_save_info.append(filepatch);
    all_save_info.append(current_date);
    all_save_info.append(fileName);

    QByteArray temp=all_save_info.toLatin1();
    photo_save=temp.data();

//    QTextStream out(stdout);
//    out << all_save_info<<endl;

    m_camera_v4l2->save_jpeg((char *)m_camera_v4l2->buffers[m_camera_v4l2->buf.index].start, m_camera_v4l2->width, m_camera_v4l2->height, photo_save);
    return 0;
}

void SystemContentWidget::timerDone(){

    if(pause==0)
    if(m_camera_v4l2->mainloop()){
        QImage image = QImage((unsigned char *)m_camera_v4l2->buffers[m_camera_v4l2->buf.index].start, m_camera_v4l2->width,m_camera_v4l2->height, QImage::Format_RGB888);
        image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
        m_show_pic->setScaledContents(true);
        m_show_pic->setPixmap(QPixmap::fromImage(image));
    }

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

//void SystemContentWidget::setDbusProxy(MxDE *mxde)
//{
//    m_mxde = mxde;
//}

void SystemContentWidget::setParentWindow(QWidget *parent)
{
    m_parent = parent;
}

