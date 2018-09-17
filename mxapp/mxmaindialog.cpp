/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName:
* Author: Sunny.Guo
* Version: 1.0
* Date:
* Description:
*
* History:
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle
*
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#include "mxmaindialog.h"
#include "constant.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QPoint>
#include <QPropertyAnimation>
#include <QTranslator>
#include <QTimer>
#include <QTime>
#include <QEventLoop>
#include <QCoreApplication>
#include <QPalette>

MxMainDialog::MxMainDialog(MxApplication *app, QWidget *parent, int w, int h)
    : QDialog(parent)
{
// create a MxDE object and connect its signals to MxMainDialog
    this->setApplication(app);
    m_width = w;
    m_height = h;
    m_default_action_width = m_width;
    m_default_content_width = m_width;
    m_default_action_height = DEFAULT_ACTION_HEIGHT(m_height);
    m_default_content_height = DEFAULT_CONTENT_HEIGHT(m_height);

    m_other_action_width = m_width;
    m_other_content_width = m_width;
    m_other_action_height = OTHER_ACTION_HEIGHT(m_height);
    m_other_content_height = OTHER_CONTENT_HEIGHT(m_height);

    qDebug() << m_default_action_width << m_default_action_height << "m_default_action_height\n" << endl;
    qDebug() << m_other_action_width << m_other_action_height << "m_other_action_height\n" << endl;
    qDebug() << m_default_content_width << m_default_content_height << "m_default_content_height\n" << endl;
    qDebug() << m_other_content_width << m_other_content_height << "m_other_content_height\n" << endl;
    qDebug() << m_width << m_height << "\n" << endl;

    this->resize(m_width, m_height);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint);
    this->setWindowOpacity(1);
    this->setStyleSheet("QDialog{border: 1px solid white;border-radius:1px;background-color: #ffffff;}");
    m_status = HOMEPAGE;
    m_statusFlag = false;

    m_Settings = new QSettings(QString(MXDE_SETTING_FILE_PATH), QSettings::IniFormat);

    m_Settings->beginGroup("LANGUAGE");
    m_current_language = m_Settings->value("language").toString();
    if(m_current_language.isEmpty()){
        m_current_language = "zh_CN";
        m_Settings->setValue("language",m_current_language);
    }
    m_Settings->endGroup();
    m_Settings->sync();

    home_action_widget = NULL;
    home_content_widget = NULL;
    box_action_widget = NULL;
    box_content_widget = NULL;

    spreadGroup = NULL;
    gatherGroup = NULL;

//    shadow_widget = new ShadowWidget(this);
//    shadow_widget->setGeometry(rect());
//    shadow_widget->setColor(QColor("#666666"));
//    shadow_widget->setOpacity(0);
////    shadow_widget->hide();

    default_action_widget = new BaseWidget();
    default_action_widget->setGeometry(QRect(0,0,m_default_action_width,m_default_action_height));
    default_action_widget->setFixedSize(m_default_action_width,m_default_action_height);
    default_action_widget->setAutoFillBackground(true);

    other_action_widget = new BaseWidget(this);
    other_action_widget->setGeometry(QRect(0,m_other_action_height*(-1),m_other_action_width,m_other_action_height));
    other_action_widget->setFixedSize(m_other_action_width,m_other_action_height);
    other_action_widget->setAutoFillBackground(true);
    other_action_widget->setObjectName("transparentWidget");

    topStack = new QStackedWidget(other_action_widget);
    topStack->setGeometry(other_action_widget->rect());
//    QPalette palette_back;
//    palette_back.setBrush(QPalette::Background, QBrush(QPixmap(":/res/images/myir/mxde_background1.png")));
//    default_action_widget->setPalette(palette_back);
//    other_action_widget->setPalette(palette_back);
//    default_action_widget->setStyleSheet("border-image: url(:/res/images/myir/mxde_background1.png)");
//    other_action_widget->setStyleSheet("border-image: url(:/res/images/myir/mxde_background1.png)");

    default_content_widget = new BaseWidget(this);
    default_content_widget->setGeometry(QRect(QRect(0,0,m_width, m_height)));
    default_content_widget->setFixedSize(m_width, m_height);
    default_content_widget->setAutoFillBackground(true);
