#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QTime>

#include "temperatureprofiler.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  Ui::MainWindow *ui;
  TemperatureProfiler *tempProfile;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void update() { tempProfile->getScene()->update(); }

private slots:
  void on_actionQuit_triggered();
  void timeChangedDial(int i);
  void timeChangedSpinbox(QTime t);
  void newDotSlot();
  void duplicateDotSlot();
  void removeDotSlot();

public slots:
  void updateDotSlot();
};

extern int maxTime;
extern int maxTemp;

#endif // MAINWINDOW_H
