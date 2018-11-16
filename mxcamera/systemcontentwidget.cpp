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
    inint_flags=0;
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
          <<"/dev/video2"<<"/dev/video1") ;

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(mCameraDev);
    hLayout1->addWidget(mCameraDevComBox);

    mCameraResolution = new QLabel();
    mCameraResolution->setText(tr("Resolution:"));
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
void SystemContentWidget::createPhotoShowGroupBox()
{
    m_SendGroup = new QGroupBox(tr("Preview"));
    m_SendLayout = new QGridLayout(m_SendGroup);

    m_show_pic = new QLabel();
    m_show_pic->setText(tr(" "));

    m_SendLayout->addWidget(m_show_pic,0,0,2,99);
//    m_SendLayout->addLayout(vLayout,1,100,1,1,Qt::AlignBottom);
//    m_SendLayout->setColumnMinimumWidth(100,80);
    m_SendGroup->setLayout(m_SendLayout);
}
void SystemContentWidget::initUI()
{
    mainLayout = new QGridLayout;

    createSettingGroupBox();
    createPhotoShowGroupBox();

    mainLayout -> setRowStretch(0, 1);
    mainLayout -> setRowStretch(1, 4);

    mainLayout->addWidget(m_SettingGroup, 0,0);
    mainLayout->addWidget(m_SendGroup,1,0);
    this->setLayout(mainLayout);

    connect(m_photograph_Button, SIGNAL(clicked()), this, SLOT(click_Photograph()));
    connect(m_preview_Button, SIGNAL(clicked()), this, SLOT(camera_preview_v4l2()));
    connect(m_savephoto_Button, SIGNAL(clicked()), this, SLOT(click_SavePhoto()));

    /*init camera  and time */
    cameraTimer = new QTimer(this);
    connect( cameraTimer, SIGNAL(timeout()),this, SLOT(timerDone()) );
}
int SystemContentWidget::init_camera_dev()
{
    QString string_dev=mCameraDevComBox->currentText();
    QString string_resolution=mCameraResolutionComBox->currentText();
    QString width_str = string_resolution.mid(0, string_resolution.indexOf("x"));
    QString heigh_str = string_resolution.mid(string_resolution.indexOf("x")+1);
    QByteArray temp=string_dev.toLatin1();
//     name_device=temp.data();

   /* v4l2 camera */
//    char name_device[16] = "/dev/video2";
   m_camera_v4l2 = new v4l2_camera();
//    m_camera_v4l2->deviceName = name_device;
   m_camera_v4l2->deviceName = temp.data();
   m_camera_v4l2->fps=15;

   m_camera_v4l2->width=width_str.toInt();
   m_camera_v4l2->height=heigh_str.toInt();

   m_camera_v4l2->deviceOpen();
   m_camera_v4l2->deviceInit();
   m_camera_v4l2->captureStart();
}

int SystemContentWidget::click_Photograph()
{
    pause=0;
    QImage image = QImage((unsigned char *)m_camera_v4l2->buffers[m_camera_v4l2->buf.index].start, m_camera_v4l2->width,m_camera_v4l2->height, QImage::Format_RGB888);
    image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
    m_show_pic->setScaledContents(true);
    m_show_pic->setPixmap(QPixmap::fromImage(image));
    return 0;
}

int SystemContentWidget::camera_preview_v4l2()
{
    if(inint_flags==0) {
        inint_flags=1;
        init_camera_dev();
    }

    if(pause==0){
        cameraTimer->start( 200 );
        pause=1;
    }else{
        pause=0;
        cameraTimer->stop();
    }
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

    if(pause==1)
    if(m_camera_v4l2->mainloop()){
        QImage image = QImage((unsigned char *)m_camera_v4l2->buffers[m_camera_v4l2->buf.index].start, m_camera_v4l2->width,m_camera_v4l2->height, QImage::Format_RGB888);
        image.scaled(m_show_pic->size(), Qt::KeepAspectRatio);
        m_show_pic->setScaledContents(true);
        m_show_pic->setPixmap(QPixmap::fromImage(image));
    }

}
void SystemContentWidget::initConnection()
{
  connect(mCameraResolutionComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(release_camera_init()));
}
int SystemContentWidget::release_camera_init()
{   cameraTimer->stop();
    m_camera_v4l2->captureStop();
    m_camera_v4l2->deviceUninit();
    m_camera_v4l2->deviceClose();
    sleep(1);
    init_camera_dev();
    cameraTimer->start(200);
    return 0;
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

