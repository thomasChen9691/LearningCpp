/********************************************************************************
** Form generated from reading UI file 'qtwidgetsapptest.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWIDGETSAPPTEST_H
#define UI_QTWIDGETSAPPTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtWidgetsAppTestClass
{
public:

    void setupUi(QWidget *QtWidgetsAppTestClass)
    {
        if (QtWidgetsAppTestClass->objectName().isEmpty())
            QtWidgetsAppTestClass->setObjectName(QString::fromUtf8("QtWidgetsAppTestClass"));
        QtWidgetsAppTestClass->resize(600, 400);

        retranslateUi(QtWidgetsAppTestClass);

        QMetaObject::connectSlotsByName(QtWidgetsAppTestClass);
    } // setupUi

    void retranslateUi(QWidget *QtWidgetsAppTestClass)
    {
        QtWidgetsAppTestClass->setWindowTitle(QCoreApplication::translate("QtWidgetsAppTestClass", "QtWidgetsAppTest", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtWidgetsAppTestClass: public Ui_QtWidgetsAppTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWIDGETSAPPTEST_H
