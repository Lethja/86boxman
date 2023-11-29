#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>

#include "boxman.h"

inline void StartupSanityCheck(BoxManager::MainWindow &window) {
    QFileInfo bin(window.settings.Box86BinaryPath), rom(window.settings.RomDirectory), vm(
            window.settings.MachineDirectory);

    if (!(bin.isFile() && bin.isExecutable()) || !rom.isDir() || !vm.isDir()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Invalid Path(s)");
        msgBox.setText("One or more of the launchers paths appear to be invalid."
                       "Please double check each path");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        window.show();
        window.ShowSettingsDialog();
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("86BoxMan");
    QApplication a(argc, argv);
    BoxManager::MainWindow window;
    StartupSanityCheck(window);
    window.show();
    return QApplication::exec();
}
