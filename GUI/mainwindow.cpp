#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <sstream>

#include <QFile>
#include <QFileDialog>

int maxTime = 900;
int maxTemp = 270;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  btManager = 0;
  cooking = false;
  ui->setupUi(this);
  this->setFixedHeight(this->height());
  this->setFixedWidth(this->width());

  tempProfile = new TemperatureProfiler(ui->graphicsView);
  connect(tempProfile->getScene(), SIGNAL(newDotSignal()), this, SLOT(updateDotSlot()));

  emit temperatureChanged(50);
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
    dotList[ui->listDots->currentRow()].x((double)i / maxTime);
  } else if (dotList.count() > 1 && ui->listDots->currentRow() == dotList.count() - 1
             && dotList[ui->listDots->currentRow() - 1].x() * maxTime < i) {
    // Right dot
    dotList[ui->listDots->currentRow()].x((double)i / maxTime );
    maxTime = i;
  }

  if (dotList.count() == 1)
    maxTime = i;

  tempProfile->getScene()->update();
}

void MainWindow::timeChangedSpinbox(QTime t)
{
  ui->dialTime->setValue(t.hour()*60*60 +
                         t.minute()*60 +
                         t.second());
  if (dotList.count() == 0)
    maxTime = ui->dialTime->value();
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
  if (dotList.count() > 0)
    dotList[ui->listDots->currentRow()].y(1.0 - (double)t / ui->dialTemperature->maximum());
  else
    currentTemperature = t;

  tempProfile->getScene()->update();
}

void MainWindow::saveTo(const QString &tmpfilename) {
  QString name = tmpfilename;

  if (name.mid(name.length() - 4, 4) != ".tpf")
    name.append(".tpf");

  QFile openFile(name);
  if (openFile.open(QIODevice::WriteOnly)) {
    filename = name;

    qDebug() << "Writing to file: " << name;

    for (int i=0; i<dotList.count(); ++i) {
      QString row = "";
      row.append(QString::number(dotList[i].x() * maxTime) + " ");
      row.append(QString::number((1.0 - dotList[i].y()) * ui->dialTemperature->maximum()) + "\n");

      openFile.write(row.toStdString().c_str());
    }

    updateTitle();
    openFile.close();

  } else {
    qDebug() << "Error while opening file";
  }
}

void MainWindow::on_actionSave_As_triggered()
{
  QString tmpfilename = QFileDialog::getSaveFileName(
        this,
        tr("Save File"),
        "./",
        "Temperature Profile File (*.tpf)"
        );
  saveTo(tmpfilename);
}

void MainWindow::on_actionSave_triggered()
{
  QString tmpfilename = filename;
  if (filename.length() == 0) {
    tmpfilename = QFileDialog::getSaveFileName(
          this,
          tr("Save File"),
          "./",
          "Temperature Profile File (*.tpf)"
          );
  }

  saveTo(tmpfilename);
}

void MainWindow::updateTitle()
{
  QString t = "SmartBluetoothOven";
  if (filename.length() > 0) {
    t.append(" - ");
    t.append(filename);
  }
  this->setWindowTitle(t);
}

void MainWindow::on_actionOpen_triggered()
{
  QString tmpfilename = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "./",
        "Temperature Profile File (*.tpf)"
        );

  QFile openFile(tmpfilename);
  if (openFile.open(QIODevice::ReadOnly)) {

    dotList.clear();

    for (;;) {
      QString s(openFile.readLine().data());
      if (s.count() == 0)
        break;

      std::stringstream ss(s.toStdString());

      double x, y;
      ss >> x;
      ss >> y;

      x = x / tempProfile->getScene()->width();
      y = 1.0 - y / maxTemp;

      qDebug() << "X : " << x << "\tY : " << y;

      Dot d(x, y);
      dotList.append(d);

      ui->buttonDotRemove->setEnabled(true);
    }

    tempProfile->getScene()->update();
    updateDotSlot();

    openFile.close();

    filename = tmpfilename;
    updateTitle();
  } else {
    qDebug() << "Error while opening file";
  }
}

void MainWindow::on_actionConnection_triggered()
{
  if (!btManager) {
    btManager = new BluetoothManager(this);
    connect(btManager, SIGNAL(messageReceived(QString,QString)),
            this, SLOT(btMessageReceived(QString,QString)));
  }
  btManager->show();
}

void MainWindow::btConnectionEstablished()
{
  qDebug() << "MainWindow::btConnectionEstablished";

  QByteArray m;
  m.append('p');
  btManager->sendMessage(m);
  ui->buttonStart->setEnabled(true);
}

int MainWindow::desiredTemperature(int time)
{
  if (dotList.empty()) {
    return ui->dialTemperature->value();
  } else {
    int i;

    for (i=0; i<dotList.count(); ++i) {
      if (dotList[i].x() * maxTime > time)
        break;
    }

    if (i == dotList.count())
      return (1.0 - dotList.last().y()) * maxTemp;

    Dot rightDot = dotList[i];
    Dot leftDot(0.0, 1.0);
    if (i > 0)
      leftDot = dotList[i-1];

    double x_1, x_2, y_1, y_2;

    x_1 = leftDot.x() * maxTime;
    y_1 = (1.0 - leftDot.y()) * maxTemp;

    x_2 = rightDot.x() * maxTime;
    y_2 = (1.0 - rightDot.y()) * maxTemp;

    int ret = (y_2 - y_1) / (x_2 - x_1) * ((double)time - x_1) + y_1;

    return ret;
  }
}

void MainWindow::btMessageReceived(QString sender, QString message)
{
  qDebug() << "Received message from: " << sender;
  qDebug() << message;

  QString ovenTemperatureS = message.mid(2,3);
  int ovenTemperature = ovenTemperatureS.toInt();

  if (cooking) {
    int currentTime = timer.elapsed() / 1000;

    ui->horizontalSlider->setValue(currentTime * 100 / maxTime);
    ui->lcdCurrent->display(ovenTemperature);
    dotListOven.append(Dot(currentTime, 1.0-((double)ovenTemperature/maxTemp)));

    int dt = desiredTemperature(currentTime);

    ui->lcdDesired->display(dt);

    tempProfile->getScene()->update();

    if (currentTime > maxTime) {
      btManager->sendMessage("d");
      on_buttonStop_clicked();
      return;
    }
    char buffer [50];
    sprintf(buffer, "t,%03d", dt);
    btManager->sendMessage(buffer);
  }
}

void MainWindow::on_buttonStart_clicked()
{
  btManager->sendMessage("s");
  ui->buttonStart->setEnabled(false);
  ui->buttonStop->setEnabled(true);
  cooking = true;

  timer.start();
  ui->horizontalSlider->setValue(0);
}

void MainWindow::on_buttonStop_clicked()
{
  cooking = false;

  ui->buttonStart->setEnabled(true);
  ui->buttonStop->setEnabled(false);
  dotListOven.clear();
}
