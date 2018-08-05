#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dataview.h"
#include "ui_led.h"

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
    ui_led   *m_uiled;

    QLabel *label1;   //文本窗体

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stack;  //堆栈窗体
};

#endif // MAINWINDOW_H
