#include <QApplication>
#include "ui_main_window.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;

    Ui_MainWindow ui{};
    ui.setupUi(&window);
    window.show();

    return QApplication::exec();
}