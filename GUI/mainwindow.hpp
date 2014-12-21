#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QTime>

#include "temperatureprofiler.hpp"
#include "bluetoothmanager.hpp"
#include "chatclient.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  Ui::MainWindow * ui;
  TemperatureProfiler * tempProfile;
  BluetoothManager * btManager;
  QString filename;
  QTime timer;
  bool cooking;

  int desiredTemperature(int time);

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void update() { tempProfile->getScene()->update(); }

private slots:
  void on_actionQuit_triggered();
  void timeChangedDial(int i);
  void timeChangedSpinbox(QTime t);
  void newDotSlot();
  void removeDotSlot();
  void anotherDotSelected(int n);
  void temperatureChanged(int t);

  void on_actionOpen_triggered();

  void on_actionSave_triggered();
  void saveTo(const QString &tmpfilename);

  void on_actionSave_As_triggered();
  void updateTitle();

  void on_actionConnection_triggered();

  void on_buttonStart_clicked();

  void on_buttonStop_clicked();

  void on_actionExport_Temperature_triggered();

public slots:
  void updateDotSlot();

  void btConnectionEstablished();
  void btMessageReceived(QString sender, QString message);
};

extern int maxTime;
extern int maxTemp;

#endif // MAINWINDOW_H
