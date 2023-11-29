#include "BoxManSettingsUi.h"
#include "ui_BoxManSettingsUi.h"

#include <QFileDialog>

BoxManSettingsUi::BoxManSettingsUi(QWidget *parent, BoxManager::MainWindow *mainWindow) : QDialog(parent),
                                                                                          ui(new Ui::BoxManSettingsUi) {
    this->mainWindow = mainWindow;
    ui->setupUi(this);
    ConnectActions();
}

void inline BoxManSettingsUi::ConnectActions() {
    ui->box86BinaryPath->setText(mainWindow->settings.Box86BinaryPath);
    ui->machinePath->setText(mainWindow->settings.MachineDirectory);
    ui->romPath->setText(mainWindow->settings.RomDirectory);

    connect(ui->applyButton, &QPushButton::released, this, &BoxManSettingsUi::Apply);
    connect(ui->box86BinaryPushButton, &QPushButton::released, this, &BoxManSettingsUi::SetBinaryPath);
    connect(ui->romPushButton, &QPushButton::released, this, &BoxManSettingsUi::SetRomPath);
    connect(ui->machinePushButton, &QPushButton::released, this, &BoxManSettingsUi::SetVmPath);
}

void BoxManSettingsUi::SetBinaryPath() {
    QString binary = QFileDialog::getOpenFileName(this, "Select 86Box binary");
    ui->box86BinaryPath->setText(binary);
}

void BoxManSettingsUi::SetVmPath() {
    QString directory = QFileDialog::getExistingDirectory(this, "Select VM directory");
    ui->box86BinaryPath->setText(directory);
}

void BoxManSettingsUi::SetRomPath() {
    QString directory = QFileDialog::getExistingDirectory(this, "Select 86Box ROM directory");
    ui->box86BinaryPath->setText(directory);
}

void BoxManSettingsUi::Apply() {
    BoxManSettings::BoxManSettings *setting;
    setting = &mainWindow->settings;
    setting->Box86BinaryPath = ui->box86BinaryPath->text();
    setting->MachineDirectory = ui->machinePath->text();
    setting->RomDirectory = ui->romPath->text();

    setting->WriteIni();
    mainWindow->PopulateList();

    this->close();
}

BoxManSettingsUi::~BoxManSettingsUi() {
    delete ui;
}
