#include "elevatorthread.h"

ElevatorThread::ElevatorThread(QObject *parent) :
    QThread(parent)
{
    upCount = downCount = 0;
    passengerStepOut = passengerStepInUp = passengerStepInDown = 0;
    currentFloor = 1;
    doorOpenRate = 0;
    passengerMovingFlag = false;
    direction = IDLE;
    openRequest = 0;
    closeRequest = false;
    weight = 0;
    size = 0;
    numberPassenger = 0;

    passengerToFloor = new QList<Passenger> [21];

    for(int index = 0; index < 21; ++index)
    {
        inRequest[index] = false;
        outRequest[index] = 0;
    }
    speed = 1;
}

ElevatorThread::~ElevatorThread()
{
    delete [] passengerToFloor;
}

int ElevatorThread::getOutRequest(int floor)
{
    return outRequest[floor];
}

//void ElevatorThread::setOutRequest(int floor, int request)
//{
//    outRequest[floor] = request;
//}

//int ElevatorThread::getNextFloor()
//{
//    if(checkUpRequest() && direction == UP)
//        for(int index = currentFloor; index < 20; ++index)
//            if(outRequest[index] == 1 || outRequest[index] > 2)
//                return index;
//    if(checkDownRequest() && direction == DOWN)
//        for(int index = currentFloor; index >= 0; --index)
//            if(outRequest[index] > 1)
//                return - index;
//    return 100; // return 100 tuc la dang idle
//}

void ElevatorThread::run()
{
    while(true)
    {
        direction = IDLE;
        emit stateChanged("Idle");

        // ko co request thi cho sleep
        while(checkRequest() == IDLE)   // sau 500ms bat dau chay.
            mwait(100);

        // thoat while loop -> co request
        direction = checkRequest();
        // msleep(500);
        while(checkRequest() != IDLE)
        {
            if(direction == UP)
            {
                while(checkUpRequest()) // check co request >= current Floor
                {
                    // co inRequest hoac outRequest di len
                    if(inRequest[currentFloor] || (outRequest[currentFloor] == 1 || outRequest[currentFloor] > 2))
                    {
                        // Khi toi tang ma ko con request tren + co request di xuong -> doi direction xuong
                        if(!checkNonCurrentUpRequest() && (checkNonCurrentDownRequest() || outRequest[currentFloor] == 2))
                            direction = DOWN;
                        emit stopped(currentFloor, number);
                    }
                    if(checkNonCurrentUpRequest())  // check request > current Floor, neu co thi tiep tuc len, ko thi thoat vong lap, doi chieu chuyen dong
                        runUp();
                    else
                        break;
                }
                if(checkDownRequest())
                    direction = DOWN;
            }
            if(direction == DOWN)
            {
                while(checkDownRequest())
                {
                    // co inRequest hoac outRequest di xuong
                    if(inRequest[currentFloor] || outRequest[currentFloor] > 1)
                    {
                        if(!checkNonCurrentDownRequest() && (checkNonCurrentUpRequest() || outRequest[currentFloor] == 1))
                            direction = UP;
                        emit stopped(currentFloor, number);
                    }
                    if(checkNonCurrentDownRequest())
                        runDown();
                    else
                        break;
                }
                if(checkUpRequest())
                    direction = UP;
            }
        }
    }
}

Direction ElevatorThread::checkRequest()
{
    int upDistance = 0;
    int downDistance = 0;
    int index;
    bool upFlag = false;    // co upRequest
    bool downFlag = false;  // co downRequest
    if(inRequest[currentFloor])
        return DOWN; // neu IDLE thi cho tam la DOWN (hoac UP), khi vao no se chuyen sang waiting
//        return direction;
    if(outRequest[currentFloor] == 1 || outRequest[currentFloor] == 3) // neu IDLE thi trang thai tiep theo se phu thuoc requestDirection
        return UP;
    if(outRequest[currentFloor] == 2 || outRequest[currentFloor] == 4) // neu IDLE thi trang thai tiep theo se phu thuoc requestDirection
        return DOWN;

    for(index = currentFloor; index < 21; ++index)
    {
        if(inRequest[index] || outRequest[index] > 0)
        {
            upFlag = true;
            break;
        }
        else
            ++upDistance;
    }
    for(index = currentFloor; index >= 0 ; --index)
    {
        if(inRequest[index] || outRequest[index] > 0)
        {
            downFlag = true;
            break;
        }
        else
            ++downDistance;
    }
    if(!downFlag && !upFlag)    // ko co request
        return IDLE;
    if(downFlag && downDistance <= upDistance)              // request duoi gan hon => xuong trc
        return DOWN;
    return UP;
}

