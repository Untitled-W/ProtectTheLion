#ifndef MONSTER_H
#define MONSTER_H

#include <QObject>
#include <vector>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <qdebug.h>
#include <QThread>
#include <QGraphicsItem>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>



class mainPage;

class monster : public QObject, public QGraphicsItem
{
    Q_OBJECT
//    QThread* workerThread;

public:
    int HP;
    int type;
    int fullHP;
    double scale;
    double speed;
    mainPage* root;

    void hurt(double damage);  //被攻击

    int _imageNum = 0;
    static QList<QPixmap>& images()
    {
        QSize newSize(50, 50);
        static QList<QPixmap> images = {QPixmap(":/plant/src/plant/ghost").scaled(newSize),
                                        QPixmap(":/plant/src/plant/rabbit1").scaled(newSize),
                                        QPixmap(":/plant/src/plant/rabbit2").scaled(newSize),
                                        QPixmap(":/plant/src/plant/dog1").scaled(newSize),
                                        QPixmap(":/plant/src/plant/dog2").scaled(newSize),
                                        QPixmap(":/plant/src/plant/horse1").scaled(newSize),
                                        QPixmap(":/plant/src/plant/horse2").scaled(newSize)};
        return images;
    }
    std::pair<int,int> nxtg;    //下一个格子
    std::pair<int,int> curd;    //当前方向
    std::pair<int,int> curg;    //当前格子
    std::pair<int,int> endg;    //终点格子
    const std::pair<int,int>* _dir; //全局移动方向
    const std::pair<int,int> dir(std::pair<int,int>);

    static int monsNum;
    static const int DATA_TYPE = 128;
    static const int MONSTER_TYPE = 127;
    static std::map<monster*,bool> memoryExisted;


    explicit monster(mainPage* _root, double speed=1, double scale=1, int blood=100, int type=1);
    virtual void advance(int phase) override;
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
    std::pair<int,int> add(std::pair<int,int> x, std::pair<int,int> y){
        return {x.first+y.first, x.second+y.second};
    }
    ~monster();


signals:
    void newWave();
    void arriveEnd(int);
};

#endif // MONSTER_H
