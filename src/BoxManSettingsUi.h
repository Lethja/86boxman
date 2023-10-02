#ifndef INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H
#define INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H

#include <QDialog>
#include "boxman.h"


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

    void ConnectActions();

    void Apply();
};


#endif //INC_86_BOX_MAN_BOX_MAN_SETTINGS_UI_H
