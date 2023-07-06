
#include "bottle.h"
#include <QGraphicsView>
#include <QEventLoop>
#include <exception>

bool flag = false;
double getd(QPointF a, QPointF b){
    //获取两点之间的距离
    return sqrt((a.x() - b.x()) * (a.x() - b.x())
                + (a.y() - b.y()) * (a.y() - b.y()));
}

Bottle::Bottle(QPointF _location, int speed, QPointF target, monster* _target) : target(_target)
{
    setData(Bottle::DATA_LOCATION, _location);
    setData(Bottle::DATA_SPEED, speed);
    setData(Bottle::DATA_WIDTH, 40);
    setData(Bottle::DATA_HEIGHT, 40);
    QPointF dir((target - _location) / getd(_location, target));
    setData(Bottle::DATA_DIR, dir);
    setTransformOriginPoint(_location);
    setRotation(rand());
    frame = 0;
}

Bottle::~Bottle(){

    //这里制作爆炸的动画
}

QRectF Bottle::boundingRect() const {
    return QRectF(data(Bottle::DATA_LOCATION).value<QPointF>().x() - data(Bottle::DATA_WIDTH).toInt() / 2,
                  data(Bottle::DATA_LOCATION).value<QPointF>().y() - data(Bottle::DATA_HEIGHT).toInt() / 2,
                  data(Bottle::DATA_WIDTH).toInt(), data(Bottle::DATA_WIDTH).toInt());
}

void Bottle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    Q_UNUSED(option); //标明该参数暂时没有使用
    Q_UNUSED(widget);

    QImage img(":/plant/src/plant/bullet.png");
    painter -> drawImage(boundingRect(), img, QRectF(0, 0, 19, 19));
}

void Bottle::advance(int phase){
    if(phase == 1){
        if(flag){
            frame++;
        }
        if(frame >= 3){
            flag = false;
            Bottle::~Bottle();
            return;
        }
        setPos(pos()
               + data(Bottle::DATA_DIR).value<QPointF>() * data(Bottle::DATA_SPEED).toInt());
        //qDebug() <<  mapToScene(target -> pos()).x() - mapFromScene(scenePos()).x();
        if(!monster::memoryExisted[target]){
            flag = true;
            return;
        }
        if(collidesWithItem(target)){
            flag = true;
        }
    }
}
