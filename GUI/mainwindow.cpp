#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setFixedHeight(this->height());
  this->setFixedWidth(this->width());

  tempProfile = new TemperatureProfiler(ui->graphicsView);
  connect(tempProfile->getScene(), SIGNAL(newDotSignal()), this, SLOT(updateDotSlot()));
}

MainWindow::~MainWindow()
{
  delete tempProfile;
  delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
  close();
}

void MainWindow::timeChangedDial(int i)
{
  int hh = i/60/60;
  int mm = (i/60)%60;
  int ss = i%60%60;
  ui->timeEditTime->setTime(QTime(hh,mm,ss));
}

void MainWindow::timeChangedSpinbox(QTime t)
{
  ui->dialTime->setValue(t.hour()*60*60 +
                         t.minute()*60 +
                         t.second());
}

void MainWindow::updateDotSlot()
{
  QListWidgetItem * it;
  ui->listDots->clear();
  for (int i=0; i<dotList.count(); i++) {
    it = new QListWidgetItem(QString::number(i),ui->listDots);
  }
  ui->listDots->setCurrentItem(it);
}

void MainWindow::newDotSlot()
{
  static int maxTime = 0;

  float time;
  float temp = 1 - (float)ui->dialTemperature->value() / ui->dialTemperature->maximum();

  if (ui->dialTime->value() > maxTime) {
    if (maxTime == 0)
      time = 1;
    else
      time = (float)ui->dialTime->value() / maxTime;
    maxTime = ui->dialTime->value();
  } else {
    time = (float)ui->dialTime->value() / maxTime;
  }

  qDebug() << "Created new dot with temp = "
           << temp;
  qDebug() << "Created new dot with time = "
           << time;

  dotList.append(Dot(time, temp));
  updateDotSlot();

  tempProfile->getScene()->update();
}

void MainWindow::duplicateDotSlot()
{
  qDebug() << "Duplicated dot";
  newDotSlot();
}

void MainWindow::removeDotSlot()
{
  qDebug() << "Removed dot";
  QListWidgetItem * it = ui->listDots->currentItem();
  ui->listDots->removeItemWidget(it);

  dotList.removeAt(ui->listDots->currentRow());

  delete it;

  tempProfile->getScene()->update();
}
