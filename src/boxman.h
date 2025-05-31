#ifndef INC_86_BOX_MAN_H
#define INC_86_BOX_MAN_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QMainWindow>
#include <QProcess>

#include "ui_MainWindow.h"

#include "BoxManSettings.h"

#define IPC_SHOW_SETTINGS "showsettings\n"

namespace BoxManager {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    typedef struct RunningMachine : public QObject {
        QString path;
        QProcess process;
        QLocalServer server;
        QVector<QLocalSocket *> sockets;

        void setupSocketEvents() {
            connect(&server, &QLocalServer::newConnection, this, &RunningMachine::newSocket);
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

        ~RunningMachine() override {
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
