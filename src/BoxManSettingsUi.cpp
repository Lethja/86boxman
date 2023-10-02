#include "BoxManSettingsUi.h"
#include "ui_BoxManSettingsUi.h"

BoxManSettingsUi::BoxManSettingsUi(QWidget *parent, BoxManager::MainWindow *mainWindow) : QDialog(parent),
                                                                                          ui(new Ui::BoxManSettingsUi) {
    this->mainWindow = mainWindow;
    ui->setupUi(this);
    ConnectActions();
}

void BoxManSettingsUi::ConnectActions() {
    ui->box86BinaryPath->setText(mainWindow->settings.Box86BinaryPath.c_str());
    ui->machinePath->setText(mainWindow->settings.MachineDirectory.c_str());
    ui->romPath->setText(mainWindow->settings.RomDirectory.c_str());

    connect(ui->applyButton, &QPushButton::released, this, &BoxManSettingsUi::Apply);
}

void BoxManSettingsUi::Apply() {
    BoxManSettings::BoxManSettings *setting;
    setting = &mainWindow->settings;
    setting->Box86BinaryPath = ui->box86BinaryPath->text().toStdString();
    setting->MachineDirectory = ui->machinePath->text().toStdString();
    setting->RomDirectory = ui->romPath->text().toStdString();

    setting->WriteIni();
    mainWindow->PopulateList();

    this->close();
}

BoxManSettingsUi::~BoxManSettingsUi() {
    delete ui;
}
