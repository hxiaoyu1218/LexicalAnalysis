#include "mainform.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("GBK");//情况2

    QTextCodec::setCodecForLocale(codec);
   // QTextCodec::setCodecForCStrings(codec);
    MainForm *w=new MainForm();
    w->setWindowFlags(Qt::FramelessWindowHint);
    w->show();

    return a.exec();
}
