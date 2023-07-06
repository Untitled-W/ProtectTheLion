#include "monster.h"
#include "mainpage.h"
#include "startpage.h"

int monster::monsNum = 0;
std::map<monster*,bool> monster::memoryExisted;

monster::monster(mainPage* _root, double _speed, double _scale ,int _HP, int _type):
    root(_root), type(_type),scale(_scale), HP(_HP), fullHP(_HP), speed(_speed)
{
//    qDebug()<<"start1"<<(void*)this<<QThread::currentThreadId();
    monsNum++;
    memoryExisted[this]=true;
    auto _pos = root->startpos;
    endg = root->endpos;
    _dir = &root->stepdir[0][0];
    setData(DATA_TYPE,MONSTER_TYPE);
    setPos((0.5+_pos.second)*mainPage::grid_l,(0.5+_pos.first)*mainPage::grid_l);
    curd = dir(_pos);
    curg = _pos;
    nxtg = add(_pos,curd);
    root->scene.addItem(this);
    root->allMons.append(this);
    connect(this,&monster::newWave,root,&mainPage::oneWave,Qt::QueuedConnection);
    connect(this,&monster::arriveEnd,root,&mainPage::cureAct);
//    workerThread = new QThread();
//    this->moveToThread(workerThread);
//    workerThread->start();
//    qDebug()<<"start2"<<(void*)this<<QThread::currentThreadId();
}

QRectF monster::boundingRect() const {
    return QRectF(-16*scale, -20*scale, 32*scale, 36*scale);
}

const std::pair<int,int> monster::dir(std::pair<int,int> x){
    if(startPage::gameOut)
        return {0,0};
    return _dir[x.first*mainPage::grid_w_num+x.second];
}

void monster::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    _imageNum = (_imageNum+1)%20;
    int t=(_imageNum <10)+1;
    t+=(type-1)*2;

    painter->drawPixmap(boundingRect(), images().at(t), images().at(t).rect());
    // Draw health bar background
    painter->setBrush(Qt::gray);
    painter->drawRect(-16*scale, -20*scale, 32*scale, 4*scale);
    // Draw health bar
    painter->setBrush(Qt::red);
    painter->drawRect(-16*scale, -20*scale, 32 * this->HP*scale/fullHP, 4*scale);
}

monster::~monster(){
//    qDebug()<<"end1"<<(void*)this<<QThread::currentThreadId();
    monsNum--;
    memoryExisted[this]=false;
    root->scene.removeItem(this);
    if(!root->rootPage->gameOut&&monsNum==0&&!root->waveGener->isActive()){
        emit newWave();
    }
//    workerThread->quit();
//    workerThread->wait();
//    delete workerThread;
//    qDebug()<<"end2"<<(void*)this<<QThread::currentThreadId();
}

void monster::hurt(double damage) {
    HP -= damage;
}

void monster::advance(int phase){
    if(!phase)   return;
    if(HP<=0){
        delete this;
        return;
    }
    std::function<bool()> checkTurn = [&](){
        return (x()-(0.5+nxtg.second)*mainPage::grid_l)*(curg.second-nxtg.second)<0 || (y()-(0.5+nxtg.first)*mainPage::grid_l)*(curg.first-nxtg.first)<0;
    };
    setPos(pos()+speed*QPointF(curd.second,curd.first));
    if(checkTurn()){
        curg = nxtg;
        curd = dir(nxtg);
        nxtg = add(nxtg,curd);
        if(curg==endg){
            emit arriveEnd(-scale);
            delete this;
        }
    }
}
