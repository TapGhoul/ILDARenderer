#include "mainwindow.h"
#include "ildaserializer.h"
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    ILDASerializer::coordinates();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
