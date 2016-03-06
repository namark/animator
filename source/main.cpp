#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.Init(argv[1],argv[0]);
    w.show();
    return a.exec();
}