//    default_content_widget->setPalette(palette_back);
//    default_content_widget->setStyleSheet("border-image: url(:/res/images/myir/mxde_background1.png)");
    default_content_widget->setObjectName("transparentWidget");

    other_content_widget = new BaseWidget(this);
    other_content_widget->setGeometry(QRect(0,m_height,m_width, m_other_content_height));
    other_content_widget->setFixedSize(m_other_content_width,m_other_content_height);
    other_content_widget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::white));
    other_content_widget->setPalette(palette);

    bottomStack = new QStackedWidget(other_content_widget);
    bottomStack->setGeometry(other_content_widget->rect());

    this->initHomePage();
    this->initOtherPage();
    this->initAnimation();
    this->initConnect();

}

MxMainDialog::~MxMainDialog()
{
    if(home_action_widget != NULL){
        delete home_action_widget;
        home_action_widget = NULL;
    }
    if(home_content_widget != NULL){
        delete home_content_widget;
        home_content_widget = NULL;
    }
    if(box_action_widget != NULL){
        delete box_action_widget;
        box_action_widget = NULL;
    }
    if(box_content_widget != NULL){
        delete box_content_widget;
        box_content_widget = NULL;
    }

}

void MxMainDialog::setApplication(MxApplication *app)
{
    m_app = app;
}

//void MxMainDialog::setDbusProxy(MxDE *mxde)
//{
//    m_mxde = mxde ;
//}

void MxMainDialog::setTranslator(QTranslator *tr)
{
    this->m_translator = tr;
}

void MxMainDialog::display()
{

       int windowWidth, windowHeight = 0;
       windowWidth = QApplication::desktop()->screenGeometry(0).width();
       windowHeight = QApplication::desktop()->screenGeometry(0).height();
       this->move((windowWidth - this->width()) / 2,(windowHeight - this->height()) / 2);
       this->show();
       this->raise();
//        QTimer::singleShot(100, this, SLOT(startDbusDaemon()));

}

