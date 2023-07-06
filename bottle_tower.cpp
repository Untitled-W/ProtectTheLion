
#include "bottle_tower.h"
#include "bottle.h"
#include "mainPage.h"

#include <math.h>
#include <assert.h>
#include "monster.h"
#include <QThread>
#include <QPainter>
#include <QUrl>
#include <QEventLoop>

double getDistance(QPointF a, QPointF b){
    //获取两点之间的距离
    QPointF c = a - b;
    return sqrt(c.x() * c.x() + c.y() * c.y());
}

BottleTower::BottleTower(QPointF _location,mainPage* _root) :
    Tower(Tower::TYPE_BOTTLE_TOWER, _location,_root){
    setData(Tower::DATA_TYPE, Tower::TYPE_BOTTLE_TOWER);
    setData(Tower::DATA_HURT, 10);
    setData(Tower::DATA_FREQ, 200);
    setData(Tower::DATA_LEVEL, 1);
    setData(Tower::DATA_VALUE, 10);
    setData(Tower::DATA_IMGPATH, "xxx");
    setData(Tower::DATA_RANGE, 200);
    setData(Tower::DATA_LOCATION, _location);
    setData(Tower::DATA_WIDTH, root->grid_l);//单位：pixel
    setData(Tower::DATA_HEIGHT, root->grid_l);
    attackTimer = new QTimer();
    targets = nullptr;
    setTransformOriginPoint(_location);
    frame = 0;
}

BottleTower::~BottleTower(){
}

void BottleTower::advance(int phase){
    if(phase == 0) return;
    renewTarget();
    if(endCheck())  return;

//    qDebug()<<(void*)targets;
    std::function<qreal(double)> angle = [&](double theita){
        theita = theita/atan2(0,-1)*180;
        return theita<0? 360+theita:theita;
    };
    QPointF loc = data(Tower::DATA_LOCATION).value<QPointF>();
    qreal targ = angle(atan2(targets->pos().y()-loc.y(), targets->pos().x()-loc.x()));
    qreal cur = rotation()<0? 360+rotation():rotation();
    if(abs(targ-cur)>5 && (targ-cur>=180||targ<cur))
        setRotation(rotation()-5);
    else if(abs(targ-cur)>5)
        setRotation(rotation()+5);

    if((frame/30.0) >= 60.0 / data(BottleTower::DATA_FREQ).toInt() && targets != nullptr){
        Attack();
        frame = 0;
        return;
    }
    frame++;
    if(frame==3) fire = false;
    if(endCheck())  return;
}

QRectF BottleTower::boundingRect() const{
    return QRectF(data(Tower::DATA_LOCATION).value<QPointF>().x() - data(Tower::DATA_WIDTH).toInt() / 2,
                  data(Tower::DATA_LOCATION).value<QPointF>().y() - data(Tower::DATA_HEIGHT).toInt() / 2,
                  data(Tower::DATA_WIDTH).toInt(), data(Tower::DATA_WIDTH).toInt());
}

void BottleTower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(!fire){
        QImage img(":/plant/src/plant/bottle.png");
        painter -> drawImage(boundingRect(), img, QRectF(0, 0, 86, 85));
    }
    else{
        QImage img(":/plant/src/plant/bottle_fire.png");
        painter -> drawImage(boundingRect(), img, QRectF(0, 0, 86, 95));
    }
}

monster* BottleTower::getNearest(){
    //找到最近的怪物
    QList<QGraphicsItem*> list = scene() -> items();
    monster* minItem = nullptr;
    int minDistance = 0x7ffffff;
    for(int i = 0;i < list.size(); i++){
        if(list[i] -> data(monster::DATA_TYPE).toInt() == 127){
            int tmpDistance = getDistance(list[i] -> pos(), data(Tower::DATA_LOCATION).value<QPointF>());
            if(tmpDistance < minDistance){
                minDistance = tmpDistance;
                minItem = (monster*)list[i];
            }
        }

    }
    if(minDistance > data(Tower::DATA_RANGE).toReal()){
        minItem = nullptr;
    }
    return minItem;
}

void BottleTower::renewTarget(){
    if(targets == nullptr || !monster::memoryExisted[targets] || getDistance(data(Tower::DATA_LOCATION).value<QPointF>(), targets -> pos()) > data(Tower::DATA_RANGE).toReal())
        targets = getNearest();
}

bool BottleTower::endCheck(){
    if(targets == nullptr || !monster::memoryExisted[targets] || getDistance(data(Tower::DATA_LOCATION).value<QPointF>(), targets -> pos()) > data(Tower::DATA_RANGE).toReal()){
        frame = 0;fire = false;
        targets = nullptr;
        this->scene()->update();
        return true;
    }
    return false;
}

void BottleTower::Attack(){
    if(endCheck())  return;
    bottle = new Bottle(data(Tower::DATA_LOCATION).value<QPointF>(), BottleTower::BOTTLE_SPEED, targets -> pos(), targets);
    scene() -> addItem(bottle);
    targets -> hurt(data(Tower::DATA_HURT).toDouble());
    frame++;
    if(endCheck())  return;
}
