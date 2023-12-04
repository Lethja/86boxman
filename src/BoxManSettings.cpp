#include "BoxManSettings.h"

#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QSettings>
#include <QFileDialog>

namespace BoxManSettings {

    const char *INI_PATH_BIN = "path/bin", *INI_PATH_ROM = "path/roms", *INI_PATH_VMP = "path/machines";

    BoxManSettings::BoxManSettings() {
        QString ApplicationDataPath = QString(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator());

        QString BoxIni = GetIniPath();
        QString MachinePath = QString(ApplicationDataPath).append("machines");
        QString RomPath = QString(ApplicationDataPath).append("roms");

        const char *DEFAULT_BIN = "86Box"
#ifdef _WIN32
        ".exe"
#endif
        ;

        QSettings s(BoxIni, QSettings::IniFormat);

        if (!QFile::exists(BoxIni)) {
            s.setValue(INI_PATH_BIN, DEFAULT_BIN);
            s.setValue(INI_PATH_ROM, RomPath);
            s.setValue(INI_PATH_VMP, MachinePath);

            QDir().mkpath(MachinePath);
            QDir().mkpath(RomPath);
        }

        binPath = QString(s.value(INI_PATH_BIN, DEFAULT_BIN).toString());
        romPath = QString(s.value(INI_PATH_ROM, RomPath).toString());
        vmPath = QString(s.value(INI_PATH_VMP, MachinePath).toString());
    }

    QString BoxManSettings::GetIniPath() {
        QString ApplicationDataPath = QString(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator());
        QDir().mkpath(ApplicationDataPath);

        QString BoxIni = QString(ApplicationDataPath).append("boxman.ini");
        return BoxIni;
    }

    void BoxManSettings::WriteIni() const {
        QString BoxIni = GetIniPath();

        QSettings s(BoxIni, QSettings::IniFormat);
        s.setValue(INI_PATH_BIN, binPath);
        s.setValue(INI_PATH_ROM, romPath);
        s.setValue(INI_PATH_VMP, vmPath);
    }

    QVector<QString> BoxManSettings::GetAllMachinePaths() const {
        QVector<QString> machines;
        if (QDir(vmPath).exists()) {
            QDirIterator it(vmPath);
            while (it.hasNext()) {
                QString cfg = QDir::cleanPath(it.next() + QDir::separator() + "86box.cfg");
                if (QFile::exists(cfg))
                    machines.push_back(QFileInfo(cfg).dir().dirName());
            }
        }
        return machines;
    }

    bool BoxManSettings::PathBinOk() const {
        QFileInfo fi(binPath);
        return fi.isFile() && fi.isExecutable();
    }

    bool BoxManSettings::PathRomOk() const {
        QFileInfo fi(romPath);
        if (fi.isDir()) {
            fi = QFileInfo(romPath + "/machines");
            if (fi.isDir())
                return true;
        }
        return false;
    }

    bool BoxManSettings::PathVmOk() const {
        QFileInfo fi(vmPath);
        return fi.isDir() && fi.isWritable();
    }

    QString BoxManSettings::FileDialogPathBin(QWidget *parent = nullptr) {
        return QFileDialog::getOpenFileName(parent, "Select 86Box Path");
    }

    QString BoxManSettings::FileDialogPathRom(QWidget *parent = nullptr) {
        return QFileDialog::getExistingDirectory(parent, "Select ROM Path");
    }

    QString BoxManSettings::FileDialogPathVm(QWidget *parent = nullptr) {
        return QFileDialog::getExistingDirectory(parent, "Select Machines Path");
    }
} // BoxManSettings
