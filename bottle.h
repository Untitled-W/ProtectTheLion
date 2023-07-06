
#ifndef BOTTLE_H
#define BOTTLE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include "monster.h"


class Bottle : public QGraphicsItem
{

public:
    Bottle(QPointF _location, int speed, QPointF target, monster* _target);
    ~Bottle();
    void advance(int phase) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    int frame;
    static const int DATA_LOCATION = 1;
    static const int DATA_SPEED = 2;
    static const int DATA_WIDTH = 3;
    static const int DATA_HEIGHT = 4;
    static const int DATA_DIR = 5;
    static const int DATA_TIME = 6;
private:
    monster* target;
};

#endif // BOTTLE_H
