#include <fstream>
#include "CreateMachine.h"
#include "ui_CreateMachine.h"


CreateMachine::CreateMachine(QWidget *parent, BoxManager::MainWindow *mainWindow) : QDialog(parent),
                                                                                    ui(new Ui::CreateMachine) {
    ui->setupUi(this);
    this->mainWindow = mainWindow;
    ConnectActions();
}

void CreateMachine::ConnectActions() {
    connect(ui->createPushButton, &QPushButton::released, this, &CreateMachine::Create);
}

void CreateMachine::Create() {
    if (ui->newMachineName->text().isEmpty())
        return;

    fs::path newPath = fs::path(mainWindow->settings.MachineDirectory).append(ui->newMachineName->text().toStdString());

    fs::create_directories(newPath);
    fs::path cfgPath = fs::path(newPath).append("86box.cfg");
    std::fstream st;
    st.open(cfgPath, std::ios::out | std::ios::app);

    if (ui->configureNowCheckBox->isChecked()) {
        mainWindow->ConfigureMachine(QString(newPath.c_str()));
    }

    mainWindow->PopulateList();
    this->close();
}

CreateMachine::~CreateMachine() {
    delete ui;
}
