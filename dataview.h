#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QDialog>

#include "sqlite3_base.h"

#include<QStandardItemModel>

namespace Ui {
class dataview;
}

class dataview : public QDialog
{
    Q_OBJECT

public:
    explicit dataview(QWidget *parent = 0);
    ~dataview();

    QStandardItemModel *model;

    sqlite3_base *sql_test;

private:
    Ui::dataview *ui;
};

#endif // DATAVIEW_H
