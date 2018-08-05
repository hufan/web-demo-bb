#ifndef UI_LED_H
#define UI_LED_H

#include <QDialog>

#include <QButtonGroup>
#include <QPushButton>
#include <QDebug>

namespace Ui {
class ui_led;
}

class ui_led : public QDialog
{
    Q_OBJECT

public:
    explicit ui_led(QWidget *parent = 0);
    ~ui_led();


private slots:
    void button_on_clicked();


private:
    Ui::ui_led *ui;
};

#endif // UI_LED_H
