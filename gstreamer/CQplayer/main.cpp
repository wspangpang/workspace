#include <QtGui/QApplication>
#include "CQplayerGUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CQplayerGUI w;
    w.show();

    return a.exec();
}
