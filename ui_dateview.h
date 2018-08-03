/********************************************************************************
** Form generated from reading UI file 'dateview.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATEVIEW_H
#define UI_DATEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_dateview
{
public:

    void setupUi(QDialog *dateview)
    {
        if (dateview->objectName().isEmpty())
            dateview->setObjectName(QStringLiteral("dateview"));
        dateview->resize(581, 521);

        retranslateUi(dateview);

        QMetaObject::connectSlotsByName(dateview);
    } // setupUi

    void retranslateUi(QDialog *dateview)
    {
        dateview->setWindowTitle(QApplication::translate("dateview", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class dateview: public Ui_dateview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATEVIEW_H
