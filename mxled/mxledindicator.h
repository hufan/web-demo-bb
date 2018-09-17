#ifndef MXLEDINDICATOR_H
#define MXLEDINDICATOR_H

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QString>
#include <QLabel>


class MxLedIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit MxLedIndicator(QWidget *parent = 0);

    void setState(bool state);
    bool getState();
    void toggle();
    void setOnColor(QColor onColor);
    void setOffColor(QColor offColor);
    void setOnPattern(Qt::BrushStyle onPattern);
    void setOffPattern(Qt::BrushStyle offPattern);
    void setLedSize(int size);
    void setLedName(const QString &str);
    QString getLedName();
    void getLedNameAndStatus(const QString &str);

  public slots:
    void switchLedIndicator();
  protected:
    void paintEvent(QPaintEvent *);
  private:
    bool lit;
    QColor ledOnColor;
    QColor ledOffColor;
    QString ledName;
    QLabel *mLabel;
    QWidget *mLed;
    Qt::BrushStyle ledOnPattern;
    Qt::BrushStyle ledOffPattern;
    int ledSize;
    int led_num;
};

#endif // MXLEDINDICATOR_H
