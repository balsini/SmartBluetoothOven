#include "mainwindow.hpp"
#include "ui_mainwindow.h"

int maxTime = 0;
int maxTemp = 270;

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

  if (ui->listDots->currentRow() > 0 && dotList[ui->listDots->currentRow() - 1].x() * maxTime < i
      && ui->listDots->currentRow() < dotList.count() - 1 && dotList[ui->listDots->currentRow() + 1].x() * maxTime > i) {
    dotList[ui->listDots->currentRow()].x((double)i / maxTime);
  } else if (ui->listDots->currentRow() == 0
             && ui->listDots->currentRow() < dotList.count() - 1 && dotList[ui->listDots->currentRow() + 1].x() * maxTime > i) {
    // Left dot
    dotList[ui->listDots->currentRow()].x((double)i / maxTime );
  } else if (dotList.count() > 1 && ui->listDots->currentRow() == dotList.count() - 1
             && dotList[ui->listDots->currentRow() - 1].x() * maxTime < i) {
    // Right dot
    dotList[ui->listDots->currentRow()].x((double)i / maxTime );
    maxTime = i;
  }

  tempProfile->getScene()->update();
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
  ui->buttonDotNew->setEnabled(false);

  double time;
  double temp = 1 - (double)ui->dialTemperature->value() / ui->dialTemperature->maximum();

  if (ui->dialTime->value() > maxTime) {
    if (maxTime == 0)
      time = 1;
    else
      time = (double)ui->dialTime->value() / maxTime;
    maxTime = ui->dialTime->value();
  } else {
    time = (double)ui->dialTime->value() / maxTime;
  }

  qDebug() << "Created new dot with temp = "
           << temp;
  qDebug() << "Created new dot with time = "
           << time;

  dotList.append(Dot(time, temp));
  updateDotSlot();

  tempProfile->getScene()->update();

  ui->buttonDotRemove->setEnabled(true);
}

void MainWindow::removeDotSlot()
{
  qDebug() << "Removed dot";
  QListWidgetItem * it = ui->listDots->currentItem();
  ui->listDots->removeItemWidget(it);

  dotList.removeAt(ui->listDots->currentRow());

  delete it;

  tempProfile->getScene()->update();

  if (dotList.count() == 0) {
    ui->buttonDotNew->setEnabled(true);
    ui->buttonDotRemove->setEnabled(false);
  }
}

void MainWindow::anotherDotSelected(int n)
{
  static int lastDotSelected = 0;

  if (n >= 0 && dotList.count() > 1) {
    if (lastDotSelected < dotList.count())
      dotList[lastDotSelected].selected(false);
    dotList[n].selected(true);
    lastDotSelected = n;

    ui->dialTemperature->setValue((1.0 - dotList[n].y()) * ui->dialTemperature->maximum());
    ui->dialTime->setValue(dotList[n].x() * maxTime);

    tempProfile->getScene()->update();
  }
}

void MainWindow::temperatureChanged(int t)
{
  dotList[ui->listDots->currentRow()].y(1 - (double)t / ui->dialTemperature->maximum());

  tempProfile->getScene()->update();
}
