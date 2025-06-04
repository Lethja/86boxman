#ifndef INC_86_BOX_MAN_H
#define INC_86_BOX_MAN_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QProcess>

#include "ui_MainWindow.h"

#include "BoxManSettings.h"

#define IPC_SEND_CTRL_ALT_DEL "coa\n"
#define IPC_SEND_PAUSE "pause\n"
#define IPC_SEND_RESET "reset\n"
#define IPC_SEND_SHOW_SETTINGS "showsettings\n"
#define IPC_SEND_SHUTDOWN "shutdown\n"
#define IPC_SEND_SHUTDOWN_NO_PROMPT "shutdownnoprompt\n"

namespace BoxManager {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    typedef struct RunningMachine : public QObject {
    public:
        QString path;
        QProcess process;
        QLocalServer server;
        QVector<QLocalSocket *> sockets;

        void setupSocketEvents() {
            connect(&server, &QLocalServer::newConnection, this, &RunningMachine::newSocket);
        }

        void setupProcessEvents() {
            connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &RunningMachine::stopped);
        }

        void newSocket() {
            auto socket = server.nextPendingConnection();
            if (socket) {
                //TODO: handle socket disconnection
                //connect(socket, &QLocalSocket::disconnected, &RunningMachine::drop);
                sockets.append(socket);
            }
        }

        void send(const QString& text) {
            for (QLocalSocket *s : sockets) {
                if (s->state() == QLocalSocket::ConnectedState) {
                    QByteArray d = text.toUtf8();
                    s->write(d), s->flush();
                }
            }
        }

    signals:
        void pointerInvalid(BoxManager::RunningMachine *machine);

    public:
        ~RunningMachine() override {
            emit pointerInvalid((RunningMachine *) this);
            if (process.state() != QProcess::ProcessState::NotRunning) {
                //TODO: send "shutdown" signal and wait a while for the virtual machine to shutdown
                process.terminate(), process.close(), process.waitForFinished();
            }
            server.close();
        }

        Q_OBJECT
        void stopped(int exitCode, QProcess::ExitStatus exitStatus) {
            delete this;
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

        void removeMachine(RunningMachine *machine);

        void SendCtrlAltDel() {
            RunningMachine *machine;
            if (IsSelectedMachineRunning(&machine))
                machine->send(IPC_SEND_CTRL_ALT_DEL);
        }

        void SendHardReset() {
            RunningMachine *machine;
            if (IsSelectedMachineRunning(&machine))
                machine->send(IPC_SEND_RESET);
        }

        void SendPowerButton() {
            RunningMachine *machine;
            if (IsSelectedMachineRunning(&machine))
                machine->send(IPC_SEND_SHUTDOWN);
        }

        void SendHardOff() {
            RunningMachine *machine;
            if (IsSelectedMachineRunning(&machine))
                machine->send(IPC_SEND_SHUTDOWN_NO_PROMPT);
        }
    };
} // BoxManager

#endif //INC_86_BOX_MAN_H
