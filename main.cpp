#include "QtWidgetsApplication2.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsApplication2 w;
    w.show();
    return a.exec();
}
