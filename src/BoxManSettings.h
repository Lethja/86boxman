#ifndef INC_86_BOX_MAN_SETTINGS_H
#define INC_86_BOX_MAN_SETTINGS_H

#include <QString>
#include <QWidget>

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

        static QString FileDialogPathBin(QWidget *parent);

        static QString FileDialogPathRom(QWidget *parent);

        static QString FileDialogPathVm(QWidget *parent);

        bool PathBinOk() const;

        bool PathRomOk() const;

        bool PathVmOk() const;
    };
} // BoxManSettings

#endif //INC_86_BOX_MAN_SETTINGS_H
