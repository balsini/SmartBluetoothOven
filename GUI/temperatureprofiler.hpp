#ifndef TEMPERATUREPROFILER_H
#define TEMPERATUREPROFILER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>

class Dot
{
  double _x, _y;
  bool _selected;

public:
  Dot(double x, double y): _x(x), _y(y), _selected(false) {}
  double x() const { return _x; }
  double y() const { return _y; }
  void x(double x) { _x = x; }
  void y(double y) { _y = y; }
  bool selected() const { return _selected; }
  void selected(bool s) { _selected = s; }
};

extern QList<Dot> dotList;
extern int currentTemperature;

class MyScene : public QGraphicsScene
{
  Q_OBJECT

signals:
  void newDotSignal();

public:
  MyScene(QWidget *parent = 0);
  ~MyScene() {}
  void addDot(double x, double y);
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
  void addDot(double x, double y);
  MyScene * getScene() { return scene; }
};

#endif // TEMPERATUREPROFILER_H