bool ElevatorThread::checkUpRequest()
{
    // !! chay tu current floor de tranh truy xuat invalid memory
    // => can set inRequest[currentFloor] = false khi reach destination floor
    for(int index = currentFloor; index < 21; ++index)
    {
        if(inRequest[index] || outRequest[index] > 0)
            return true;
    }
    return false;
}

bool ElevatorThread::checkDownRequest()
{
    for(int index = currentFloor; index >= 0; --index)
    {
        if(inRequest[index] || outRequest[index] >0)
            return true;
    }
    return false;
}

bool ElevatorThread::checkNonCurrentUpRequest()
{
    if(currentFloor == 20)
        return false;
    for(int index = currentFloor + 1; index < 21; ++index)
    {
        if(inRequest[index] || outRequest[index] > 0)
            return true;
    }
    return false;
}

bool ElevatorThread::checkNonCurrentDownRequest()
{
    if(currentFloor == 0)
        return false;
    for(int index = currentFloor - 1; index >= 0; --index)
    {
        if(inRequest[index] || outRequest[index] >0)
            return true;
    }
    return false;
}

void ElevatorThread::setSpeed(int value)
{
    speed = value;
}

int ElevatorThread::getLastInRequest(Direction direct)
{
    if(direct == UP)
    {
        int i;
        for(i = 20; i > currentFloor; --i)
            if(inRequest[i])
                return i;
    }
    if(direct == DOWN)
    {
        int i;
        for(i = 0; i < currentFloor; ++i)
            if(inRequest[i])
                return i;
    }
    return currentFloor;
}

int ElevatorThread::getLastOutRequest(Direction direct)
{
    if(direct == UP)
    {
        int i;
        for(i = 20; i > currentFloor; --i)
            if(outRequest[i])
                return i;
    }
    if(direct == DOWN)
    {
        int i;
        for(i = 0; i < currentFloor; ++i)
            if(outRequest[i])
                return i;
    }
    return currentFloor;
}

void ElevatorThread::removeOutRequest(int floor, Direction direct)
{
    QMutexLocker lock(&mutex);
    if(direct == UP)
        outRequest[floor] = outRequest[floor] > 1 ? 2 : 0;
    if(direct == DOWN)
        outRequest[floor] = outRequest[floor] == 1 || outRequest[floor] > 2 ? 1 : 0;
}

void ElevatorThread::drawDoor(QPainter &painter)
{
    QRect fixedSize(90, 60, 120, 180);
//    QRect elevatorFrame(0 + 5, 20 + 5, 330 - 10, 250);
    QRect door1(fixedSize.x(), fixedSize.y(), fixedSize.width()/2 * (1 - doorOpenRate/100.0), fixedSize.height());
    QRect door2(fixedSize.x() + fixedSize.width(), fixedSize.y() + fixedSize.height(), -fixedSize.width()/2 * (1 - doorOpenRate/100.0), -fixedSize.height());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::gray));
//    painter.drawRoundRect(elevatorFrame, 5, 5);
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(fixedSize);
    painter.setFont(QFont("Ubuntu", 20));
    painter.drawText(fixedSize, Qt::AlignCenter, QString::number(numberPassenger));

    painter.setBrush(Qt::gray);
    painter.drawRect(door1);
    painter.drawRect(door2);

}

