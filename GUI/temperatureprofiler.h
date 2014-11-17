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

class TemperatureProfiler : public QWidget
{
  Q_OBJECT

  QGraphicsView * view;
  MyScene * scene;

public:
  TemperatureProfiler(QGraphicsView * view, QWidget *parent);
  ~TemperatureProfiler() { delete scene; }
};

#endif // TEMPERATUREPROFILER_H
