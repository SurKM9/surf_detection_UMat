#include "surf_detection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    surf_detection w;
    w.show();

    return a.exec();
}
