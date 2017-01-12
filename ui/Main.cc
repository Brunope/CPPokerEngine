#include <QApplication>
#include <QMainWindow>
#include "HumanActor.h"
#include "Hand.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    w.show();

    // see if this links - nope
    HumanActor h;
    Hand hand;
    
    return a.exec();
}
