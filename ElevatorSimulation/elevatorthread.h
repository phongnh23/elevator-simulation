#ifndef ELEVATORTHREAD_H
#define ELEVATORTHREAD_H

#include <QThread>
#include <QList>
#include <QPainter>
#include <QDebug>
#include <QMutex>
#include "passenger.h"
#include "enums.h"

#define SPEED 1        // max 20
#define M_RUN_VELOCITY 1000/SPEED
#define M_PASSENGER_MOVING_VELOCITY 800/SPEED    // 1 step = velocity / 40
#define M_OPEN_DOOR_VELOCITY 1500/SPEED         // 1 step = velocity / 90
#define OPEN_WAIT_TIME 1000/SPEED
#define MAX_LOAD 1000
#define MAX_SIZE 15000

class ElevatorThread: public QThread
{
    Q_OBJECT

public:
    explicit ElevatorThread(QObject *parent = NULL);
    ~ElevatorThread();

    int getOutRequest(int floor);

//    void setOutRequest(int floor, int request);//

//    int getNextFloor();//

    void drawDoor(QPainter &painter);

    void drawPassenger(QPainter &painter);

    void setOpenRequest(int value);

    void setCloseRequest(bool value);

    int getCurrentFloor() const;

    void setCurrentFloor(int value);

    Direction getDirection() const;

    void setNumber(int value);

    int getWeight() const;
    void setWeight(int value);

    int getSize() const;
    void setSize(int value);

    void clearData();

    void setSpeed(int value);

    int getLastInRequest(Direction direct);

    int getLastOutRequest(Direction direct);

    void removeOutRequest(int floor, Direction direct);

protected:

    bool inRequest[21];
    // outRequest: 0- ko request, 1- up request, 2- down request, 3- up and down, 4- down and up
    int outRequest[21];

    int currentFloor;
    Direction direction;
    int weight;
    int size;
    int numberPassenger;

    int openRequest;
    bool closeRequest;
    int number;

    void run() override;

    Direction checkRequest();
    bool checkUpRequest();
    bool checkDownRequest();

    bool checkNonCurrentUpRequest();
    bool checkNonCurrentDownRequest();

    int speed;

private:
    QList<Passenger> *passengerToFloor;

    // For painting
    QList<int> tempGetOutPsgr;
    QList<int> tempGoUpPsgr;
    QList<int> tempGoDownPsgr;
    int passengerStepOut, passengerStepInUp, passengerStepInDown;

    QMutex outRequest_mutex, drawPsgr_mutex;

    int upCount;
    int downCount;

    bool passengerMovingFlag;
    int doorOpenRate;

    void runUp();
    void runDown();
    void stopRunnig(int time);

    void addPassengerToLift(QList<Passenger> &outList, QList<int> &inList);
    void removePassengerFromLift();

    void movePassenger(Direction direction);

    void removeOutRequestWhenFull(int fullWeight, int fullSize);

public slots:
    void openDoor(int mtime);
    void closeDoor();
    void mwait(int msec);

    // from Elevator
    void onFloorButtonClicked(int floor);
    void onFloorButtonUnclicked(int floor);

    // from ElevatorManager
    void onElevatorDelegated(int pickedElevator, int floorRequest, Direction directionRequest);

    void onPassengerOutsideListSent(QList<Passenger> &upList, QList<Passenger> &downList, int elevatorNumber);

signals:

    void painterChanged();  // call update()

    void stateChanged(QString state);
    void currentFloorChanged(int currentFloor);
    void inRequestCompleted(int currentFloor); // bo chon tang
    void outRequestCompleted(int currentFloor, int requestDirection); // completed -> unlight up/down buttons

    void outRequestUncompleted(int currentFloor, int requestDirection, int numRequestCompleted);

    void elevatorAttributeChanged(int weight, int size, int floorButton);    // modify weight and size labels and floorButton light (floorButton = -1: off)
    void numberUpPassengerchanged(int inFloor, int upNumber);
    void numberDownPassengerchanged(int inFloor, int downNumber);

    void stopped(int currentFloor, int number);
};

#endif // ELEVATORTHREAD_H
