#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setFixedHeight(this->height());
  this->setFixedWidth(this->width());

  tempProfile = new TemperatureProfiler(ui->graphicsView, this);
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

void MainWindow::newDotSlot() {
  static unsigned int nodeNumber = 0;
  qDebug() << "Created new dot";
  QListWidgetItem * it = new QListWidgetItem(QString::number(nodeNumber),ui->listDots);
  ui->listDots->setCurrentItem(it);
  nodeNumber++;
}

void MainWindow::duplicateDotSlot() {
  qDebug() << "Duplicated dot";
  newDotSlot();
}

void MainWindow::removeDotSlot() {
  qDebug() << "Removed dot";
  QListWidgetItem * it = ui->listDots->currentItem();
  ui->listDots->removeItemWidget(it);
  delete it;
}
