#include "mainwidget.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Times New Roman");
    font.setPointSize(18);
    qApp->setFont(font);
    MainWidget w;
    w.show();

    return a.exec();
}
