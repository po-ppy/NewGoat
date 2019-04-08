#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("GoatDataServer");
    QApplication::setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("This is a server to receive data.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
                "source",
                QApplication::translate("main","SOurce file to copy."));
    parser.addPositionalArgument("destination",QApplication::translate("main","Desinaltion directory."));

    // A boolean option with a single name (-p)
    QCommandLineOption showProgressQption("p",QApplication::translate("main","Show progress during copy."));
    parser.addOption(showProgressQption);

    // A boolean option with multiple names (-f, --force)
    QCommandLineOption forceOption(QStringList() << "f" << "force",
                                   QApplication::translate("main","Overwirte existing files.") );
    parser.addOption(forceOption);

    // An option with a value
    QCommandLineOption targetDirectOption(QStringList() << "t" << "target-directory",
                                          QApplication::translate("main","Copy all source files into <directory>."),
                                          QApplication::translate("main","directory"));
    parser.addOption(targetDirectOption);
    parser.process(a);
    const QStringList args = parser.positionalArguments();
    if(args.length() != 2){
        qDebug() << "参数个数不对!";
    }else{
        //获取参数
        qDebug() << "first: " << args.at(0) << "    second: " << args.at(1);
    }

    //确定是否使用参数
    bool showPorgress = parser.isSet(showProgressQption);
    bool force = parser.isSet(forceOption);
    //获取参数
    QString targetDir = parser.value(targetDirectOption);
    qDebug() << targetDir;

    if(showPorgress){
        qDebug() << "showProgress is selected!";
    }else{
        qDebug() << "showPorgress is not selected!";
    }

    if(force){
        qDebug() << "force is selected!";
    }else{
        qDebug() << "force is not selected!";
    }
    //翻译
    QTranslator trans;
    QLocale locale;
    //判断机器语言环境
    if(locale.language() == QLocale::English ){
        qDebug() << "Language is English!!";
    }else if(locale.language() == QLocale::Chinese){
        qDebug() << "Language is China!!!";
    }else{
        qDebug() << "no Language is selected!!";
    }
    //加载语言包
    trans.load(":/translator/zh.qm");
    a.installTranslator(&trans);
    MainWindow w;
    w.show();

    return a.exec();
}