void ElevatorThread::drawPassenger(QPainter &painter)
{
      painter.setBrush(Qt::white);
      painter.setFont(QFont("Ubuntu", 10));

    // duyet list va ve ra
    int offset = 40;
    QMutexLocker locker1(&mutex1);
    for(int i = 0; i < tempGetOutPsgr.size() && i < 12; ++i)
    {
        QRect frame(90 + 75, 250 + offset * i + passengerStepOut, 30, 30);
        painter.drawEllipse(frame);
        painter.drawText(frame, Qt::AlignCenter, QString("%1").arg(tempGetOutPsgr.at(i),0,10)/*.toUpper()*/);
    }
    for(int i = 0; i < tempGoUpPsgr.size() && i < 12; ++i)
    {
        QRect frame(90 + 15, 290 + offset * i - passengerStepInUp, 30, 30);
        painter.drawEllipse(frame);
        painter.drawText(frame, Qt::AlignCenter, QString("%1").arg(tempGoUpPsgr.at(i),0,10)/*.toUpper()*/);
    }
    for(int i = 0; i < tempGoDownPsgr.size() && i < 12; ++i)
    {
        QRect frame(90 + 15, 290 + offset * i - passengerStepInDown, 30, 30);
        painter.drawEllipse(frame);
        painter.drawText(frame, Qt::AlignCenter, QString("%1").arg(tempGoDownPsgr.at(i),0,10)/*.toUpper()*/);
    }
}

void ElevatorThread::setOpenRequest(int value)
{
    openRequest = value;
}

void ElevatorThread::setCloseRequest(bool value)
{
    closeRequest = value;
}

int ElevatorThread::getCurrentFloor() const
{
    return currentFloor;
}

void ElevatorThread::setCurrentFloor(int value)
{
    currentFloor = value;
}

Direction ElevatorThread::getDirection() const
{
    return direction;
}

void ElevatorThread::setNumber(int value)
{
    number = value;
}

int ElevatorThread::getWeight() const
{
    return weight;
}

void ElevatorThread::setWeight(int value)
{
    weight = value;
}

int ElevatorThread::getSize() const
{
    return size;
}

void ElevatorThread::setSize(int value)
{
    size = value;
}

void ElevatorThread::clearData()
{
    upCount = downCount = 0;
    passengerStepOut = passengerStepInUp = passengerStepInDown = 0;
    doorOpenRate = 0;
    passengerMovingFlag = false;
    direction = IDLE;
    openRequest = 0;
    closeRequest = false;
    weight = 0;
    size = 0;
    numberPassenger = 0;

    for(int index = 0; index < 21; ++index)
    {
        passengerToFloor[index].clear();
        inRequest[index] = false;
        outRequest[index] = 0;
    }
}

void ElevatorThread::runUp()
{
    emit stateChanged("Up");
    msleep(M_RUN_VELOCITY/speed);
    ++currentFloor;
    emit currentFloorChanged(currentFloor);
}

void ElevatorThread::runDown()
{
    emit stateChanged("Down");
    msleep(M_RUN_VELOCITY/speed);
    --currentFloor;
    emit currentFloorChanged(currentFloor);
}

void ElevatorThread::onPassengerOutsideListSent(QList<Passenger> &upList, QList<Passenger> &downList, int elevatorNumber)
{
    if(elevatorNumber == number)
    {
        if(direction == UP)
            emit stateChanged("Up...");
        else if(direction == DOWN)
            emit stateChanged("Down...");
        else
            qDebug() << "something wrong...";

        // open door
        passengerMovingFlag = true;
        closeRequest = 0; // de chac chan se mo dc cua
        openDoor(0);

        // tao List tempGoUpPsgr de ve~ trc khi mo cua !!
        mutex1.lock();
        if(direction == UP)
            for(int i = 0; i < upList.size(); ++i)
                tempGoUpPsgr.append(upList.at(i).getPassengerId());      //append/remove
        if(direction == DOWN)
            for(int i = 0; i < downList.size(); ++i)
                tempGoDownPsgr.append(downList.at(i).getPassengerId());
        mutex1.unlock();

        movePassenger(direction);

        // passenger get out
        removePassengerFromLift();

        // passenger get in
        int newOutRequest = outRequest[currentFloor];
        if(direction == UP)
        {
            newOutRequest = outRequest[currentFloor] < 2 ? 0 : 2;
            addPassengerToLift(upList, tempGoUpPsgr);
        }
        else
        {
            newOutRequest = (outRequest[currentFloor] == 0 || outRequest[currentFloor] == 2) ? 0 : 1;
            addPassengerToLift(downList, tempGoDownPsgr);
        }

        passengerMovingFlag = false;
        mwait(0);
        closeDoor();
        tempGetOutPsgr.clear();
        tempGoUpPsgr.clear();
        tempGoDownPsgr.clear();

        upCount = downCount = 0;
        emit painterChanged();

        outRequest[currentFloor] = newOutRequest; // cuoi cung moi thay doi outRequest de tranh no bi thay doi boi thread khac. (bi floorThread thay doi!!)
    }
}

