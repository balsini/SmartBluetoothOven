#ifndef TEMPERATUREPROFILER_H
#define TEMPERATUREPROFILER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MyScene : public QGraphicsScene
{
  Q_OBJECT

public:
  MyScene(QWidget *parent = 0);

public slots:
  void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

class MyView : public QGraphicsView
{
  Q_OBJECT

  MyScene * scene;

public:
  MyView(QWidget *parent = 0);
  ~MyView() { delete scene; }
};

class TemperatureProfiler : public QWidget
{
  Q_OBJECT

  MyView * view;

public:
  TemperatureProfiler(QWidget *parent = 0);
  ~TemperatureProfiler() { delete view; }
};

#endif // TEMPERATUREPROFILER_H
