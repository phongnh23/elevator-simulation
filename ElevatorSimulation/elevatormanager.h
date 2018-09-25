#ifndef ELEVATORMANAGER_H
#define ELEVATORMANAGER_H

#include <QMainWindow>
#include <QLabel>
#include "elevator.h"
#include "floorpanel.h"
#include "settingdialog.h"

namespace Ui {
class ElevatorManager;
}

class ElevatorManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElevatorManager(QWidget *parent = 0);
    ~ElevatorManager();

public slots:
    void onUpButtonClicked(int button);
    void onDownButtonClicked(int button);
    void onInitialDataSent(int *elevatorFloor, QList<Passenger> *passengerInFloor);

protected:
    int calculateDistance(int elevatorIndex, int requestFloor, Direction requestDirection, bool maxDistance); // khoang cach lon nhat de 1 elevator toi tang can toi
    void delegate(int floorRequest, Direction directionRequest);
    int speed;
    QMutex mutex;

private:
    Ui::ElevatorManager *ui;

    int numElevator;

    Elevator *elevator[4];
    FloorPanel *floorPanel;
    SettingDialog *settingDialog;
    QLabel *label;

signals:
    void elevatorDelegated(int pickedElevator, int floorRequest, Direction directionRequest);
private slots:
    void on_actionExit_triggered();
    void on_actionSetting_triggered();
    void on_actionSpeed_Up_triggered();
    void on_actionSpeed_Down_triggered();
};

#endif // ELEVATORMANAGER_H
