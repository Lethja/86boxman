#include "BoxManSettings.h"

#include <QStandardPaths>
#include <QSettings>

namespace BoxManSettings {

    const char *INI_PATH_BIN = "path/binary", *INI_PATH_ROM = "path/roms", *INI_PATH_VMP = "path/vms";

    BoxManSettings::BoxManSettings() {
        fs::path ApplicationDataPath = fs::path(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString());

        fs::path BoxIni = GetIniPath();
        fs::path MachinePath = fs::path(ApplicationDataPath).append("vms");
        fs::path RomPath = fs::path(ApplicationDataPath).append("roms");

        const char *DEFAULT_BIN = "86Box";

        QSettings s(QString::fromStdString(BoxIni), QSettings::IniFormat);

        if (!fs::exists(BoxIni)) {
            s.setValue(INI_PATH_BIN, DEFAULT_BIN);
            s.setValue(INI_PATH_ROM, RomPath.c_str());
            s.setValue(INI_PATH_VMP, MachinePath.c_str());

            fs::create_directories(MachinePath);
            fs::create_directories(RomPath);
        }

        Box86BinaryPath = fs::path(s.value(INI_PATH_BIN, DEFAULT_BIN).toString().toStdString());
        RomDirectory = fs::path(s.value(INI_PATH_ROM, RomPath.c_str()).toString().toStdString());
        MachineDirectory = fs::path(s.value(INI_PATH_VMP, MachinePath.c_str()).toString().toStdString());
    }

    fs::path BoxManSettings::GetIniPath() {
        fs::path ApplicationDataPath = fs::path(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString());
        fs::create_directories(ApplicationDataPath);

        fs::path BoxIni = fs::path(ApplicationDataPath).append("boxman.ini");
        return BoxIni;
    }

    void BoxManSettings::WriteIni() const {
        fs::path BoxIni = GetIniPath();

        QSettings s(QString::fromStdString(BoxIni), QSettings::IniFormat);
        s.setValue(INI_PATH_BIN, Box86BinaryPath.c_str());
        s.setValue(INI_PATH_ROM, RomDirectory.c_str());
        s.setValue(INI_PATH_VMP, MachineDirectory.c_str());
    }

    std::vector<fs::path> BoxManSettings::GetAllMachinePaths() const {
        std::vector<fs::path> machines;
        if (is_directory(MachineDirectory)) {
            for (const auto &path: fs::directory_iterator(MachineDirectory)) {
                fs::path cfg = fs::path(path.path());
                cfg.append("86box.cfg");
                if (exists(cfg))
                    machines.push_back(path.path());
            }
        }
        return machines;
    }
} // BoxManSettings
