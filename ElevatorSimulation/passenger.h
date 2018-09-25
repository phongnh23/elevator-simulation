#ifndef PASSENGER_H
#define PASSENGER_H

#include <QObject>
#include <QWidget>
#include <math.h>

class Passenger
{

public:
    Passenger(int weight, int size, int fromFloor, int destinationFloor);
    Passenger(int inFloor);
    Passenger(int inFloor, bool isGoUp);
    ~Passenger();

    int getSize() const;

    int getWeight() const;

    int getFromFloor() const;

    int getDestinationFloor() const;

    int getPassengerId() const;

    static int iD;

protected:

private: 
    void setId();
    int passengerId;
    int size;
    int weight;
    int fromFloor;
    int destinationFloor;
};

#endif // PASSENGER_H
