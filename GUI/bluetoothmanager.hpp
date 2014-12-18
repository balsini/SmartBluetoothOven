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
#include "chatclient.hpp"

namespace Ui {
  class BluetoothManager;
}

class BluetoothManager : public QDialog
{
  Q_OBJECT

  Ui::BluetoothManager * ui;
  ChatClient * client;

public:
  explicit BluetoothManager(QWidget * parent = 0);
  ~BluetoothManager();

private slots:
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void connected(QString str);
  void on_pushButton_3_clicked();

public slots:
  void sendMessage(QString str) {
    client->sendMessage(str);
  }
  void showMessage(QString sender, QString message);

signals:
  void btConnectionEstablished(ChatClient * client);
  void sentMessage(QString str);
  void clientDisconnected();
  void messageReceived(QString s1, QString s2);
};

#endif
