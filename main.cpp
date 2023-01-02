#include "widget.h"

#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
	w.setWindowTitle("百度AI翻译");
    w.show();
    return a.exec();
}
