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
  //addRect(0,0,parent->width()/2-15, parent->height()/2-15)->setPos(5, 5);

  for (unsigned int i=25; i<=275; i-=25) {
    addSimpleText(QString::number(i))->setPos(20, i);
  }
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
 * MyView              *
 *                     *
 ***********************/

MyView::MyView(QWidget *parent) :
  QGraphicsView(parent)
{
  scene = new MyScene(this);
  this->setScene(scene);

  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setAlignment(Qt::AlignLeft | Qt::AlignTop);
}


/***********************
 *                     *
 * TemperatureProfiler *
 *                     *
 ***********************/

TemperatureProfiler::TemperatureProfiler(QWidget *parent):
  QWidget(parent)
{
  view = new MyView(this);
  setWindowTitle("Temperature Profile Editor");
  setFixedWidth(480);
  setFixedHeight(320);
  QHBoxLayout *l = new QHBoxLayout(this);
  this->setLayout(l);
  l->addWidget(view);
}
