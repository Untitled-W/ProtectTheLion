
#ifndef BottleTOWER_H
#define BottleTOWER_H

#include "tower.h"
#include "bottle.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QCoreApplication>
#include <QWidget>

class BottleTower : public Tower
{
public:
    BottleTower(QPointF _location,mainPage* _root);
    ~BottleTower();

    void advance(int phase) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void renewTarget() override;
    void Attack() override;
    monster* getNearest();
    bool endCheck();
    static const int BOTTLE_SPEED = 30;//每毫秒的像素数
    QTimer* attackTimer;
    Bottle* bottle;

public slots:
//    void hurt();
public:
    int frame;
    static const int FPS = 30;
};

#endif // BottleTOWER_H
