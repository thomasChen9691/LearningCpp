#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtwidgetsapptest.h"

class QtWidgetsAppTest : public QWidget
{
    Q_OBJECT

public:
    QtWidgetsAppTest(QWidget *parent = nullptr);
    ~QtWidgetsAppTest();

private:
    Ui::QtWidgetsAppTestClass ui;
};
