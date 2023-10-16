#include "boxman.h"
#include "ui_MainWindow.h"
#include "BoxManSettingsUi.h"
#include "CreateMachine.h"

#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QStringListModel>

namespace BoxManager {
    MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        PopulateList();
        ConnectActions(ui);
    }

    MainWindow::~MainWindow() {
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
    }

    void MainWindow::StartMachine() {
        QString dir = GetSelectedMachine();
        QString cfg = QDir::cleanPath(dir + QDir::separator() + "86box.cfg");
        QFile cfgFile(cfg);

        if (!(QFile::exists(cfg) && cfgFile.size() > 0)) {
            ConfigureMachine(dir);
            return;
        }

        QStringList args;
        args << "-P" << dir;
        Run86Box(args, dir);
    }

    void MainWindow::ConfigureMachine(const QString &dir) {
        QStringList args;
        args << "-S" << "-P" << dir;
        Run86Box(args, dir);
    }

    void MainWindow::ConfigureMachineAction() {
        ConfigureMachine(GetSelectedMachine());
    }

    void MainWindow::Run86Box(QStringList &args, const QString &wd) const {
        auto program = QString(settings.Box86BinaryPath);
        args << "-R" << QString(settings.RomDirectory);

        auto *process = new QProcess(nullptr);

        if (QDir().exists(wd))
            process->setWorkingDirectory(wd);

        process->start(program, args);
        connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), process, SLOT(deleteLater()));
    }

    QString MainWindow::GetSelectedMachine() {
        int selectedRow = ui->MachineList->selectionModel()->currentIndex().row();
        QAbstractItemModel *model = ui->MachineList->model();
        QString path(settings.MachineDirectory + QDir::separator() +
                     QString(model->index(selectedRow, 0).data().toString()));
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
}
