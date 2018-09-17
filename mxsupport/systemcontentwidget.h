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

#include "mxapplication.h"
#include "basewidget.h"
class SystemContentWidget : public BaseWidget
{
    Q_OBJECT
public:

    SystemContentWidget(QWidget *parent = 0,MxApplication *obj = 0, int width = 0, int height = 0);
    ~SystemContentWidget();
    void setApplication(MxApplication *app);

    void setParentWindow(QWidget *parent);
    void initUI();
    void initConnection();
    void display();

signals:


public slots:


private:
    QWidget                 *m_parent;
    MxApplication           *m_app;
    int                      m_width;
    int                      m_height;
    QGridLayout     *mainLayout;
    QLabel* m_support_info_label;
};

#endif // SYSTEMCONTENTWIDGET_H
