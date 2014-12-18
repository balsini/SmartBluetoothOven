#include "bluetoothmanager.hpp"
#include "ui_bluetoothmanager.h"

#include "mainwindow.hpp"

#include <QDebug>

BluetoothManager::BluetoothManager(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BluetoothManager)
{
  ui->setupUi(this);
  MainWindow * mw = (MainWindow *)parent;
  connect(this, SIGNAL(btConnectionEstablished(ChatClient*)), mw, SLOT(btConnectionEstablished()));
  on_pushButton_clicked();
}

BluetoothManager::~BluetoothManager()
{
  delete ui;
}

void BluetoothManager::on_pushButton_clicked()
{
  ui->localDeviceComboBox->clear();

  QList<QBluetoothHostInfo> btList;
  btList = QBluetoothLocalDevice::allDevices();
  for (int i=0; i<btList.count(); i++) {
    QString deviceInfo = "";
    deviceInfo.append(btList[i].address().toString() + " (" + btList[i].name() + ")");
    ui->localDeviceComboBox->addItem(deviceInfo);
    //qDebug() << "Bt Device: " << btList[i].address().toString() + " - " + btList[i].name();
  }

  ui->pushButton_2->setEnabled(btList.count() > 0);
}

void BluetoothManager::on_pushButton_2_clicked()
{
  QString adapterAddress = ui->localDeviceComboBox->currentText().mid(0,17);
  const QBluetoothAddress adapter(adapterAddress);

  RemoteSelector remoteSelector(adapter);
  remoteSelector.startDiscovery(/*QBluetoothUuid(serviceUuid)*/);
  if (remoteSelector.exec() == QDialog::Accepted) {
    QBluetoothServiceInfo service = remoteSelector.service();

    qDebug() << "Connecting to service 2" << service.serviceName()
             << "on" << service.device().name();

    client = new ChatClient(this);

    ui->remoteTargetLabel->setText(QString("Connecting to: \"%1\"...").arg(service.device().name()));

    connect(client, SIGNAL(messageReceived(QString,QString)), this, SLOT(showMessage(QString,QString)));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
    connect(this, SIGNAL(sentMessage(QString)), client, SLOT(sendMessage(QString)));
    qDebug() << "Start client";
    client->startClient(service);

    //clients.append(client);
  }

  //ui->connectButton->setEnabled(true);
}

void BluetoothManager::on_pushButton_3_clicked()
{
  this->close();
  emit btConnectionEstablished(client);
}

void BluetoothManager::connected(QString str)
{
  ui->remoteTargetLabel->setText("Connection established with: \"" + str + "\"");
  ui->pushButton_3->setEnabled(true);
}

void BluetoothManager::showMessage(QString sender, QString message)
{
  emit messageReceived(sender, message);
}
