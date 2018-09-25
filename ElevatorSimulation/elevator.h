#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QWidget>
#include <QPushButton>
#include "elevatorthread.h"

namespace Ui {
class Elevator;
}

class Elevator : public QWidget
{
    Q_OBJECT

public:
    explicit Elevator(QWidget *parent = 0, int toNumber = 1);
    ~Elevator();

    int getCurrentFloor();
    void setCurrentFloor(int value);

//    int getNextFloor();//

    Direction getDirection();

    ElevatorThread *getOperation() const;

    int getOutRequest(int floor);
//    void setOutRequest(int floor, int request);//

//    void setOpenRequest();//

    int getWeight() const;
    void setWeight(int value);

    int getSize() const;
    void setSize(int value);

    void clearData();

    void setSpeed(int value);

private slots:
    void on_closeButton_pressed();
    void on_openButton_pressed();
    void on_closeButton_released();
    void on_openButton_released();

    void on_button_0_clicked();
    void on_button_1_clicked();
    void on_button_2_clicked();
    void on_button_3_clicked();
    void on_button_4_clicked();
    void on_button_5_clicked();
    void on_button_6_clicked();
    void on_button_7_clicked();
    void on_button_8_clicked();
    void on_button_9_clicked();
    void on_button_10_clicked();
    void on_button_11_clicked();
    void on_button_12_clicked();
    void on_button_13_clicked();
    void on_button_14_clicked();
    void on_button_15_clicked();
    void on_button_16_clicked();
    void on_button_17_clicked();
    void on_button_18_clicked();
    void on_button_19_clicked();
    void on_button_20_clicked();

    void onPainterChanged();

    void onStateChanged(QString state);
    void onCurrentFloorChanged(int currentFloor);
    void onInRequestCompleted(int currentFloor);

    void onElevatorAttributeChanged(int weight, int size, int floorButton);    // modify weight and size labels, and floorButton light

private:
    Ui::Elevator *ui;
    ElevatorThread *operation;

    bool isFloorButtonClicked[21];
    int speed;

    void paintEvent(QPaintEvent *event) override;
    void buttonClickFunction(QPushButton* button, int index);

signals:
    void floorButtonClicked(int floor);
    void floorButtonUnclicked(int floor);

};

#endif // ELEVATOR_H
