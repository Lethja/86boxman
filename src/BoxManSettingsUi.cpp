#include "BoxManSettingsUi.h"
#include "ui_BoxManSettingsUi.h"

BoxManSettingsUi::BoxManSettingsUi(QWidget *parent, BoxManager::MainWindow *mainWindow) : QDialog(parent),
                                                                                          ui(new Ui::BoxManSettingsUi) {
    this->mainWindow = mainWindow;
    ui->setupUi(this);
    ConnectActions();
}

void BoxManSettingsUi::ConnectActions() {
    ui->box86BinaryPath->setText(mainWindow->settings.Box86BinaryPath);
    ui->machinePath->setText(mainWindow->settings.MachineDirectory);
    ui->romPath->setText(mainWindow->settings.RomDirectory);

    connect(ui->applyButton, &QPushButton::released, this, &BoxManSettingsUi::Apply);
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
