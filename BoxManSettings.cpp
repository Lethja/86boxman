#include "BoxManSettings.h"

#include <QStandardPaths>
#include <QSettings>

namespace BoxManSettings {
    BoxManSettings::BoxManSettings() {
        fs::path ApplicationDataPath = fs::path(
                QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString());
        fs::create_directories(ApplicationDataPath);

        fs::path BoxIni = fs::path(ApplicationDataPath).append("boxman.ini");
        fs::path MachinePath = fs::path(ApplicationDataPath).append("vms");
        fs::path RomPath = fs::path(ApplicationDataPath).append("roms");

        const char *DEFAULT_BIN = "86Box";
        const char *INI_PATH_BIN = "path/binary", *INI_PATH_ROM = "path/roms", *INI_PATH_VMP = "path/vms";

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

    std::vector<fs::path> BoxManSettings::GetAllMachineConfigs() const {
        std::vector<fs::path> machines;
        for (const auto &path: fs::directory_iterator(MachineDirectory)) {
            fs::path cfg = fs::path(path.path());
            cfg.append("86box.cfg");
            if (exists(cfg))
                machines.push_back(path.path());
        }
        return machines;
    }
} // BoxManSettings
