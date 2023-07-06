
#include "startpage.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startPage* w = new startPage();
    w->show();
    return a.exec();
}
