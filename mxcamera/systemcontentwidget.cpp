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
//#include <QTimer>
#include <iostream>

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

void SystemContentWidget::initUI()
{
    mainLayout = new QGridLayout();
    m_Grbox_Group = new QGroupBox("Camera");
    m_Grbox_Layout = new QGridLayout(m_Grbox_Group);

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
    hLayout1->setSpacing(120);
    hLayout1->addStretch();
    hLayout1->addWidget(m_preview_Button);
    hLayout1->addStretch();
    hLayout1->addWidget(m_photograph_Button);
    hLayout1->addStretch();
    hLayout1->addWidget(m_savephoto_Button);
    // hLayout1->addWidget(m_update_Button);
    hLayout1->addStretch();

    QHBoxLayout *hLayout2 = new QHBoxLayout(m_Grbox_Group);
    hLayout2->addWidget(m_show_pic);

//     QGridLayout *grid = new QGridLayout();
    m_Grbox_Layout->addLayout(hLayout1,0,0,1,10);
    m_Grbox_Layout->addLayout(hLayout2,1,0,1,10);
    m_Grbox_Group->setLayout(m_Grbox_Layout);
    mainLayout->addWidget(m_Grbox_Group, 0,0);
    this->setLayout(mainLayout);

    connect(m_photograph_Button, SIGNAL(clicked()), this, SLOT(click_Photograph()));
    connect(m_preview_Button, SIGNAL(clicked()), this, SLOT(camera_preview_v4l2()));
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

    sleep(2);
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

