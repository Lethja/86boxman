#ifndef INC_86_BOX_MAN_CREATE_MACHINE_H
#define INC_86_BOX_MAN_CREATE_MACHINE_H

#include "boxman.h"

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class CreateMachine; }
QT_END_NAMESPACE

class CreateMachine : public QDialog {
Q_OBJECT

public:
    explicit CreateMachine(QWidget *parent, BoxManager::MainWindow *mainWindow);

    ~CreateMachine() override;

private:
    Ui::CreateMachine *ui;

    BoxManager::MainWindow *mainWindow;

    void ConnectActions();

    void Create();
};


#endif //INC_86_BOX_MAN_CREATE_MACHINE_H
