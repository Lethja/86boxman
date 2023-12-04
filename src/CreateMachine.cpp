#include <fstream>
#include "CreateMachine.h"
#include "ui_CreateMachine.h"

#include <QDir>
#include <QMessageBox>


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
    if (ui->newMachineName->text().isEmpty() || ui->newMachineName->text().contains(QRegExp("[\\/\\\n\r\t]"))) {
        QMessageBox messageBox(QMessageBox::Icon::Critical, QString("Invalid Machine Name"),
                               QString("Machine name must be a valid folder name"), QMessageBox::StandardButton::Ok,
                               mainWindow);
        messageBox.exec();
        return;
    }

    QString newPath(mainWindow->settings.vmPath + QDir::separator() + ui->newMachineName->text());
    if (!QDir().mkpath(newPath)) {
        QMessageBox messageBox(QMessageBox::Icon::Critical, QString("Configuration write error"),
                               QString("Unable to create machine folder '" + newPath + "'"),
                               QMessageBox::StandardButton::Ok, mainWindow);
        return;
    }
    QString cfgPath = newPath + QDir::separator() + QString("86box.cfg");
    QFile cfgFile(cfgPath);

    if (!cfgFile.open(QIODevice::ReadWrite)) {
        QMessageBox messageBox(QMessageBox::Icon::Critical, QString("Configuration write error"),
                               QString("Unable to create machine configuration file '" + cfgPath + "'"),
                               QMessageBox::StandardButton::Ok, mainWindow);
        return;
    }

    cfgFile.flush();
    cfgFile.close();

    if (ui->configureNowCheckBox->isChecked()) {
        mainWindow->ConfigureMachine(QString(newPath));
    }

    mainWindow->PopulateList();
    this->close();
}

CreateMachine::~CreateMachine() {
    delete ui;
}
