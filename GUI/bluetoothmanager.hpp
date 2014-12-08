#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QDialog>
#include <QListWidgetItem>

#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

#include "remoteselector.hpp"

namespace Ui {
  class BluetoothManager;
}

class BluetoothManager : public QDialog
{
  Q_OBJECT

  Ui::BluetoothManager * ui;

public:
  explicit BluetoothManager(QWidget * parent = 0);
  ~BluetoothManager();

private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void connected(QString str);
  void on_pushButton_3_clicked();
};

#endif
