#include "elevatormanager.h"
#include "ui_elevatormanager.h"


ElevatorManager::ElevatorManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ElevatorManager)
{
    ui->setupUi(this);

    settingDialog = new SettingDialog(this);
//    settingDialog->show();
    numElevator = 4;
    int index = 0;
    for(index = 0; index < numElevator; ++index)
    {
        elevator[index] = new Elevator(this, index);    //350x300
        elevator[index]->move(10+index*350, 20);
    }
    floorPanel = new FloorPanel(this);  //300 x 700
    floorPanel->move(10+index*350,20);
    this->resize(index*350 + 300 + 20 + 30, 720);

    // test which elevator was delegated
    label = new QLabel [21];
    for(int i = 20; i >= 0; --i)
    {
        label[i].setText("~");
        label[i].setParent(this);
        label[i].move(index*350 + 300 + 20, 33*(20 - i) - 200);
    }
    // connection
    connect(floorPanel, SIGNAL(upButtonClicked(int)), this, SLOT(onUpButtonClicked(int)));
    connect(floorPanel, SIGNAL(downButtonClicked(int)), this, SLOT(onDownButtonClicked(int)));
    connect(floorPanel->getFloor(), SIGNAL(passengerUpRequestSent(int)), this, SLOT(onUpButtonClicked(int)));
    connect(floorPanel->getFloor(), SIGNAL(passengerDownRequestSent(int)), this, SLOT(onDownButtonClicked(int)));
    for(int index = 0; index < numElevator; ++index)
    {
        connect(this, SIGNAL(elevatorDelegated(int,int,Direction)), elevator[index]->getOperation(), SLOT(onElevatorDelegated(int,int,Direction)));

        connect(elevator[index]->getOperation(), SIGNAL(stopped(int,int)), floorPanel->getFloor(), SLOT(onStopped(int,int)), Qt::DirectConnection);
        connect(elevator[index]->getOperation(), SIGNAL(outRequestCompleted(int,int)), floorPanel, SLOT(onOutRequestCompleted(int,int)));

        connect(floorPanel->getFloor(), SIGNAL(passengerOutsideListSent(QList<Passenger>&, QList<Passenger>&,int)),
                elevator[index]->getOperation(), SLOT(onPassengerOutsideListSent(QList<Passenger>&, QList<Passenger>&, int)), Qt::DirectConnection);

        connect(elevator[index]->getOperation(), SIGNAL(numberUpPassengerchanged(int,int)), floorPanel, SLOT(onNumberUpPassengerchanged(int,int)));
        connect(elevator[index]->getOperation(), SIGNAL(numberDownPassengerchanged(int,int)), floorPanel, SLOT(onNumberDownPassengerchanged(int,int)));
    }
    connect(settingDialog, SIGNAL(initialDataSent(int*,QList<Passenger>*)), SLOT(onInitialDataSent(int*,QList<Passenger>*)));

    speed = 1;
}

ElevatorManager::~ElevatorManager()
{
    delete ui;
}

void ElevatorManager::onUpButtonClicked(int button) //// co van de <<<<<<<<<<<
{
    delegate(button, UP);
    floorPanel->setButtonOn(button, 1);
}

void ElevatorManager::onDownButtonClicked(int button)
{
    delegate(button, DOWN);
    floorPanel->setButtonOn(button, 2);
}

void ElevatorManager::onInitialDataSent(int *elevatorFloor, QList<Passenger> *passengerInFloor)
{
    floorPanel->getFloor()->runFlag = false;
    QThread::msleep(500);   // doi 0.5s cho vong lap ham run ben floorThread quay ve dau tien.
    floorPanel->clearData();
    for(int i = 0; i < numElevator; ++i)
    {
        elevator[i]->setCurrentFloor(elevatorFloor[i]);
        elevator[i]->clearData();
    }

    for(int i = 0; i < 21; ++i)
    {
        QListIterator<Passenger> iter(passengerInFloor[i]);
        while(iter.hasNext())
            floorPanel->getFloor()->pushPassengerInFloor(iter.next());
        // setText
        floorPanel->onNumberUpPassengerchanged(i,floorPanel->getFloor()->getPassengerGoingUpInFloorSize(i));
        floorPanel->onNumberDownPassengerchanged(i,floorPanel->getFloor()->getPassengerGoingDownSizeInFloor(i));
    }
    floorPanel->getFloor()->runFlag = true;
}

