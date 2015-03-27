#include "SetParameters.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SetParameters setParameter;
    setParameter.resize(800,600);
    setParameter.show();
    
    return a.exec();
}