void MxMainDialog::initAnimation()
{
    QPoint origPoint(0, m_default_content_height);
    QPoint needPoint(0, m_other_action_height);

    QPoint origPoint1(0, 0);
    QPoint needPoint1(0, m_default_action_height*(-1));

    QPoint origPoint2(0, m_other_action_height*(-1));
    QPoint needPoint2(0, 0);

    QPoint origPoint3(0, 0);
    QPoint needPoint3(0, m_height);

    QPoint origPoint4(0, m_height);
    QPoint needPoint4(0, m_other_action_height);

    QPropertyAnimation *mainActionAnimation = new QPropertyAnimation(default_action_widget, "pos");
    mainActionAnimation->setDuration(700);
    mainActionAnimation->setStartValue(origPoint1);
    mainActionAnimation->setEndValue(needPoint1);

    QPropertyAnimation *mainActionAnimation2 = new QPropertyAnimation(other_action_widget, "pos");
    mainActionAnimation2->setDuration(700);
    mainActionAnimation2->setStartValue(origPoint2);
    mainActionAnimation2->setEndValue(needPoint2);


    QPropertyAnimation *mainContentAnimation = new QPropertyAnimation(default_content_widget, "pos");
    mainContentAnimation->setDuration(700);
    mainContentAnimation->setStartValue(origPoint3);
    mainContentAnimation->setEndValue(needPoint3);

    QPropertyAnimation *mainContentAnimation2 = new QPropertyAnimation(other_content_widget, "pos");
    mainContentAnimation2->setDuration(700);
    mainContentAnimation2->setStartValue(origPoint4);
    mainContentAnimation2->setEndValue(needPoint4);

//    QPropertyAnimation  *m_toTrans = new QPropertyAnimation(shadow_widget, "opacity");
//    m_toTrans->setDuration(200);
//    m_toTrans->setStartValue(1);
//    m_toTrans->setEndValue(0);

    spreadGroup = new QParallelAnimationGroup(this);
    spreadGroup->addAnimation(mainActionAnimation);
    spreadGroup->addAnimation(mainActionAnimation2);
    spreadGroup->addAnimation(mainContentAnimation);
    spreadGroup->addAnimation(mainContentAnimation2);
//    spreadGroup->addAnimation(m_toTrans);

    QPropertyAnimation *mainActionBackAnimation2 = new QPropertyAnimation(other_action_widget, "pos");
    mainActionBackAnimation2->setDuration(500);
    mainActionBackAnimation2->setStartValue(needPoint2);
    mainActionBackAnimation2->setEndValue(origPoint2);

    QPropertyAnimation *mainActionBackAnimation = new QPropertyAnimation(default_action_widget, "pos");
    mainActionBackAnimation->setDuration(500);
    mainActionBackAnimation->setStartValue(needPoint1);
    mainActionBackAnimation->setEndValue(origPoint1);

    QPropertyAnimation *mainContentBackAnimation = new QPropertyAnimation(default_content_widget, "pos");
    mainContentBackAnimation->setDuration(500);
    mainContentBackAnimation->setStartValue(needPoint3);
    mainContentBackAnimation->setEndValue(origPoint3);

    QPropertyAnimation *mainContentBackAnimation2 = new QPropertyAnimation(other_content_widget, "pos");
    mainContentBackAnimation2->setDuration(500);
    mainContentBackAnimation2->setStartValue(needPoint4);
    mainContentBackAnimation2->setEndValue(origPoint4);

//    QPropertyAnimation  *m_toGray = new QPropertyAnimation(shadow_widget, "opacity");
//    m_toGray->setDuration(200);
//    m_toGray->setStartValue(0);
//    m_toGray->setEndValue(1);

    gatherGroup = new QParallelAnimationGroup(this);
    gatherGroup->addAnimation(mainActionBackAnimation);
    gatherGroup->addAnimation(mainActionBackAnimation2);
    gatherGroup->addAnimation(mainContentBackAnimation);
    gatherGroup->addAnimation(mainContentBackAnimation2);
//    gatherGroup->addAnimation(m_toGray);

    connect(spreadGroup, SIGNAL(finished()), this, SLOT(OnOpenAnimFinished()));
    connect(gatherGroup, SIGNAL(finished()), this, SLOT(onCloseAnimFinished()));

}

void MxMainDialog::initHomePage()
{
    if(home_action_widget == NULL)
    {
        QGridLayout *home_top_grid_layout = new QGridLayout();
        home_action_widget = new HomeActionWidget(this, m_app, m_default_action_width, m_default_action_height);
        home_top_grid_layout->addWidget(home_action_widget,0,0);
        default_action_widget->setLayout(home_top_grid_layout);
        home_top_grid_layout->setSpacing(0);
        home_top_grid_layout->setContentsMargins(0, 0, 0, 0);
    }

    if(home_content_widget == NULL)
    {
        QVBoxLayout *home_bottom_grid_layout = new QVBoxLayout();
        home_content_widget = new HomeContentWidget(this, m_app, m_default_content_width, m_default_content_height);
        home_content_widget->initUI();
        home_content_widget->initConnection();
        home_content_widget->setObjectName("transparentWidget");

        home_bottom_grid_layout->addStretch();
        home_bottom_grid_layout->addWidget(home_content_widget);
        default_content_widget->setLayout(home_bottom_grid_layout);
        home_bottom_grid_layout->setSpacing(0);
        home_bottom_grid_layout->setContentsMargins(0, 0, 0, 0);
    }
}

