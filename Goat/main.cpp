#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    QTranslator tran;
    tran.load(":/qt_zh_CN.qm");
    a.installTranslator(&tran);
    QFont font;
    font.setPointSize(16);
//    font.setPixelSize(16);
//    font.setFamily("黑体");
    a.setFont(font);
//    QApplication::setFont(font);
    MainWindow w;
    w.show();

    return a.exec();
}
