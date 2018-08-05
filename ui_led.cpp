#include "ui_led.h"
#include "ui_ui_led.h"

ui_led::ui_led(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_led)
{
    ui->setupUi(this);

    ui->groupBox->setTitle("LED Control");


    QPushButton* button_on = new QPushButton(this);
    button_on->setGeometry(QRect(50,75,100,35));
    button_on->setText("ON");

    QPushButton* button_off  = new QPushButton(this);
    button_off->setGeometry(QRect(200,75,100,35));
    button_off->setText("OFF");

    connect(button_on, SIGNAL(clicked()), this, SLOT(button_on_clicked()));

}

ui_led::~ui_led()
{
    delete ui;
}

void ui_led::button_on_clicked(){
    qDebug()<<"connect test "<<endl;
}