void MxMainDialog::initOtherPage()
{
    if(box_action_widget == NULL)
    {
        box_action_widget = new BoxActionWidget(this, m_app , m_other_action_width, m_other_action_height);
    }

    topStack->addWidget(box_action_widget);

    if(box_content_widget == NULL)
    {
        box_content_widget = new BoxContentWidget(this, m_app, m_other_content_width, m_other_content_height);
    }
    box_content_widget->setParentWindow(this);
    bottomStack->addWidget(box_content_widget);
}

void MxMainDialog::initConnect()
{
    connect(home_content_widget, SIGNAL(sigLanguageChanged(QString)), this, SLOT(OnLanguageChanged(QString)));
    connect(box_action_widget, SIGNAL(CloseBox()), this, SLOT(OnCloseBox()));
    connect(box_content_widget, SIGNAL(sigClickSystemInfo()), this, SLOT(OnSystemInfoClicked()));
    connect(box_content_widget, SIGNAL(demoStarted()), this , SLOT(OnDemoStarted()));
    connect(box_content_widget, SIGNAL(demoFinished()), this, SLOT(OnDemoFinished()));
}

void MxMainDialog::onCloseAnimFinished()
{

}

void MxMainDialog::OnOpenAnimFinished()
{
//    shadow_widget->hide();
}

void MxMainDialog::OnApplicationClosed()
{
//    shadow_widget->hide();
}

void MxMainDialog::OnSystemInfoClicked()
{
        qDebug() << "OnSystemInfoClicked  \n" << endl;

//        MxSystemDialog *w = new MxSystemDialog(m_app);
//        w->setParentWindow(this);
//        w->setDbusProxy(m_mxde);
//        this->hide();
//        this->lower();
//        w->display();
}

void MxMainDialog::OnCloseBox()
{
    OnCurrentPageChanged(0);
}

void MxMainDialog::OnCurrentPageChanged(int index){
    if(index == 1){
//        shadow_widget->show();
        topStack->setCurrentWidget(box_action_widget);
        bottomStack->setCurrentWidget(box_content_widget);
        spreadGroup->start();
    }
    else
    {
        qDebug() << "onCurrentPageChanged " << index << "\n" << endl;
        //bottomStack->setCurrentWidget(home_content_widget);
        gatherGroup->start();

    }
}

void MxMainDialog::OnLanguageChanged(QString language)
{
    qDebug() << "onLanguageChanged: " << language << endl;
    m_current_language = language;
    if(language == "zh_CN")
    {
        qDebug()<<"loading zh_CN\n" << endl;
        m_translator->load(QString(":/res/translation/mxapp_zh_CN.qm"));
    }
    else
    {
        m_app->removeTranslator(m_translator);
        m_translator->load(QString(":/res/translation/mxapp_en.qm"));
    }

    m_app->installTranslator(m_translator);

    m_Settings->beginGroup("LANGUAGE");
    QString old_language=  m_Settings->value("language").toString();
    if(m_current_language != old_language){
        m_Settings->setValue("language",m_current_language);
    }
    m_Settings->endGroup();
    m_Settings->sync();

    box_action_widget->setCurrentLanguage(language);
    box_content_widget->setCurrentLanguage(language);

    OnCurrentPageChanged(1);

}

void MxMainDialog::OnDemoStarted()
{
    qDebug() << "hide main \n" << endl;
    if(this->isHidden()){
        return;
    }
    lower();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(20);
    animation->setStartValue(this->pos());
    animation->setEndValue(this->pos()-QPoint(0,QApplication::desktop()->screenGeometry(0).height()));
    animation->start();
    connect(animation, SIGNAL(finished()), this, SLOT(hide()));
}

void MxMainDialog::OnDemoFinished()
{
    qDebug() << "show main\n" << endl;
    this->display();
    raise();
    activateWindow();
}

void MxMainDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    main_skin_pixmap = new QPixmap(":/res/images/myir/mxde_background1.png");

    painter.drawPixmap(0,0,this->width(), this->height(), *main_skin_pixmap);
}
