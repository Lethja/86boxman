#ifndef INC_86_BOX_MAN_SETTINGS_H
#define INC_86_BOX_MAN_SETTINGS_H

#include <filesystem>
#include <QDialog>

namespace fs = std::filesystem;

namespace BoxManSettings {
    class BoxManSettings {

    public:
        BoxManSettings();

        fs::path Box86BinaryPath;
        fs::path MachineDirectory;
        fs::path RomDirectory;

        [[nodiscard]] std::vector<fs::path> GetAllMachineConfigs() const;
    };
} // BoxManSettings

#endif //INC_86_BOX_MAN_SETTINGS_H