void ElevatorThread::addPassengerToLift(QList<Passenger> &outList, QList<int> &inList)
{
//    qDebug() << outList.size() << "passenger(s) expected to get in" << currentFloor;
    int addedWeight = 0, addedSize = 0/*, numGetIn = 0*/;

    // add passengers into
    while(!outList.isEmpty())
    {
        // append inList trong vong lap de tranh sau khi click up/down de add passenger thi inList ko dc update kip -> ko pop dc
        mutex1.lock();
        for(int i = inList.size(); i < outList.size(); ++i)
            inList.append(outList.at(i).getPassengerId());  // append/remove
        mutex1.unlock();
        // add passengers
        addedWeight = outList.first().getWeight();
        addedSize = outList.first().getSize();
        if(weight + addedWeight < MAX_LOAD && size + addedSize < MAX_SIZE) // add vao ma chua full -> add
        {
            Passenger tempPsgr = outList.first();
            passengerToFloor[tempPsgr.getDestinationFloor()].push_back(tempPsgr);

            if(!outList.isEmpty())
                outList.pop_front();

            // draw
            mutex1.lock();
            if(!inList.isEmpty())
                inList.pop_front();
            mutex1.unlock();
            ++numberPassenger;
            emit painterChanged();

            if(direction == UP)
                emit numberUpPassengerchanged(currentFloor, outList.size());
            else if(direction == DOWN)
                emit numberDownPassengerchanged(currentFloor, outList.size());

            inRequest[tempPsgr.getDestinationFloor()] = true;
//            qDebug() << ++numGetIn << "passenger got in...";
            weight += addedWeight;
            size += addedSize;

            emit elevatorAttributeChanged(weight, size, tempPsgr.getDestinationFloor());
            movePassenger(direction);
        }
        else
        {
            removeOutRequestWhenFull(MAX_LOAD - MAX_LOAD / 10, MAX_SIZE - MAX_SIZE / 10);
            break;
        }
    }
    msleep(500);
    if(outList.isEmpty())
        emit outRequestCompleted(currentFloor, direction); // unlight up/down buttons
}

void ElevatorThread::removePassengerFromLift()
{
    inRequest[currentFloor] = false; // matched
    emit inRequestCompleted(currentFloor);  // unlight floor buttons

//    int numGetOut = 0;
    while(!passengerToFloor[currentFloor].isEmpty())
    {
        Passenger tempPsgr = passengerToFloor[currentFloor].back();
        mutex1.lock();
        tempGetOutPsgr.push_front(tempPsgr.getPassengerId());        // append/remove
        mutex1.unlock();
        --numberPassenger;
        emit painterChanged();    // update paintEvent

        passengerToFloor[currentFloor].pop_back();

//        qDebug() << ++numGetOut << "passenger got out...";
        // update weight, size
        weight -= tempPsgr.getWeight();
        size -= tempPsgr.getSize();
        emit elevatorAttributeChanged(weight, size, -1);
        movePassenger(IDLE);
    }
}

void ElevatorThread::movePassenger(Direction direction)
{
    if(direction == UP)
    {
        passengerStepInUp =  passengerStepInDown = 0;
        while(passengerStepInUp < 40)
        {
            passengerStepInUp ++;
            msleep(M_PASSENGER_MOVING_VELOCITY/speed/40 + 1);
            emit painterChanged();
        }
    }
    else if(direction == DOWN)
    {
        passengerStepInDown = passengerStepInUp = 0;
        while(passengerStepInDown < 40)
        {
            passengerStepInDown ++;
            msleep(M_PASSENGER_MOVING_VELOCITY/speed/40 + 1);
            emit painterChanged();
        }
    }
    else
    {
        passengerStepOut= 0;
        while(passengerStepOut < 40)
        {
            passengerStepOut ++;
            msleep(M_PASSENGER_MOVING_VELOCITY/speed/40 + 1);
            emit painterChanged();
        }
    }
}

