#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QDialog>

#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>

namespace Ui {
class BluetoothManager;
}

class BluetoothManager : public QDialog
{
  Q_OBJECT

public:
  explicit BluetoothManager(QWidget *parent = 0);
  ~BluetoothManager();

private:
  Ui::BluetoothManager *ui;
};

#endif // BLUETOOTHMANAGER_H
