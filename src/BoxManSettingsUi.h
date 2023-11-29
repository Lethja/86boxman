#ifndef INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H
#define INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H

#include "boxman.h"

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class BoxManSettingsUi; }
QT_END_NAMESPACE

class BoxManSettingsUi : public QDialog {
Q_OBJECT


public:
    explicit BoxManSettingsUi(QWidget *parent, BoxManager::MainWindow *mainWindow);

    ~BoxManSettingsUi() override;

private:
    Ui::BoxManSettingsUi *ui;
    BoxManager::MainWindow *mainWindow;

    void inline ConnectActions();

    void Apply();

    void SetBinaryPath();

    void SetRomPath();

    void SetVmPath();
};


#endif //INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H
