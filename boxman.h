#ifndef INC_86_BOX_MAN_H
#define INC_86_BOX_MAN_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "BoxManSettings.h"

namespace BoxManager {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainWindow *ui;

        void PopulateList(BoxManager::Ui_MainWindow *window);
        BoxManSettings::BoxManSettings settings;

        void ConnectButtons(Ui_MainWindow *window) const;

        void StartMachine();

        QString GetSelectedMachine();

        void ConfigureMachineAction();

        void Run86Box(QStringList &args, const QString& wd) const;

        void ConfigureMachine(const QString &dir);
    };
} // BoxManager

#endif //INC_86_BOX_MAN_H
