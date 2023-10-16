#ifndef INC_86_BOX_MAN_SETTINGS_H
#define INC_86_BOX_MAN_SETTINGS_H

#include <QString>

namespace BoxManSettings {
    class BoxManSettings {

    public:
        BoxManSettings();

        QString Box86BinaryPath;
        QString MachineDirectory;
        QString RomDirectory;

        QVector<QString> GetAllMachinePaths() const;

        static QString GetIniPath();

        void WriteIni() const;
    };
} // BoxManSettings

#endif //INC_86_BOX_MAN_SETTINGS_H
