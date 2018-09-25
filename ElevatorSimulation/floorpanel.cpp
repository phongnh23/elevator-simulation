#include "floorpanel.h"

FloorPanel::FloorPanel(QWidget *parent) : QWidget(parent)
{
    floor = new FloorThread;
    this->setFixedSize(300,700);
    vLayout = new QVBoxLayout(this);

    QPixmap upPixmap(":/elevator/up.png");
    QPixmap downPixmap(":/elevator/down.png");
    QIcon upIcon(upPixmap);
    QIcon downIcon(downPixmap);
    for(int i = 20; i >= 0; --i)
    {
        hLayout[i] = new QHBoxLayout;
        label[i] = new QLabel("Floor " + (i==0 ? "G" : QString::number(i)) + ":");
        label[i]->setMinimumWidth(60);
        upButton[i] = new QPushButton;
        upButton[i]->setIcon(upIcon);
        upButton[i]->setIconSize(QSize(15,15));
        upButton[i]->setFixedSize(QSize(30,30));
        upButton[i]->setStyleSheet("background-color:white; border-radius: 4px; border: 1px solid gray");
        upButton[i]->setMaximumWidth(30);

        downButton[i] = new QPushButton;
        downButton[i]->setIcon(downIcon);
        downButton[i]->setIconSize(QSize(15,15));
        downButton[i]->setFixedSize(QSize(30,30));
        downButton[i]->setStyleSheet("background-color:white; border-radius: 4px; border: 1px solid gray");
        downButton[i]->setMaximumWidth(30);

        addUpButton[i] = new QPushButton("+");
        addUpButton[i]->setIcon(upIcon);
        addUpButton[i]->setIconSize(QSize(15,15));
        addUpButton[i]->setFixedSize(QSize(30,30));
        addUpButton[i]->setStyleSheet("background-color:white; border-radius: 4px; border: 1px solid gray");
        addUpButton[i]->setMinimumWidth(60);

        addDownButton[i] = new QPushButton("+");
        addDownButton[i]->setIcon(downIcon);
        addDownButton[i]->setIconSize(QSize(15,15));
        addDownButton[i]->setFixedSize(QSize(30,30));
        addDownButton[i]->setStyleSheet("background-color:white; border-radius: 4px; border: 1px solid gray");
        addDownButton[i]->setMinimumWidth(60);

        hLayout[i]->addWidget(label[i]);
        hLayout[i]->addWidget(upButton[i]);
        hLayout[i]->addWidget(downButton[i]);
        hLayout[i]->addSpacing(10);
        hLayout[i]->addWidget(addUpButton[i]);
        hLayout[i]->addWidget(addDownButton[i]);
        vLayout->addLayout(hLayout[i]);
    }
    upButton[20]->setEnabled(false);
    addUpButton[20]->setEnabled(false);
    downButton[0]->setEnabled(false);
    addDownButton[0]->setEnabled(false);

    //set connection
    for(int index = 0; index < 21; ++index)
    {
        connect(this->upButton[index], SIGNAL(clicked()), this, SLOT(onUpButtonClicked()));
        connect(this->downButton[index], SIGNAL(clicked()), this, SLOT(onDownButtonClicked()));
        connect(this->addUpButton[index], SIGNAL(pressed()), this, SLOT(onAddUpButtonPressed()));
        connect(this->addUpButton[index], SIGNAL(released()), this, SLOT(onAddUpButtonReleased()));
        connect(this->addDownButton[index], SIGNAL(pressed()), this, SLOT(onAddDownButtonPressed()));
        connect(this->addDownButton[index], SIGNAL(released()), this, SLOT(onAddDownButtonReleased()));
    }
    connect(floor, SIGNAL(numberUpPassengerchanged(int,int)), this, SLOT(onNumberUpPassengerchanged(int,int)));
    connect(floor, SIGNAL(numberDownPassengerchanged(int,int)), this, SLOT(onNumberDownPassengerchanged(int,int)));
    floor->start();
    floor->setPriority(QThread::HighestPriority);
}

FloorPanel::~FloorPanel()
{

}

FloorThread *FloorPanel::getFloor() const
{
    return floor;
}

Passenger FloorPanel::getPassengerGoingUpInFloor(int inFloor)
{
    return floor->getPassengerGoingUpInFloor(inFloor);
}

int FloorPanel::getPassengerGoingUpInFloorSize(int inFloor)
{
    return floor->getPassengerGoingUpInFloorSize(inFloor);
}

Passenger FloorPanel::getPassengerGoingDownInFloor(int inFloor)
{
    return floor->getPassengerGoingDownInFloor(inFloor);
}

int FloorPanel::getPassengerGoingDownSizeInFloor(int inFloor)
{
    return floor->getPassengerGoingDownSizeInFloor(inFloor);
}

void FloorPanel::setButtonOn(int button, int direction)
{
    if(direction == 1)
        upButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
    else
        downButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
}

void FloorPanel::onUpButtonClicked()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->upButton[button])
        {
            upButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
            emit upButtonClicked(button);
        }
    }
}

void FloorPanel::onDownButtonClicked()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->downButton[button])
        {
            downButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
            emit downButtonClicked(button);
        }
    }
}

void FloorPanel::onAddUpButtonPressed()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->addUpButton[button])
        {
            addUpButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
            floor->addUpPassenger(button);
        }
    }
}

void FloorPanel::onAddUpButtonReleased()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->addUpButton[button])
        {
            addUpButton[button]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
        }
    }
}

void FloorPanel::onAddDownButtonPressed()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->addDownButton[button])
        {
            addDownButton[button]->setStyleSheet("background-color:#32ced1; border-radius: 4px; border: 1px solid gray");
            floor->addDownPassenger(button);
        }
    }
}

void FloorPanel::onAddDownButtonReleased()
{
    for(int button = 0; button < 21; ++button)
    {
        if(sender() == this->addDownButton[button])
        {
            addDownButton[button]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
        }
    }
}

void FloorPanel::onOutRequestCompleted(int currentFloor, int requestDirection)
{
    if(requestDirection == 1)
        upButton[currentFloor]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
    if(requestDirection == 2)
        downButton[currentFloor]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
}

void FloorPanel::onNumberUpPassengerchanged(int inFloor, int upNumber)
{
    addUpButton[inFloor]->setText(upNumber > 0 ? "(" + QString::number(upNumber) + ")" : "+");
}

void FloorPanel::onNumberDownPassengerchanged(int inFloor, int downNumber)
{
    addDownButton[inFloor]->setText(downNumber > 0 ? "(" + QString::number(downNumber) + ")" : "+");
}

void FloorPanel::clearData()
{
    for(int i = 0; i < 21; ++i)
    {
        onNumberUpPassengerchanged(i, 0);
        onNumberDownPassengerchanged(i, 0);
        upButton[i]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
        downButton[i]->setStyleSheet("background-color:#ffffff; border-radius: 4px; border: 1px solid gray");
    }
    floor->clearData();
}
