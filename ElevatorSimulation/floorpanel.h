#ifndef FLOORPANEL_H
#define FLOORPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "floorthread.h"

class FloorPanel : public QWidget
{
    Q_OBJECT
public:
    explicit FloorPanel(QWidget *parent = 0);
    ~FloorPanel();

    FloorThread *getFloor() const;
    Passenger getPassengerGoingUpInFloor(int inFloor);
    Passenger getPassengerGoingDownInFloor(int inFloor);
    int getPassengerGoingUpInFloorSize(int inFloor);
    int getPassengerGoingDownSizeInFloor(int inFloor);

    void setButtonOn(int button, int direction);

protected:

public slots:
    void onUpButtonClicked();
    void onDownButtonClicked();
    void onAddUpButtonPressed();
    void onAddUpButtonReleased();
    void onAddDownButtonPressed();
    void onAddDownButtonReleased();
    void onOutRequestCompleted(int currentFloor, int requestDirection); // from Elevator

    void onNumberUpPassengerchanged(int inFloor, int upNumber); // from elevatorThread & floorThread -> modify number of passengers in each floor
    void onNumberDownPassengerchanged(int inFloor, int downNumber);

    void clearData();

private:
    FloorThread *floor;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout[21];
    QLabel *label[21];
    QPushButton *upButton[21], *downButton[21], *addUpButton[21], *addDownButton[21];
    QList<Passenger> *passengerInFloor;

signals:
    void upButtonClicked(int button);
    void downButtonClicked(int button);

};
#endif // FLOORPANEL_H