void ElevatorThread::removeOutRequestWhenFull(int fullWeight, int fullSize)
{
    // full -> bo cac request tiep theo cho den khi co inRequest -> case 7,8
    if(weight > fullWeight || size > fullSize)
    {
        if(direction == UP && currentFloor < 20)
        {
            int nextFloorWithInRequest;
            for(nextFloorWithInRequest = currentFloor + 1; nextFloorWithInRequest < 21; ++nextFloorWithInRequest)
                 if(inRequest[nextFloorWithInRequest])
                     break;     // -> nextFloorWithInRequest la floor gan nhat co inRequest
            for(int index = currentFloor + 1; index < nextFloorWithInRequest; ++index)
            {
                QMutexLocker locker(&mutex);
                outRequest[index] = outRequest[index] < 2 ? 0 : 2;
            }
        }
        if(direction == DOWN && currentFloor > 0)
        {
            int nextFloorWithInRequest;
            for(nextFloorWithInRequest = currentFloor - 1; nextFloorWithInRequest >= 0; --nextFloorWithInRequest)
                 if(inRequest[nextFloorWithInRequest])
                     break;     // -> nextFloorWithInRequest la floor gan nhat co inRequest
            for(int index = currentFloor - 1; index > nextFloorWithInRequest; --index)
            {
                QMutexLocker locker(&mutex);
                outRequest[index] = (outRequest[index] == 0 || outRequest[index] == 2) ? 0 : 1;
            }
        }
    }
}

void ElevatorThread::openDoor(int mtime)
{
    // remove request
    openRequest = 0;

    while(doorOpenRate < 90)
    {
        ++doorOpenRate;
        emit painterChanged();
        msleep(M_OPEN_DOOR_VELOCITY/speed/90 + 1);

        // Press close button
        if(closeRequest)
        {
            closeDoor();
            return; //must-havetempGetOutPsgr
        }
    }
    mwait(mtime);
    if(doorOpenRate > 0)    //must-have
        closeDoor();
}

void ElevatorThread::closeDoor()
{
    // remove request
    closeRequest = 0;
    if(!passengerMovingFlag)
    {
        while(doorOpenRate > 0)
        {
            --doorOpenRate;
            emit painterChanged();
            msleep(M_OPEN_DOOR_VELOCITY/speed/90 + 1);

            // Press open button
            if(openRequest)
            {
                openDoor(openRequest);
                return; //must-have
            }
        }
        // dong vao roi, trong 1s co the mo lai
        mwait(OPEN_WAIT_TIME/speed);
    }
}

void ElevatorThread::mwait(int msec)
{
    for(int i = 0; i< msec/10; ++i)
    {
        msleep(10);
        // press open/close
        if(openRequest)
        {
            openDoor(openRequest);
            return; //must-have
        }
        if(closeRequest)
        {
            closeDoor();
            return; //must-have
        }
    }
}

void ElevatorThread::onFloorButtonClicked(int floor)
{
    inRequest[floor] = true;
}

void ElevatorThread::onFloorButtonUnclicked(int floor)
{
    inRequest[floor] = false;
}

void ElevatorThread::onElevatorDelegated(int pickedElevator, int floorRequest, Direction directionRequest)
{
    if(pickedElevator == number) // la elevator receive signal
    {
        QMutexLocker locker(&mutex);
        if(outRequest[floorRequest] == 0)
            outRequest[floorRequest] = directionRequest == UP ? 1 : 2;
        else if(outRequest[floorRequest] == 1 && directionRequest == DOWN)     //up-down
            outRequest[floorRequest] = 3;
        else if(outRequest[floorRequest] == 2 && directionRequest == UP)       //down-up
            outRequest[floorRequest] = 4;
    }
}
