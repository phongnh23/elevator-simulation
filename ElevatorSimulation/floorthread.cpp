#include "floorthread.h"

FloorThread::FloorThread(QObject *parent) : QThread(parent)
{
    passengerGoingUpInFloor = new QList<Passenger> [21];
    passengerGoingDownInFloor = new QList<Passenger> [21];
    runFlag = true;
}

FloorThread::~FloorThread()
{
    delete [] passengerGoingUpInFloor;
    delete [] passengerGoingDownInFloor;
}

void FloorThread::run()
{
    while(true)
    {
        while(!runFlag)
            msleep(500);
        for(int index = 0; index < 21; ++index)
        {
            if(!passengerGoingUpInFloor[index].isEmpty())
            {
                emit passengerUpRequestSent(index);
            }
        }
        msleep(100); // 500s check 1 lan
        for(int index = 0; index < 21; ++index)
        {
            if(!passengerGoingDownInFloor[index].isEmpty())
            {
                emit passengerDownRequestSent(index);
            }
        }
        msleep(100);
    }
}

void FloorThread::addPassenger(int inFloor)
{
    Passenger inPassenger(inFloor);
    if(inPassenger.getDestinationFloor() > inFloor)
        passengerGoingUpInFloor[inFloor].append(inPassenger);
    else
        passengerGoingDownInFloor[inFloor].append(inPassenger);
}

void FloorThread::addUpPassenger(int inFloor)
{
    Passenger inPassenger(inFloor, true);
    passengerGoingUpInFloor[inFloor].append(inPassenger);
    emit numberUpPassengerchanged(inFloor, passengerGoingUpInFloor[inFloor].size());
}

void FloorThread::addDownPassenger(int inFloor)
{
    Passenger inPassenger(inFloor, false);
    passengerGoingDownInFloor[inFloor].append(inPassenger);
    emit numberDownPassengerchanged(inFloor, passengerGoingDownInFloor[inFloor].size());
}

Passenger FloorThread::getPassengerGoingUpInFloor(int floor) const
{
    Passenger passenger = passengerGoingUpInFloor[floor].first();
    passengerGoingUpInFloor[floor].pop_front();
    return passenger;
}

int FloorThread::getPassengerGoingUpInFloorSize(int floor)
{
    return passengerGoingUpInFloor[floor].size();
}

Passenger FloorThread::getPassengerGoingDownInFloor(int floor) const
{
    Passenger passenger = passengerGoingDownInFloor[floor].first();
    passengerGoingDownInFloor[floor].pop_front();
    return passenger;
}

int FloorThread::getPassengerGoingDownSizeInFloor(int floor)
{
    return passengerGoingDownInFloor[floor].size();
}

void FloorThread::pushPassengerInFloor(Passenger passenger)
{
    int fromFloor = passenger.getFromFloor(), toFloor = passenger.getDestinationFloor();
    if(fromFloor > toFloor)
    {
        passengerGoingDownInFloor[fromFloor].append(passenger);
    }
    else if(fromFloor < toFloor)
    {
        passengerGoingUpInFloor[fromFloor].append(passenger);
    }
}

void FloorThread::clearData()
{
    for(int i = 0; i < 21; ++i)
    {
        passengerGoingUpInFloor[i].clear();
        passengerGoingDownInFloor[i].clear();
    }
}

void FloorThread::onStopped(int currentFloor, int number)
{
    emit passengerOutsideListSent(passengerGoingUpInFloor[currentFloor], passengerGoingDownInFloor[currentFloor], number);
}

