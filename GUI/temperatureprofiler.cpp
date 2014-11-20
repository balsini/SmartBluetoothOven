#include "temperatureprofiler.hpp"

#include <QDebug>
#include <QWidget>
#include <QGraphicsSimpleTextItem>
#include <QLayout>
#include <QBrush>

#include "mainwindow.hpp"

QList<Dot> dotList;

/***********************
 *                     *
 * MyScene             *
 *                     *
 ***********************/

MyScene::MyScene(QWidget *parent) :
  QGraphicsScene(parent)
{
  setSceneRect(0,
               0,
               parent->width()/2,
               parent->height()/2);
  addRect(0, 0, 0, 0);
}

void MyScene::addDot(float x, float y)
{
  dotList.append(Dot(x, y));
  update();
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  if (dotList.count() > 0) {
    qDebug() << "Mouse premuto in [ "
             << mouseEvent->lastScenePos().x()
             << " , "
             << mouseEvent->lastScenePos().y()
             << " ]";

    float x = (float)(mouseEvent->lastScenePos().x()) / (((QWidget *)this->parent())->width());
    float y = (float)(mouseEvent->lastScenePos().y()) / (((QWidget *)this->parent())->height());

    addDot(x,y);

    qDebug() << "addDot("
             << x
             << ", "
             << y
             << ");";

    emit newDotSignal();
  }
}

void MyScene::update()
{
  this->clear();

  if (dotList.count() >= 1) {
    // Reorder Dots from left to right

    qDebug() << "Printing unordered list";

    for (int i=0; i<dotList.count(); i++) {
      qDebug() << "[" << dotList.at(i).x() << "," << dotList.at(i).y() << "]";
    }

    for (int i=0; i<dotList.count(); i++) {
      for (int j=0; j<dotList.count()-1; j++) {
        if (dotList.at(j).x() > dotList.at(j+1).x()) {
          dotList.insert(j, dotList.takeAt(j+1));
        }
      }
    }

    qDebug() << "Printing ordered list";

    for (int i=0; i<dotList.count(); i++) {
      qDebug() << "[" << dotList.at(i).x() << "," << dotList.at(i).y() << "]";
    }

    // Normalize dots "x" with the maximum one
    float maxX = dotList.last().x();
    float scale = 1 / maxX;

    for (int i=0; i<dotList.count(); i++) {
      Dot d = dotList.takeFirst();
      d.x(d.x() * scale);
      dotList.append(d);
    }

    qDebug() << "Printing normalized list";

    for (int i=0; i<dotList.count(); i++) {
      qDebug() << "[" << dotList.at(i).x() << "," << dotList.at(i).y() << "]";
    }


    QPen dotPen;
    dotPen.setWidth(2);

    // Draw dots
    for (int i=0; i<dotList.count(); i++) {

      if (dotList.at(i).selected())
        dotPen.setColor(Qt::red);
      else
        dotPen.setColor(Qt::darkGreen);

      int posX = ((QWidget *)this->parent())->width() * dotList.at(i).x();
      int posY = ((QWidget *)this->parent())->height() * dotList.at(i).y();
      addEllipse(posX,
                 posY,
                 10,
                 10,
                 dotPen)->setPos(-5,-5);
      QString txt1, txt2;
      txt1.append(QString::number((int)(dotList.at(i).x() * (float)maxTime)));
      txt1.append(" s, ");
      txt2.append(QString::number((int)((1.0 - dotList.at(i).y()) * (float)maxTemp)));
      txt2.append(" 'C");
      addSimpleText(txt1)->setPos(posX - 30,
                                  posY - 32);
      addSimpleText(txt2)->setPos(posX - 30,
                                  posY - 20);
    }

    // Draw lines
    QPen linePen;
    linePen.setWidth(2);

    addLine(((QWidget *)this->parent())->width() * 0,
            ((QWidget *)this->parent())->height() * 1,
            ((QWidget *)this->parent())->width() * dotList.at(0).x(),
            ((QWidget *)this->parent())->height() * dotList.at(0).y(),
            linePen);

    for (int i=1; i<dotList.count(); i++) {
      addLine(((QWidget *)this->parent())->width() * dotList.at(i).x(),
              ((QWidget *)this->parent())->height() * dotList.at(i).y(),
              ((QWidget *)this->parent())->width() * dotList.at(i-1).x(),
              ((QWidget *)this->parent())->height() * dotList.at(i-1).y(),
              linePen);
    }
  } else {
    // Working as common oven
  }
}

/***********************
 *                     *
 * TemperatureProfiler *
 *                     *
 ***********************/

TemperatureProfiler::TemperatureProfiler(QGraphicsView *view)
{
  this->view = view;
  scene = new MyScene(view);
  view->setScene(scene);

  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  QBrush viewbrush;
  viewbrush.setStyle(Qt::CrossPattern);
  viewbrush.setColor(Qt::lightGray);
  view->setBackgroundBrush(viewbrush);
}

TemperatureProfiler::~TemperatureProfiler()
{
  delete scene;
}

void TemperatureProfiler::addDot(float x, float y)
{
  if (x<=1 && x>=0 && y<=1 && y>=0)
    scene->addDot(x, y);
}
