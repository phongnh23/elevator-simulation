#include "elevator.h"
#include "ui_elevator.h"

Elevator::Elevator(QWidget *parent, int toNumber) :
    QWidget(parent),
    ui(new Ui::Elevator)
{
    ui->setupUi(this);
    operation = new ElevatorThread(this);
    operation->setNumber(toNumber);
    ui->elevatorGroupBox->setTitle("ELEVATOR " + QString::number(toNumber + 1));
    ui->floorGroupBox->setTitle("Floor " + (operation->getCurrentFloor() == 0 ? "G" : QString::number(operation->getCurrentFloor())));

    for(int index = 0; index < 21; ++index)
    {
        isFloorButtonClicked[index] = false;
    }

    operation->start();
    operation->setPriority(QThread::HighestPriority);

    //Connection
    connect(operation, SIGNAL(painterChanged()), this, SLOT(onPainterChanged()));
    connect(this, SIGNAL(floorButtonClicked(int)), operation, SLOT(onFloorButtonClicked(int)));
    connect(this, SIGNAL(floorButtonUnclicked(int)), operation, SLOT(onFloorButtonUnclicked(int)));
    connect(operation, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));
    connect(operation, SIGNAL(currentFloorChanged(int)), this, SLOT(onCurrentFloorChanged(int)));
    connect(operation, SIGNAL(inRequestCompleted(int)), this, SLOT(onInRequestCompleted(int)));
    connect(operation, SIGNAL(elevatorAttributeChanged(int,int,int)), this, SLOT(onElevatorAttributeChanged(int,int,int)));
    speed = 1;
}

Elevator::~Elevator()
{
    delete ui;
}

int Elevator::getCurrentFloor()
{
    return operation->getCurrentFloor();
}

void Elevator::setCurrentFloor(int value)
{
    operation->setCurrentFloor(value);
    ui->floorGroupBox->setTitle("Floor " + (value == 0 ? "G" : QString::number(value)));
}

//int Elevator::getNextFloor()
//{
//    return operation->getNextFloor();
//}

Direction Elevator::getDirection()
{
    return operation->getDirection();
}

void Elevator::on_closeButton_pressed()
{
    operation->setCloseRequest(true);
    ui->closeButton->setStyleSheet("background-color:#32ced1");
}

void Elevator::on_openButton_pressed()
{
    operation->setOpenRequest(OPEN_WAIT_TIME/speed);
    ui->openButton->setStyleSheet("background-color:#32ced1");
}

void Elevator::on_closeButton_released()
{
    ui->closeButton->setStyleSheet("background-color:#ffffff");
}

void Elevator::on_openButton_released()
{
    ui->openButton->setStyleSheet("background-color:#ffffff");
}

void Elevator::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    operation->drawDoor(painter);
    operation->drawPassenger(painter);
    painter.end();
}

//=================================BUTTON===========================================

void Elevator::on_button_0_clicked()
{
    buttonClickFunction(ui->button_0, 0);
}

void Elevator::on_button_1_clicked()
{
    buttonClickFunction(ui->button_1, 1);
}

void Elevator::on_button_2_clicked()
{
    buttonClickFunction(ui->button_2, 2);
}

void Elevator::on_button_3_clicked()
{
    buttonClickFunction(ui->button_3, 3);
}

void Elevator::on_button_4_clicked()
{
    buttonClickFunction(ui->button_4, 4);
}

void Elevator::on_button_5_clicked()
{
    buttonClickFunction(ui->button_5, 5);
}

void Elevator::on_button_6_clicked()
{
    buttonClickFunction(ui->button_6, 6);
}

void Elevator::on_button_7_clicked()
{
    buttonClickFunction(ui->button_7, 7);
}

void Elevator::on_button_8_clicked()
{
    buttonClickFunction(ui->button_8, 8);
}

void Elevator::on_button_9_clicked()
{
    buttonClickFunction(ui->button_9, 9);
}

void Elevator::on_button_10_clicked()
{
    buttonClickFunction(ui->button_10, 10);
}

void Elevator::on_button_11_clicked()
{
    buttonClickFunction(ui->button_11, 11);
}

void Elevator::on_button_12_clicked()
{
    buttonClickFunction(ui->button_12, 12);
}

void Elevator::on_button_13_clicked()
{
    buttonClickFunction(ui->button_13, 13);
}

void Elevator::on_button_14_clicked()
{
    buttonClickFunction(ui->button_14, 14);
}

void Elevator::on_button_15_clicked()
{
    buttonClickFunction(ui->button_15, 15);
}

void Elevator::on_button_16_clicked()
{
    buttonClickFunction(ui->button_16, 16);
}

