#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
  ui->timeEdit->setTime(QTime(hh,mm,ss));
}

void MainWindow::timeChangedSpinbox(QTime t)
{
  ui->dial_2->setValue(t.hour()*60*60 +
                       t.minute()*60 +
                       t.second());
}
