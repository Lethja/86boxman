#include "boxman.h"
#include "ui_MainWindow.h"
#include "BoxManSettingsUi.h"
#include "CreateMachine.h"

#include <QDesktopServices>
#include <QDir>
#include <QLocalSocket>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStringListModel>
#include <QUrl>

namespace BoxManager {

    const QString sockName = "ipc.sock";

    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        ConnectActions(ui);
        show();

        if (!PathsAreOk())
            ShowSettingsDialog();

        PopulateList();
    }

    MainWindow::~MainWindow() {
        for (auto machine : machines)
            delete machine;

        delete ui;
    }

    void MainWindow::ShowNewMachineDialog() {
        auto newMachineUi = new CreateMachine(ui->centralwidget, this);
        newMachineUi->exec();
    }

    void MainWindow::ShowSettingsDialog() {
        auto settingsUi = new BoxManSettingsUi(ui->centralwidget, this);
        settingsUi->exec();
    }

    void MainWindow::ConnectActions(Ui_MainWindow *window) const {
        connect(window->actionExit, &QAction::triggered, this, &QApplication::quit);
        connect(window->actionRefresh_Machine_List, &QAction::triggered, this, &MainWindow::PopulateList);
        connect(window->actionStart_Machine, &QAction::triggered, this, &MainWindow::StartMachine);
        connect(window->actionConfigure_Machine, &QAction::triggered, this, &MainWindow::ConfigureMachineAction);
        connect(window->MachineList, &QListView::doubleClicked, this, &MainWindow::StartMachine);
        connect(window->actionSettings, &QAction::triggered, this, &MainWindow::ShowSettingsDialog);
        connect(window->actionNew_Machine, &QAction::triggered, this, &MainWindow::ShowNewMachineDialog);
        connect(window->actionOpen_Directory, &QAction::triggered, this, &MainWindow::OpenDirectory);
    }

    void MainWindow::StartMachine() {
        if (!PathsAreOk())
            return;

        QString dir = GetSelectedMachine();
        QString cfg = QDir::cleanPath(dir + QDir::separator() + "86box.cfg");
        QFile cfgFile(cfg);

        if (!(QFile::exists(cfg) && cfgFile.size() > 0)) {
            ConfigureMachine(dir);
            return;
        }

        RunningMachine *machine;

        if (IsSelectedMachineRunning(&machine)) {
            QMessageBox msgBox;
            msgBox.setText("This machine is already running.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setTextFormat(Qt::TextFormat::RichText);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        } else {
            QStringList args;

            args << "-P" << dir, machine = Run86Box(args, dir);
            if (machine)
                machines.append(machine);
        }
    }

    void MainWindow::ConfigureMachine(const QString &dir) {
        if (!PathsAreOk())
            return;

        RunningMachine *machine;

        if (IsSelectedMachineRunning(&machine)) {
            //TODO: write 'showsettings' to the socket here.
            QMessageBox msgBox;
            msgBox.setText("This machine is already running.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setTextFormat(Qt::TextFormat::RichText);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        } else {
            QStringList args;

            args << "-S" << "-P" << dir, machine = Run86Box(args, dir);
            if (machine)
                machines.append(machine);
        }
    }

    void MainWindow::ConfigureMachineAction() {
        ConfigureMachine(GetSelectedMachine());
    }

    void MainWindow::OpenDirectory() {
        QString name = GetSelectedMachine();
        if (!QDir().exists(name))
            return;

        QUrl url = QUrl::fromLocalFile(QDir().absoluteFilePath(name));
        if (url.isValid())
            QDesktopServices::openUrl(url);
    }

    RunningMachine *MainWindow::Run86Box(QStringList &args, const QString &wd) const {
        if (!PathsAreOk())
            return nullptr;

        auto *machine = new RunningMachine;
        QString program = QString(settings.binPath);
        args << "-R" << QString(settings.romPath);

        if (QDir().exists(wd))
            machine->process.setWorkingDirectory(wd);

        machine->path = wd;

        QString socket = wd + QDir::separator() + sockName;
        if (machine->server.listen(socket)) {
            QStringList env = machine->process.environment();
            env.append("86BOX_MANAGER_SOCKET=" + socket);
        }

        machine->process.start(program, args);
        //TODO: connect signals in a way that makes sense to the RunningMachine struct
        //connect(&machine->process, SIGNAL(finished(int,QProcess::ExitStatus)), &machine->process, SLOT(deleteLater()));

        return machine;
    }

    QString MainWindow::GetSelectedMachine() {
        int selectedRow = ui->MachineList->selectionModel()->currentIndex().row();
        QAbstractItemModel *model = ui->MachineList->model();
        QString path(settings.vmPath + QDir::separator() + QString(model->index(selectedRow, 0).data().toString()));
        return path;
    }

    void MainWindow::PopulateList() {
        QStringList list;
        auto *model = new QStringListModel(this);
        for (const auto &machine: settings.GetAllMachinePaths()) {
            auto str = QDir(machine).dirName();
            list.append(str);
        }

        model->setStringList(list);
        ui->MachineList->setModel(model);
    }

    bool MainWindow::PathsAreOk() const {
        size_t i = 0;
        QString list;

        if (!settings.PathBinOk())
            list += "<li>86Box (" + settings.binPath + ") isn't a executable binary</li>", ++i;

        if (!settings.PathVmOk())
            list += "<li>Machine directory (" + settings.vmPath + ") isn't a writable directory</li>", ++i;

        if (!settings.PathRomOk())
            list += "<li>ROM directory (" + settings.romPath + ") doesn't contain ROM set files</li>", ++i;

        if (!list.length())
            return true;

        QString title = i == 1 ? "Invalid Path" : "Invalid Paths";

        QString header =
                i == 1 ? "<p>Issue with configured path:</p>" : "<p>Multiple issues with configured paths:</p><ul>";

        QString footer = "</ul><p>To get started using 86Box visit <a href='https://86box.net/'>86box.net</a>. "
                         "If you already have 86Box configured check all paths are set correctly.";

        list = header + "<ul>" + list + footer;

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(title);
        msgBox.setTextFormat(Qt::TextFormat::RichText);
        msgBox.setText(list);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return false;
    }

    RunningMachine *MainWindow::FindRunningMachineByPath(const QString& path) {
        for (RunningMachine *machine : machines)
            if (machine->path.compare(path) == 0)
                return machine;

        return nullptr;
    }

    bool MainWindow::IsSelectedMachineRunning(RunningMachine **machine) {
        QString path = GetSelectedMachine();
        RunningMachine *m = FindRunningMachineByPath(path);

        if (!m)
            return false;

        if (machine)
            *machine = m;

        return m->process.state() == QProcess::ProcessState::Running;
    }
}
