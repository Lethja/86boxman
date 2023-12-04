#include "BoxManSettingsUi.h"
#include "ui_BoxManSettingsUi.h"

BoxManSettingsUi::BoxManSettingsUi(QWidget *parent, BoxManager::MainWindow *mainWindow) : QDialog(parent),
                                                                                          ui(new Ui::BoxManSettingsUi) {
    this->mainWindow = mainWindow;
    ui->setupUi(this);
    ConnectActions();
}

void inline BoxManSettingsUi::ConnectActions() {
    ui->binPath->setText(mainWindow->settings.binPath);
    ui->vmPath->setText(mainWindow->settings.vmPath);
    ui->romPath->setText(mainWindow->settings.romPath);

    connect(ui->applyButton, &QPushButton::released, this, &BoxManSettingsUi::Apply);
    connect(ui->binPathBrowse, &QPushButton::released, this, &BoxManSettingsUi::SetBinaryPath);
    connect(ui->romPathBrowse, &QPushButton::released, this, &BoxManSettingsUi::SetRomPath);
    connect(ui->vmPathBrowse, &QPushButton::released, this, &BoxManSettingsUi::SetVmPath);
}

void BoxManSettingsUi::SetBinaryPath() {
    QString path = BoxManSettings::BoxManSettings::FileDialogPathBin(this);
    if (path.length())
        ui->binPath->setText(path);
}

void BoxManSettingsUi::SetRomPath() {
    QString path = BoxManSettings::BoxManSettings::FileDialogPathRom(this);
    if (path.length())
        ui->romPath->setText(path);
}

void BoxManSettingsUi::SetVmPath() {
    QString path = BoxManSettings::BoxManSettings::FileDialogPathVm(this);
    if (path.length())
        ui->vmPath->setText(path);
}

void BoxManSettingsUi::Apply() {
    BoxManSettings::BoxManSettings *setting;
    setting = &mainWindow->settings;
    setting->binPath = ui->binPath->text();
    setting->vmPath = ui->vmPath->text();
    setting->romPath = ui->romPath->text();

    if (mainWindow->PathsAreOk()) {
        setting->WriteIni();
        mainWindow->PopulateList();
        this->close();
    }
}

BoxManSettingsUi::~BoxManSettingsUi() {
    delete ui;
}
