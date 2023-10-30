#include <QApplication>

#include "newcolor.h"

int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    NewColor window;
    window.show();
    a.exec();
}
