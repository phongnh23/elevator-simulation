#include "passenger.h"

int Passenger::iD = 1;

Passenger::Passenger(int weight, int size, int fromFloor, int destinationFloor)
{
    setId();
    this->weight = weight >= 0 ? weight : 30 + qrand() % 70;
    this->size = size >= 0 ? size : round(pow(this->weight/100.0, 2.0/3) * 1500);
    this->fromFloor = fromFloor >= 0 ? fromFloor : qrand() % 21;
    this->destinationFloor = destinationFloor >= 0 ? destinationFloor : qrand() % 21;
}

Passenger::Passenger(int inFloor)
{
    setId();
    weight = 30 + qrand() % 70;
    size = round(pow(weight/100.0, 2.0/3) * 1500);
    fromFloor = inFloor;
    destinationFloor = qrand() % 21;
    while(destinationFloor == inFloor)
        destinationFloor = qrand() % 21;
}

Passenger::Passenger(int inFloor, bool isGoUp)
{
    setId();
    weight = 30 + qrand() % 70;
    size = round(pow(weight/100.0, 2.0/3) * 1500);
    fromFloor = inFloor;
    if(isGoUp && inFloor != 20)
        destinationFloor = inFloor + qrand() % (20 - inFloor) + 1;
    else if(!isGoUp && inFloor != 0)
        destinationFloor = inFloor - qrand() % inFloor  - 1;
    else
        while(destinationFloor == inFloor)
            destinationFloor = qrand() % 21;
}

Passenger::~Passenger()
{

}

void Passenger::setId()
{
    passengerId = iD++;
    if(iD > 999)
    {
        iD = 0;
    }
}

int Passenger::getPassengerId() const
{
    return passengerId;
}

int Passenger::getDestinationFloor() const
{
    return destinationFloor;
}

int Passenger::getFromFloor() const
{
    return fromFloor;
}

int Passenger::getWeight() const
{
    return weight;
}

int Passenger::getSize() const
{
    return size;
}
