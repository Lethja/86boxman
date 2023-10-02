#include "boxman.h"
#include "ui_MainWindow.h"

#include <QProcess>
#include <QStandardPaths>
#include <QStringListModel>

namespace fs = std::filesystem;

namespace BoxManager {
    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        PopulateList(ui);
        ConnectButtons(ui);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::ConnectButtons(Ui_MainWindow* window) const {
        connect(window->StartButton, &QPushButton::released, this, &MainWindow::StartMachine);
        connect(window->EditButton, &QPushButton::released, this, &MainWindow::ConfigureMachineAction);
    }

    void MainWindow::StartMachine() {
        QString dir = GetSelectedMachine();
        fs::path cfg = fs::path(dir.toStdString());
        cfg.append("86box.cfg");
        if(!(exists(cfg) && file_size(cfg) > 0)) {
            ConfigureMachine(dir);
            return;
        }

        QStringList args;
        args << "-P" << dir;
        Run86Box(args, dir);
    }

    void MainWindow::ConfigureMachine(const QString& dir) {
        QStringList args;
        args << "-S" << "-P" << dir;
        Run86Box(args, dir);
    }

    void MainWindow::ConfigureMachineAction() {
        ConfigureMachine(GetSelectedMachine());
    }

    void MainWindow::Run86Box(QStringList& args, const QString& wd) const {
        auto program = QString(settings.Box86BinaryPath.c_str());
        args << "-R" << QString(settings.RomDirectory.c_str());

        auto *process = new QProcess(nullptr);

        if(wd != nullptr)
            process->setWorkingDirectory(wd);

        process->start(program, args);
    }

    QString MainWindow::GetSelectedMachine() {
        int selectedRow = ui->MachineList->selectionModel()->currentIndex().row();
        QAbstractItemModel* model = ui->MachineList->model();
        return model->index(selectedRow, 0).data().toString();
    }

    void MainWindow::PopulateList(Ui_MainWindow* window) {
        QStringList list;
        auto *model = new QStringListModel(this);
        for(const auto& machines : settings.GetAllMachineConfigs()) {
            auto str = QString(machines.c_str());
            list.append(str);
        }

        model->setStringList(list);
        window->MachineList->setModel(model);
    }
}
