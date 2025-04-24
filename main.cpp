#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   // a.setStyle(QStyleFactory::create("Fusion"));
    Widget w;
    w.show();
    return a.exec();
}
