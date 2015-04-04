#include "TabWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TabWidget tabWidget;
    tabWidget.resize(1220, 550);
    tabWidget.show();
    return app.exec();
}
