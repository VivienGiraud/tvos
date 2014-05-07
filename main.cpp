#include "player.h"
#include "FenCodeGenere.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    player x;
    x.showFullScreen();

    return a.exec();
}
