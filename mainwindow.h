#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dataview.h"

#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    dataview *m_dataview;
    QLabel *label1;   //文本窗体

private:
    Ui::MainWindow *ui;
    QStackedWidget *stack;  //堆栈窗体
};

#endif // MAINWINDOW_H
