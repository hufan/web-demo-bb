#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

#include "mxmaindialog.h"
#include "mxledindicator.h"

MxLedIndicator::MxLedIndicator(QWidget *parent) :
    QWidget(parent)
{
//    QVBoxLayout *vLayout = new QVBoxLayout(this);
 //   QWidget *mLed = new QWidget(this);
//    mLabel = new QLabel(this);
//    mLabel->setText(tr("LED"));

    setFixedSize(28, 28);
    lit = false;
    ledOnColor=Qt::green;
    ledOffColor=Qt::gray;
    ledOnPattern = Qt::SolidPattern;
    ledOffPattern = Qt::SolidPattern;
    ledSize=20;

//    vLayout->addWidget(mLed);
//    vLayout->addWidget(mLabel);

//    setLayout(vLayout);
}

void MxLedIndicator::paintEvent(QPaintEvent *) {
  QPainter p(this);
  lit ?  p.setBrush(QBrush(ledOnColor, ledOnPattern)) : p.setBrush(QBrush(ledOffColor, ledOffPattern));
  p.drawEllipse(0,0,ledSize,ledSize);
}

void MxLedIndicator::switchLedIndicator() {
  lit = ! lit;
  repaint();
}

void MxLedIndicator::setState(bool state)
{
    lit = state;
    repaint();
}

bool MxLedIndicator::getState()
{
    return lit;
}

void MxLedIndicator::setLedName(const QString &str)
{
    ledName = str;
//    mLabel->setText(ledName);
    repaint();
}

QString MxLedIndicator::getLedName()
{
    return ledName;
}

void MxLedIndicator::toggle()
{
    lit = ! lit;
    repaint();
}

void MxLedIndicator::setOnColor(QColor onColor)
{
    ledOnColor=onColor;
    repaint();
}
void MxLedIndicator::setOffColor(QColor offColor)
{
    ledOffColor=offColor;
    repaint();
}
void MxLedIndicator::setOnPattern(Qt::BrushStyle onPattern)
{
    ledOnPattern=onPattern;
    repaint();
}
void MxLedIndicator::setOffPattern(Qt::BrushStyle offPattern)
{
    ledOffPattern=offPattern;
    repaint();
}
void MxLedIndicator::setLedSize(int size)
{
    ledSize=size;
    setFixedSize(size+10, size+10);
    repaint();
}