int ElevatorManager::calculateDistance(int elevatorIndex, int requestFloor, Direction requestDirection, bool maxDistance)
{
    int currentFloor = elevator[elevatorIndex]->getOperation()->getCurrentFloor();
    int lastUpInRequest = elevator[elevatorIndex]->getOperation()->getLastInRequest(UP);
    int lastDownInRequest = elevator[elevatorIndex]->getOperation()->getLastInRequest(DOWN);
//    int lastUpOutRequest = elevator[elevatorIndex]->getOperation()->getLastOutRequest(UP);
//    int lastDownOutRequest = elevator[elevatorIndex]->getOperation()->getLastOutRequest(DOWN);

//    int lastUpRequest = lastUpInRequest > lastUpOutRequest ? lastUpInRequest : lastUpOutRequest;
//    int lastDownRequest = lastDownInRequest < lastDownOutRequest ? lastDownInRequest : lastDownOutRequest;

    int lastUpRequest = lastUpInRequest;
    int lastDownRequest = lastDownInRequest;

    if(maxDistance)
    {
        lastUpRequest = 20;
        lastDownRequest = 0;
    }
    Direction state = elevator[elevatorIndex]->getDirection();

    if(lastDownRequest > requestFloor)
        lastDownRequest = requestFloor;
    if(lastUpRequest < requestFloor)
        lastUpRequest = requestFloor;

    if(state == IDLE)
    {
        if(currentFloor == requestFloor)
            return 0;
        state = requestFloor > currentFloor ? UP : DOWN;
    }
    if(requestDirection == UP && state == UP)
        return requestFloor >= currentFloor ? requestFloor - currentFloor : (lastUpRequest - lastDownRequest) * 2 - currentFloor + requestFloor;
    if(requestDirection == DOWN && state == DOWN)
        return requestFloor <= currentFloor ? currentFloor - requestFloor : (lastUpRequest - lastDownRequest) * 2 - requestFloor + currentFloor;
    if(requestDirection == UP && state == DOWN)
        return requestFloor + currentFloor - lastDownRequest * 2;
    if(requestDirection == DOWN && state == UP)
        return lastUpRequest * 2 - requestFloor - currentFloor;

    qDebug() << "something wrong...";
    return -1;
}

void ElevatorManager::delegate(int floorRequest, Direction directionRequest)
{
    int distance[numElevator];
    int pickedElevator = qrand() % numElevator;
    // Tinh "distance"
    for(int i = 0; i < numElevator; ++i)
    {
        distance[i] = calculateDistance(i, floorRequest, directionRequest, true);

        if(elevator[i]->getWeight() > MAX_LOAD - MAX_LOAD / 5 || elevator[i]->getSize() > MAX_SIZE - MAX_SIZE / 5)
            distance[i] += 10;
        if(elevator[i]->getWeight() > MAX_LOAD - MAX_LOAD / 10 || elevator[i]->getSize() > MAX_SIZE - MAX_SIZE / 10)
            distance[i] += 50;  // full 90% -> tranh delegate vao thang dang idle ma dang don khach -> full
    }
    for(int i = 0; i < numElevator; ++i)
    {
        if(distance[i] < distance[pickedElevator])
            pickedElevator = i;
        else if(distance[i] == distance[pickedElevator] && elevator[i]->getDirection() != IDLE)
            pickedElevator = i;
    }
    if(elevator[pickedElevator]->getWeight() < MAX_LOAD - MAX_LOAD / 10 && elevator[pickedElevator]->getSize() < MAX_SIZE - MAX_SIZE / 10) // it nhat 90% trong thi moi delegate
    {
        for(int i = 0; i < numElevator; ++i)
        {
            int outRequest = elevator[i]->getOutRequest(floorRequest);
            if((outRequest == directionRequest || outRequest > 2) && i == pickedElevator)
                return;
            if((outRequest == directionRequest || outRequest > 2) && abs(elevator[i]->getCurrentFloor() - floorRequest) < 2)
            {
                return;
            }
            if(outRequest == directionRequest || outRequest > 2)
            {
                elevator[i]->getOperation()->removeOutRequest(floorRequest, directionRequest);
            }
        }
        emit elevatorDelegated(pickedElevator, floorRequest, directionRequest);
        label[floorRequest].setText(QString::number(pickedElevator + 1));   // check xem elevator nao dc goi
    }
}

void ElevatorManager::on_actionExit_triggered()
{
    this->close();
}

void ElevatorManager::on_actionSetting_triggered()
{
    settingDialog->show();
}

void ElevatorManager::on_actionSpeed_Up_triggered()
{
    if(speed < 10)
        ++speed;
    for(int i = 0; i < numElevator; ++i)
        elevator[i]->setSpeed(speed);
    qDebug() << "Speed changed..." << speed;
}

void ElevatorManager::on_actionSpeed_Down_triggered()
{
    if(speed > 1)
        --speed;
    for(int i = 0; i < numElevator; ++i)
        elevator[i]->setSpeed(speed);
    qDebug() << "Speed changed..." << speed;
}
