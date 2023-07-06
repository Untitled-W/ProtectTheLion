
#ifndef TOWER_H
#define TOWER_H

#include <string>
#include <QGraphicsItem>
#include "monster.h"
#include <QWidget>

class mainPage;

class Tower : public QGraphicsItem
{
public:
    int x,y;
    mainPage* root;
    Tower(int _type, QPointF _location, mainPage* _root);
    ~Tower();
    QRectF boundingRect()const  override = 0 ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override = 0;

    virtual void Attack() = 0;//攻击
    virtual void renewTarget() = 0;//更新目标列表

    static const int TYPE_BOTTLE_TOWER = 1;
    static const int TPYE_SUNTOWER = 2;
    static const int DATA_LEVEL = 0;//在使用setData和data方法的时候下面这些
    static const int DATA_TYPE = 1;
    static const int DATA_VALUE = 2;
    static const int DATA_HURT = 3;
    static const int DATA_FREQ = 4;
    static const int DATA_TARGET_NUM = 6;
    static const int DATA_IMGPATH = 5;
    static const int DATA_RANGE = 7;
    static const int DATA_ANGLE = 8;
    static const int DATA_LOCATION = 9;
    static const int DATA_WIDTH = 10;
    static const int DATA_HEIGHT = 11;
    static const int DATA_LOCATION_X = 12;
    static const int DATA_LOCATION_Y = 12;
    monster* targets;//攻击的目标的数组的头指针，更新的时候需要delete
    bool fire=false;

    //virtual void getNewTarget(QGraphicsItem*) = 0;

protected:
    virtual void advance(int phase) override = 0;

private:

    int type;

};

#endif // TOWER_H
