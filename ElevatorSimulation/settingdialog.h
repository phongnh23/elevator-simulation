#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QTreeWidgetItem>
#include "passenger.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    void setup();

private slots:
    void on_elevator1ComboBox_currentIndexChanged(int index);

    void on_elevator2ComboBox_currentIndexChanged(int index);

    void on_elevator3ComboBox_currentIndexChanged(int index);

    void on_elevator4ComboBox_currentIndexChanged(int index);

    void on_passengerWeightSlider_valueChanged(int value);

    void on_passengerSizeSlider_valueChanged(int value);

    void on_passengerFromFloorComboBox_currentIndexChanged(int index);

    void on_passengerToFloorComboBox_currentIndexChanged(int index);

    void on_add1Button_clicked();

    void on_add5Button_clicked();

    void on_add10Button_clicked();

    void on_floorComboBox_currentIndexChanged(int index);

    void on_resetButton_clicked();

    void on_okButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::SettingDialog *ui;

    bool randomFlag;
    int passengerWeight;
    int passengerSize;
    int passengerFromFloor;
    int passengerToFloor;
    int *elevatorFloor;
    QList<Passenger> *passengerInFloor;
    QTreeWidgetItem *treeItem;

signals:
    void initialDataSent(int *elevatorFloor, QList<Passenger> *passengerInFloor);

};

#endif // SETTINGDIALOG_H
