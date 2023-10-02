#include "boxman.h"
#include "ui_MainWindow.h"
#include "BoxManSettingsUi.h"
#include "CreateMachine.h"

#include <QProcess>
#include <QStandardPaths>
#include <QStringListModel>

namespace fs = std::filesystem;

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
        fs::path cfg = fs::path(dir.toStdString());
        cfg.append("86box.cfg");
        if (!(exists(cfg) && file_size(cfg) > 0)) {
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
        auto program = QString(settings.Box86BinaryPath.c_str());
        args << "-R" << QString(settings.RomDirectory.c_str());

        auto *process = new QProcess(nullptr);

        if (is_directory(fs::path(wd.toStdString())))
            process->setWorkingDirectory(wd);

        process->start(program, args);
    }

    QString MainWindow::GetSelectedMachine() {
        int selectedRow = ui->MachineList->selectionModel()->currentIndex().row();
        QAbstractItemModel *model = ui->MachineList->model();
        fs::path path = fs::path(settings.MachineDirectory);
        path.append(model->index(selectedRow, 0).data().toString().toStdString());
        return {path.c_str()};
    }

    void MainWindow::PopulateList() {
        QStringList list;
        auto *model = new QStringListModel(this);
        for (const auto &machines: settings.GetAllMachinePaths()) {
            auto str = QString(machines.filename().c_str());
            list.append(str);
        }

        model->setStringList(list);
        ui->MachineList->setModel(model);
    }
}
