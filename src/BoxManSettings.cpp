#include "BoxManSettings.h"

#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QSettings>

namespace BoxManSettings {

    const char *INI_PATH_BIN = "path/binary", *INI_PATH_ROM = "path/roms", *INI_PATH_VMP = "path/vms";

    BoxManSettings::BoxManSettings() {
        QString ApplicationDataPath = QString(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator());

        QString BoxIni = GetIniPath();
        QString MachinePath = QString(ApplicationDataPath).append("vms");
        QString RomPath = QString(ApplicationDataPath).append("roms");

        const char *DEFAULT_BIN = "86Box";

        QSettings s(BoxIni, QSettings::IniFormat);


        if (!QFile::exists(BoxIni)) {
            s.setValue(INI_PATH_BIN, DEFAULT_BIN);
            s.setValue(INI_PATH_ROM, RomPath);
            s.setValue(INI_PATH_VMP, MachinePath);

            QDir().mkpath(MachinePath);
            QDir().mkpath(RomPath);
        }

        Box86BinaryPath = QString(s.value(INI_PATH_BIN, DEFAULT_BIN).toString());
        RomDirectory = QString(s.value(INI_PATH_ROM, RomPath).toString());
        MachineDirectory = QString(s.value(INI_PATH_VMP, MachinePath).toString());
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
        s.setValue(INI_PATH_BIN, Box86BinaryPath + QDir::separator());
        s.setValue(INI_PATH_ROM, RomDirectory + QDir::separator());
        s.setValue(INI_PATH_VMP, MachineDirectory + QDir::separator());
    }

    QVector<QString> BoxManSettings::GetAllMachinePaths() const {
        QVector<QString> machines;
        if (QDir(MachineDirectory).exists()) {
            QDirIterator it(MachineDirectory);
            while (it.hasNext()) {
                QString cfg = QDir::cleanPath(it.next() + QDir::separator() + "86box.cfg");
                if (QFile::exists(cfg))
                    machines.push_back(QFileInfo(cfg).dir().dirName());
            }
        }
        return machines;
    }
} // BoxManSettings
