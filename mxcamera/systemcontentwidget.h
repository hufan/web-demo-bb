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
//#include "mxde.h"

#include <QGroupBox>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QList>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QHeaderView>

#include "mxapplication.h"
#include "basewidget.h"
#include "v4l2_camera.h"


class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:
//    MxDE    *m_mxde;
    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);
//    void setDbusProxy(MxDE *mxde);
    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();


    QTimer       *cameraTimer;
    QImage       *frame_show;
    v4l2_camera  *m_camera_v4l2;

public slots:
    void timerDone();
    int click_Photograph();
    int click_SavePhoto();
    int camera_preview_v4l2();
    int release_camera_init();

private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;

    QGroupBox               *m_SettingGroup;
    QGridLayout             *m_SettingLayout;

    QStringList             infoList;
    QLabel                  *m_show_pic;

    QLabel                  *mCameraResolution;
    QComboBox               *mCameraResolutionComBox;
    QLabel                  *mCameraDev;
    QComboBox               *mCameraDevComBox;

    QGroupBox               *m_SendGroup;
    QGridLayout             *m_SendLayout;

    QPushButton              *m_photograph_Button;
    QPushButton              *m_savephoto_Button;
    QPushButton              *m_preview_Button;
    int                      pause;
    int                      inint_flags;

    QGroupBox            *m_Grbox_Group;
    QGroupBox            *m_Grbox_Group2;
    QGridLayout          *m_Grbox_Layout;
    QGridLayout          *mainLayout;

    QTableView           *m_tableview;

    void            createSettingGroupBox();
    void            createPhotoShowGroupBox();
     int            init_camera_dev();

};

#endif // SYSTEMCONTENTWIDGET_H
