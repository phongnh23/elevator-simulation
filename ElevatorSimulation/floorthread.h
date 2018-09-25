#ifndef FLOORTHREAD_H
#define FLOORTHREAD_H

#include <QDebug>
#include <QThread>
#include "enums.h"
#include "passenger.h"

class FloorThread : public QThread
{
    Q_OBJECT

public:
    explicit FloorThread(QObject *parent = NULL);
    ~FloorThread();
    bool runFlag;

    void addPassenger(int inFloor);

    void addUpPassenger(int inFloor);
    void addDownPassenger(int inFloor);

    Passenger getPassengerGoingUpInFloor(int floor) const;

    int getPassengerGoingUpInFloorSize(int floor);

    Passenger getPassengerGoingDownInFloor(int floor) const;

    int getPassengerGoingDownSizeInFloor(int floor);

    void pushPassengerInFloor(Passenger passenger);

    void clearData();

public slots:
    //    void sendPassengerRequest(int floor, int direction);
    void onStopped(int currentFloor, int number);

protected:
    void run();

private:
    QList<Passenger> *passengerGoingUpInFloor, *passengerGoingDownInFloor;

signals:
    void passengerUpRequestSent(int floor);
    void passengerDownRequestSent(int floor);
    void passengerOutsideListSent(QList<Passenger> &upList, QList<Passenger> &downList, int elevatorNumber);
    void numberUpPassengerchanged(int inFloor, int upNumber);
    void numberDownPassengerchanged(int inFloor, int downNumber);

};

#endif // FLOORTHREAD_H
