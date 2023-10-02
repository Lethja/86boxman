#include <QApplication>
#include "boxman.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("86BoxMan");
    QApplication a(argc, argv);
    BoxManager::MainWindow window;
    window.show();
    return QApplication::exec();
}
