#ifndef TEMPERATUREPROFILER_H
#define TEMPERATUREPROFILER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Dot
{
  float _x, _y;

public:
  Dot(float x, float y): _x(x), _y(y) {}
  float x() const { return _x; }
  float y() const { return _y; }
  void x(float x) { _x = x; }
  void y(float y) { _y = y; }
};

extern QList<Dot> dotList;

class MyScene : public QGraphicsScene
{
  Q_OBJECT

signals:
  void newDotSignal();

public:
  MyScene(QWidget *parent = 0);
  ~MyScene() {}
  void addDot(float x, float y);
  void update();

public slots:
  void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

class TemperatureProfiler
{
  QGraphicsView * view;
  MyScene * scene;

public:
  TemperatureProfiler(QGraphicsView *view);
  ~TemperatureProfiler();
  void addDot(float x, float y);
  MyScene * getScene() { return scene; }
};

#endif // TEMPERATUREPROFILER_H