void Elevator::on_button_17_clicked()
{
    buttonClickFunction(ui->button_17, 17);
}

void Elevator::on_button_18_clicked()
{
    buttonClickFunction(ui->button_18, 18);
}

void Elevator::on_button_19_clicked()
{
    buttonClickFunction(ui->button_19, 19);
}

void Elevator::on_button_20_clicked()
{
    buttonClickFunction(ui->button_20, 20);
}

void Elevator::onPainterChanged()
{
    update();
}

void Elevator::onStateChanged(QString state)
{
    ui->runLabel->setScaledContents(true);
    if(state == "Up")
    {
        QPixmap upStatePixmap(":/elevator/u2.png");
        ui->runLabel->setPixmap(upStatePixmap);
    }
    else if(state == "Down")
    {
        QPixmap downStatePixmap(":/elevator/d2.png");
        ui->runLabel->setPixmap(downStatePixmap);
    }
    else if(state == "Up...")
    {
        QPixmap downStatePixmap(":/elevator/u1.png");
        ui->runLabel->setPixmap(downStatePixmap);
    }
    else if(state == "Down...")
    {
        QPixmap downStatePixmap(":/elevator/d1.png");
        ui->runLabel->setPixmap(downStatePixmap);
    }
    else
    {
        QPixmap idleStatePixmap(":/elevator/up.png");
        ui->runLabel->setPixmap(idleStatePixmap.scaled(0, 0, Qt::KeepAspectRatio));
    }
}

void Elevator::onCurrentFloorChanged(int floor)
{
    ui->floorGroupBox->setTitle("Floor " + (floor == 0 ? "G" : QString::number(floor)));
}

void Elevator::onInRequestCompleted(int currentFloor)
{
    switch(currentFloor)
    {
        case 0: ui->button_0->setStyleSheet("background-color:#ffffff"); break;
        case 1: ui->button_1->setStyleSheet("background-color:#ffffff"); break;
        case 2: ui->button_2->setStyleSheet("background-color:#ffffff"); break;
        case 3: ui->button_3->setStyleSheet("background-color:#ffffff"); break;
        case 4: ui->button_4->setStyleSheet("background-color:#ffffff"); break;
        case 5: ui->button_5->setStyleSheet("background-color:#ffffff"); break;
        case 6: ui->button_6->setStyleSheet("background-color:#ffffff"); break;
        case 7: ui->button_7->setStyleSheet("background-color:#ffffff"); break;
        case 8: ui->button_8->setStyleSheet("background-color:#ffffff"); break;
        case 9: ui->button_9->setStyleSheet("background-color:#ffffff"); break;
        case 10: ui->button_10->setStyleSheet("background-color:#ffffff"); break;
        case 11: ui->button_11->setStyleSheet("background-color:#ffffff"); break;
        case 12: ui->button_12->setStyleSheet("background-color:#ffffff"); break;
        case 13: ui->button_13->setStyleSheet("background-color:#ffffff"); break;
        case 14: ui->button_14->setStyleSheet("background-color:#ffffff"); break;
        case 15: ui->button_15->setStyleSheet("background-color:#ffffff"); break;
        case 16: ui->button_16->setStyleSheet("background-color:#ffffff"); break;
        case 17: ui->button_17->setStyleSheet("background-color:#ffffff"); break;
        case 18: ui->button_18->setStyleSheet("background-color:#ffffff"); break;
        case 19: ui->button_19->setStyleSheet("background-color:#ffffff"); break;
        case 20: ui->button_20->setStyleSheet("background-color:#ffffff"); break;
    default: break;
    }
}

