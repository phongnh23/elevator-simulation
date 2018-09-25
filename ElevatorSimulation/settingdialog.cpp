#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    elevatorFloor = new int [4];
    passengerInFloor = new QList<Passenger> [21];
    treeItem = new QTreeWidgetItem [2];

    for(int i = 0; i < 21; ++i)
    {
        ui->elevator1ComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->elevator2ComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->elevator3ComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->elevator4ComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->passengerFromFloorComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->passengerToFloorComboBox->addItem(i == 0 ? "G" : QString::number(i));
        ui->floorComboBox->addItem(i == 0 ? "G" : QString::number(i));
    }
    ui->passengerToFloorComboBox->addItem("?");
    ui->passengerFromFloorComboBox->addItem("?");
    setup();
}

SettingDialog::~SettingDialog()
{
    if(passengerInFloor != NULL)
    {
        delete [] passengerInFloor;
        passengerInFloor = NULL;
    }
    if(treeItem != NULL)
    {
        delete [] treeItem;
        treeItem = NULL;
    }
    if(elevatorFloor != NULL)
    {
        delete [] elevatorFloor;
        elevatorFloor = NULL;
    }
    delete ui;
}

void SettingDialog::setup()
{
    for(int i = 0; i < 21; ++i)
        passengerInFloor[i].clear();

    for(int i = 0; i < 4; ++i)
    {
        elevatorFloor[i] = 1;
    }
    passengerWeight = 60;
    passengerSize = 1000;
    passengerFromFloor = 1;
    passengerToFloor = 17;
    randomFlag = false;

    ui->checkBox->setChecked(false);
    ui->elevator1ComboBox->setCurrentIndex(elevatorFloor[0]);
    ui->elevator2ComboBox->setCurrentIndex(elevatorFloor[1]);
    ui->elevator3ComboBox->setCurrentIndex(elevatorFloor[2]);
    ui->elevator4ComboBox->setCurrentIndex(elevatorFloor[3]);
    ui->passengerFromFloorComboBox->setCurrentIndex(passengerFromFloor);
    ui->passengerToFloorComboBox->setCurrentIndex(passengerToFloor);
    ui->floorComboBox->setCurrentIndex(1);
    ui->passengerWeightLabel->setText(QString::number(passengerWeight));
    ui->passengerSizeLabel->setText(QString::number(passengerSize));
    ui->passengerWeightSlider->setValue(passengerWeight);
    ui->passengerSizeSlider->setValue(passengerSize);

    on_floorComboBox_currentIndexChanged(ui->floorComboBox->currentIndex());
    Passenger::iD = 1;
}

void SettingDialog::on_elevator1ComboBox_currentIndexChanged(int index)
{
    elevatorFloor[0] = index;
}

void SettingDialog::on_elevator2ComboBox_currentIndexChanged(int index)
{
    elevatorFloor[1] = index;
}

void SettingDialog::on_elevator3ComboBox_currentIndexChanged(int index)
{
    elevatorFloor[2] = index;
}


void SettingDialog::on_elevator4ComboBox_currentIndexChanged(int index)
{
    elevatorFloor[3] = index;
}

void SettingDialog::on_passengerWeightSlider_valueChanged(int value)
{
    ui->passengerWeightLabel->setText(QString::number(value));
    passengerWeight = value;
}

void SettingDialog::on_passengerSizeSlider_valueChanged(int value)
{
    ui->passengerSizeLabel->setText(QString::number(value));
    passengerSize = value;
}

void SettingDialog::on_passengerFromFloorComboBox_currentIndexChanged(int index)
{
    if(index < 21)
    {
        passengerFromFloor = index;
    }
    else
        passengerFromFloor = -1;
}

void SettingDialog::on_passengerToFloorComboBox_currentIndexChanged(int index)
{
    if(index < 21)
    {
        passengerToFloor = index;
    }
    else
        passengerToFloor = -1;
}

void SettingDialog::on_add1Button_clicked()
{
    Passenger inPassenger(randomFlag ? -1 : passengerWeight, randomFlag ? -1 : passengerSize, passengerFromFloor, passengerToFloor);
//    qDebug() << inPassenger.getWeight() << inPassenger.getSize() << inPassenger.getFromFloor() << inPassenger.getDestinationFloor();
    passengerInFloor[inPassenger.getFromFloor()].append(inPassenger);
    if(passengerFromFloor >= 0)
        ui->floorComboBox->setCurrentIndex(ui->passengerFromFloorComboBox->currentIndex());
    on_floorComboBox_currentIndexChanged(ui->floorComboBox->currentIndex());
}

void SettingDialog::on_add5Button_clicked()
{
    for(int i = 0; i < 5; ++i)
    {
        Passenger inPassenger(randomFlag ? -1 : passengerWeight, randomFlag ? -1 : passengerSize, passengerFromFloor, passengerToFloor);
        passengerInFloor[inPassenger.getFromFloor()].append(inPassenger);
    }
    if(passengerFromFloor >= 0)
        ui->floorComboBox->setCurrentIndex(ui->passengerFromFloorComboBox->currentIndex());
    on_floorComboBox_currentIndexChanged(ui->floorComboBox->currentIndex());
}

void SettingDialog::on_add10Button_clicked()
{
    for(int i = 0; i < 10; ++i)
    {
        Passenger inPassenger(randomFlag ? -1 : passengerWeight, randomFlag ? -1 : passengerSize, passengerFromFloor, passengerToFloor);
        passengerInFloor[inPassenger.getFromFloor()].append(inPassenger);
    }
    if(passengerFromFloor >= 0)
        ui->floorComboBox->setCurrentIndex(ui->passengerFromFloorComboBox->currentIndex());
    on_floorComboBox_currentIndexChanged(ui->floorComboBox->currentIndex());
}

void SettingDialog::on_floorComboBox_currentIndexChanged(int index)
{
    if(treeItem != NULL)
    {
        delete [] treeItem;
        treeItem = NULL;
    }

    int listSize = passengerInFloor[index].size();
    treeItem = new QTreeWidgetItem [listSize];
    for(int i = 0; i < listSize; ++i)
    {
        treeItem[i].setText(0, QString::number(passengerInFloor[index].at(i).getPassengerId()));
        treeItem[i].setText(1, QString::number(passengerInFloor[index].at(i).getDestinationFloor()));
        treeItem[i].setText(2, QString::number(passengerInFloor[index].at(i).getWeight()));
        treeItem[i].setText(3, QString::number(passengerInFloor[index].at(i).getSize()));
        ui->treeWidget->addTopLevelItem(treeItem + i);
    }
    ui->treeWidget->scrollToBottom();
}

void SettingDialog::on_resetButton_clicked()
{
    setup();
}

void SettingDialog::on_okButton_clicked()
{
    emit initialDataSent(elevatorFloor, passengerInFloor);
//    close();
}

void SettingDialog::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->passengerWeightSlider->setEnabled(true);
        ui->passengerSizeSlider->setEnabled(true);
        randomFlag = false;
    }
    else
    {
        ui->passengerWeightSlider->setEnabled(false);
        ui->passengerSizeSlider->setEnabled(false);
        randomFlag = true;
    }
}
