#include "temperatureprofiler.h"

#include <QDebug>
#include <QWidget>
#include <QGraphicsSimpleTextItem>
#include <QLayout>

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
  /*
  //addRect(0,0,parent->width()/2-15, parent->height()/2-15)->setPos(5, 5);

  for (unsigned int i=25; i<=275; i-=25) {
      addSimpleText(QString::number(i))->setPos(20, i);
    }
    */
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  qDebug() << "Mouse premuto in [ "
           << mouseEvent->lastScenePos().x()
           << " , "
           << mouseEvent->lastScenePos().y()
           << " ]";

  addEllipse(mouseEvent->lastScenePos().x(),
             mouseEvent->lastScenePos().y(),
             8,
             8)->setPos(-4,-4);

  qDebug() << "Scene size [ "
           << ((QWidget *)this->parent())->width()
           << " , "
           << ((QWidget *)this->parent())->height()
           << " ]";
}

/***********************
 *                     *
 * TemperatureProfiler *
 *                     *
 ***********************/

TemperatureProfiler::TemperatureProfiler(QGraphicsView *view, QWidget *parent):
  QWidget(parent)
{
  this->view = view;
  scene = new MyScene(view);
  view->setScene(scene);

  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}
