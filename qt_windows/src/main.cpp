#include <QApplication>
#include "ui/main_window.h"

import raii;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return QApplication::exec();
}