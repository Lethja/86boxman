#ifndef INC_86_BOX_MAN_H
#define INC_86_BOX_MAN_H

#include <QLocalServer>
#include <QMainWindow>
#include <QProcess>

#include "ui_MainWindow.h"

#include "BoxManSettings.h"

namespace BoxManager {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    typedef struct RunningMachine {
        QString path;
        QProcess process;
        QLocalServer server;
        //TODO: Manage connected sockets in QVector<QLocalSocket> sockets; even though there should only be one

        ~RunningMachine() {
            if (process.state() != QProcess::ProcessState::NotRunning) {
                //TODO: send "shutdown" signal and wait a while for the virtual machine to shutdown
                process.terminate(), process.close(), process.waitForFinished();
            }
            server.close();
        }
    } RunningMachine;

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

        BoxManSettings::BoxManSettings settings;

        void PopulateList();

        void ConfigureMachine(const QString &dir);

        void ShowSettingsDialog();

        bool PathsAreOk() const;

    private:
        QVector<RunningMachine *> machines;

        Ui::MainWindow *ui;

        void ConnectActions(Ui_MainWindow *window) const;

        void StartMachine();

        QString GetSelectedMachine();

        bool IsSelectedMachineRunning(RunningMachine **);

        void ConfigureMachineAction();

        RunningMachine *Run86Box(QStringList &args, const QString &wd) const;

        void ShowNewMachineDialog();

        void OpenDirectory();

        RunningMachine *FindRunningMachineByPath(const QString& path);
    };
} // BoxManager

#endif //INC_86_BOX_MAN_H
