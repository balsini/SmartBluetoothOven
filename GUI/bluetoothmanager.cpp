#include "bluetoothmanager.h"
#include "ui_bluetoothmanager.h"

#include <QDebug>

BluetoothManager::BluetoothManager(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BluetoothManager)
{
  ui->setupUi(this);

  QList<QBluetoothHostInfo> btList;
  btList = QBluetoothLocalDevice::allDevices();
  for (int i=0; i<btList.count(); i++) {
    qDebug() << "Bt Device: " << btList[i].address().toString();
  }

  QBluetoothLocalDevice localDevice;
  QString localDeviceName;

  // Check if Bluetooth is available on this device
  if (localDevice.isValid()) {

      // Turn Bluetooth on
      localDevice.powerOn();

      // Read local device name
      localDeviceName = localDevice.name();

      // Make it visible to others
      localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

      // Get connected devices
      //QList<QBluetoothAddress> remotes;
      //remotes = localDevice.connectedDevices();

      qDebug() << "localDeviceName : " << localDeviceName;
  }
}

BluetoothManager::~BluetoothManager()
{
  delete ui;
}