void Elevator::onElevatorAttributeChanged(int weight, int size, int floorButton)
{
    if(weight < MAX_LOAD - MAX_LOAD / 10 && size < MAX_SIZE - MAX_SIZE / 10)    // > 90% -> full
    {
        ui->weightLabel->setStyleSheet("color:black");
        ui->weightLabel->setText(QString::number(weight) + " Kg");
        ui->sizeLabel->setText(QString::number(size) + " Cm2");
    }
    else
    {
        ui->weightLabel->setStyleSheet("color:red; font: bold");
        ui->weightLabel->setText("FULL");
        ui->sizeLabel->setText("");
    }
    switch(floorButton)
    {
        case 0: ui->button_0->setStyleSheet("background-color:#32ced1"); break;
        case 1: ui->button_1->setStyleSheet("background-color:#32ced1"); break;
        case 2: ui->button_2->setStyleSheet("background-color:#32ced1"); break;
        case 3: ui->button_3->setStyleSheet("background-color:#32ced1"); break;
        case 4: ui->button_4->setStyleSheet("background-color:#32ced1"); break;
        case 5: ui->button_5->setStyleSheet("background-color:#32ced1"); break;
        case 6: ui->button_6->setStyleSheet("background-color:#32ced1"); break;
        case 7: ui->button_7->setStyleSheet("background-color:#32ced1"); break;
        case 8: ui->button_8->setStyleSheet("background-color:#32ced1"); break;
        case 9: ui->button_9->setStyleSheet("background-color:#32ced1"); break;
        case 10: ui->button_10->setStyleSheet("background-color:#32ced1"); break;
        case 11: ui->button_11->setStyleSheet("background-color:#32ced1"); break;
        case 12: ui->button_12->setStyleSheet("background-color:#32ced1"); break;
        case 13: ui->button_13->setStyleSheet("background-color:#32ced1"); break;
        case 14: ui->button_14->setStyleSheet("background-color:#32ced1"); break;
        case 15: ui->button_15->setStyleSheet("background-color:#32ced1"); break;
        case 16: ui->button_16->setStyleSheet("background-color:#32ced1"); break;
        case 17: ui->button_17->setStyleSheet("background-color:#32ced1"); break;
        case 18: ui->button_18->setStyleSheet("background-color:#32ced1"); break;
        case 19: ui->button_19->setStyleSheet("background-color:#32ced1"); break;
        case 20: ui->button_20->setStyleSheet("background-color:#32ced1"); break;
    default: break;
    }
}

ElevatorThread *Elevator::getOperation() const
{
    return operation;
}

int Elevator::getOutRequest(int floor)
{
    return operation->getOutRequest(floor);
}

//void Elevator::setOutRequest(int floor, int request)
//{
//    operation->setOutRequest(floor, request);
//}

//void Elevator::setOpenRequest()
//{
//    operation->setOpenRequest(OPEN_WAIT_TIME/speed);
//}

int Elevator::getWeight() const
{
    return operation->getWeight();
}

void Elevator::setWeight(int value)
{
    operation->setWeight(value);
}

int Elevator::getSize() const
{
    return operation->getSize();
}

void Elevator::setSize(int value)
{
    operation->setSize(value);
}

void Elevator::clearData()
{
    operation->clearData();
    for(int index = 0; index < 21; ++index)
    {
        isFloorButtonClicked[index] = false;
    }
    ui->button_0->setStyleSheet("background-color:#ffffff");
    ui->button_1->setStyleSheet("background-color:#ffffff");
    ui->button_2->setStyleSheet("background-color:#ffffff");
    ui->button_3->setStyleSheet("background-color:#ffffff");
    ui->button_4->setStyleSheet("background-color:#ffffff");
    ui->button_5->setStyleSheet("background-color:#ffffff");
    ui->button_6->setStyleSheet("background-color:#ffffff");
    ui->button_7->setStyleSheet("background-color:#ffffff");
    ui->button_8->setStyleSheet("background-color:#ffffff");
    ui->button_9->setStyleSheet("background-color:#ffffff");
    ui->button_10->setStyleSheet("background-color:#ffffff");
    ui->button_11->setStyleSheet("background-color:#ffffff");
    ui->button_12->setStyleSheet("background-color:#ffffff");
    ui->button_13->setStyleSheet("background-color:#ffffff");
    ui->button_14->setStyleSheet("background-color:#ffffff");
    ui->button_15->setStyleSheet("background-color:#ffffff");
    ui->button_16->setStyleSheet("background-color:#ffffff");
    ui->button_17->setStyleSheet("background-color:#ffffff");
    ui->button_18->setStyleSheet("background-color:#ffffff");
    ui->button_19->setStyleSheet("background-color:#ffffff");
    ui->button_20->setStyleSheet("background-color:#ffffff");
    ui->weightLabel->setStyleSheet("color:black");
    ui->weightLabel->setText("0 Kg");
    ui->sizeLabel->setText("0 Cm2");
    QPixmap idleStatePixmap(":/elevator/up.png");
    ui->runLabel->setPixmap(idleStatePixmap.scaled(0, 0, Qt::KeepAspectRatio));
}

void Elevator::setSpeed(int value)
{
    speed = value;
    operation->setSpeed(value);
}

// for reducing workload of writing on_button_clicked codes
void Elevator::buttonClickFunction(QPushButton* button, int index)
{
    isFloorButtonClicked[index] = !isFloorButtonClicked[index];
    if(isFloorButtonClicked[index])
    {
        button->setStyleSheet("background-color:#32ced1");
        emit floorButtonClicked(index);
    }
    else
    {
        button->setStyleSheet("background-color:#ffffff");
        emit floorButtonUnclicked(index);
    }
}
