#include "TabWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TabWidget tabWidget;
    tabWidget.resize(640, 480);
    tabWidget.show();
    return app.exec();
}
