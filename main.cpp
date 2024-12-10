#include "TestTreeView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestTreeView w;
    w.show();
    return a.exec();
}
