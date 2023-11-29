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

        static QString FileDialog86BoxPath(QWidget *parent);

        static QString FileDialogRomPath(QWidget *parent);

        static QString FileDialogVmPath(QWidget *parent);
    };
} // BoxManSettings

#endif //INC_86_BOX_MAN_SETTINGS_H
