#include "elevatormanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElevatorManager w;
    w.show();
    w.setAttribute(Qt::WA_DeleteOnClose);

    return a.exec();
}
