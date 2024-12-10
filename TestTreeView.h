#pragma once

#include <QtWidgets/QDialog>
#include "ui_TestTreeView.h"

class TestTreeView : public QDialog
{
    Q_OBJECT

public:
    TestTreeView(QWidget *parent = nullptr);
    ~TestTreeView();

private:
    Ui::TestTreeViewClass ui;
};
