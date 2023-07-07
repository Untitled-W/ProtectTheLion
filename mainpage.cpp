
#include "mainpage.h"

double lasttime;
startPage* mainPage::rootPage = nullptr;

mainPage::mainPage(QWidget* parent)
    : QWidget(parent)
{
    ui_setting();
    generate_road();
    graphics_setting();
    popup_setting();
    connect(this,&mainPage::_gameFail,this,&mainPage::gameFail);
    connect(this,&mainPage::_gameWin,this,&mainPage::gameWin);
    qDebug()<<"page:"<<(void*)this<<QThread::currentThreadId();
}

void mainPage::graphics_setting() {
    scene.setSceneRect(0, 0, grid_w_num*grid_l, grid_h_num*grid_l);
    view = new QGraphicsView(this);
    view->setScene(&scene);
    view->setStyleSheet("background: transparent;border:1px");
    view->setGeometry(padding_left, padding_top, grid_w_num*grid_l, grid_h_num*grid_l);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    view->show();
    timer = new QTimer(this);
    waveGener = new QTimer(this);
    connect(timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    connect(waveGener,&QTimer::timeout,[&](){
        if(newgenermons>=monsWave[curWave-1]){
            waveGener->stop();
            return;
        }
        newgenermons++;
        new monster(this, monsSpeed[curWave-1][newgenermons-1], monsScale[curWave-1][newgenermons-1], monsHP[curWave-1][newgenermons-1], monsType[curWave-1][newgenermons-1]);
        qDebug()<<newgenermons;
    });
    timer->start(1000 / fps);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,[](){qDebug()<<clock()-lasttime;lasttime=clock();});
}

void mainPage::ui_setting() {

    lasttime = clock();

    this->setFixedSize(window_width, window_height);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setWindowFlags(windowFlags()& ~Qt::WindowCloseButtonHint);

    backgroundpic = setLabel(":/img/src/img/background.png",QRect(0, 0, window_width, window_height));

    left = labelstart,width = 3*buttonsize;
    GPABar = setLabel("");

    left+=width+labelpadding;
    moneyBar = setLabel(":/img/src/img/coinpic.png");
    moneyBar->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    moneyBar->setText(QString("%1   ").arg(money));
    moneyBar->setFont(QFont("STHupo",20));

    left+=width+labelpadding;width=5*buttonsize;
    progressBar = setLabel(":/img/src/img/progresspic.png");
    progressBar->setText("0 / "+QString::number(totalWave));
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setFont(QFont("STHupo",20));

    left+=width+labelpadding;width=2*buttonsize;
    speedBar = setButton(":/img/src/img/speed1.png", &mainPage::speedAct);
    speedBar->setFlat(true);
    left+=width+labelpadding;width=buttonsize;
    stopBar = setButton(":/img/src/img/stoppic.png", &mainPage::stopAct);
    left+=width+labelpadding;width=buttonsize;
    backBar = setButton(":/img/src/img/menupic.png", &mainPage::closeAct);

    left=200;width=skillsize;
    cureBar = setButton(":/img/src/img/cure_magic.png", &mainPage::cureAct, QRect(left, window_height-skillsize-10, skillsize, skillsize));
    left+=width+labelpadding;
    cheerBar = setButton(":/img/src/img/radical_magic.png", &mainPage::cheerAct, QRect(left, window_height-skillsize-10, skillsize, skillsize));
    left+=width+labelpadding;
    boomBar = setButton(":/img/src/img/bomb_magic.png", &mainPage::bombAct, QRect(left, window_height-skillsize-10, skillsize, skillsize));
    left+=width+labelpadding;
    iceBar = setButton(":/img/src/img/frozen_magic.png", &mainPage::iceAct, QRect(left, window_height-skillsize-10, skillsize, skillsize));

    focusPic = new QLabel(this);
    focusPic->setGeometry(QRect(0,0,grid_l, grid_l));
    focusPic->setStyleSheet("border-radius:5px;border:1px solid red;");
    focusPic->hide();

    rangePic = new QLabel(this);
    rangePic->setStyleSheet("background-color:grey;");
    rangePic->hide();

}

void mainPage::generate_road() {

    std::pair<int,int> dir[4] = {{-1,0},{0,-1},{1,0},{0,1}};//上，左，下，右
    QString _pic[4][4] = {
        {QString("vertical%1").arg(1),"turning_bl",                "",                          "turning_br"},
        {"turning_tr",                 QString("straight%1").arg(1),"turning_br",                 ""},
        {"",                          "turning_tl",                 QString("vertical%1").arg(1),"turning_tr"},
        {"turning_tl",                 "",                          "turning_bl",                 QString("straight%1").arg(1)}
    };
    std::function<QString(int lastd, int curd)> pic = [&](int lastd, int curd){
        if(lastd == -1) return QString("Main%1").arg(curd);
        if(curd == -1)  return QString("End%1").arg((lastd+2)%4);
        return _pic[lastd][curd];
    };
    std::function<void(std::pair<int,int>, QString s)> setRoad = [&](std::pair<int,int> pos,QString s){
        int x = pos.first, y = pos.second;
        notEmpty[x][y] = true;
        setLabel(":/img/src/img/"+s+".png",QRect(padding_left+grid_l*y,padding_top+grid_l*x,grid_l,grid_l));
    };
    std::function<bool(std::pair<int,int>,int,int)> check = [&](std::pair<int,int> pos, int d, int len){
        for(int k=0;k<len;k++){
            pos = add(pos,dir[d]);
            int x = pos.first, y = pos.second;
            if(x<0||y<0||x>=grid_h_num||y>=grid_w_num)
                return 0;
            if(notEmpty[x][y])
                return 0;
        }
        return 1;
    };

    std::pair<int,int> stpos = {rand()%grid_h_num, rand()%grid_w_num};
    startpos = stpos;
    int lastd = -1;
    bool stopFlag = false;
    while(!stopFlag){
        stopFlag = true;
        for(int trial = 10; trial>0; trial--){
            int curd = rand()%4;
            int numr = rand()%2+2;
            if(check(stpos,curd,numr)){
                stopFlag = false;
                setRoad(stpos,pic(lastd,curd));
                stepdir[stpos.first][stpos.second] = dir[curd];
                stpos = add(stpos,dir[curd]);
                for(int k=1; k<numr-1; k++){
                    setRoad(stpos,pic(curd,curd));
                    stepdir[stpos.first][stpos.second] = dir[curd];
                    stpos = add(stpos,dir[curd]);
                }
                lastd = curd;
                break;
            }
        }
    }
    endpos = stpos;
    setRoad(stpos,pic(lastd,-1));
    theLionPic = setLabel(QString(":/plant/src/plant/%1.png").arg(totalGPA),QRect(padding_left+grid_l*stpos.second,padding_top+grid_l*stpos.first,75,90));
}

class plantAct{
public:
    int t;
    mainPage* root;
    plantAct(mainPage* _root, int _t):root(_root),t(_t){}
    void operator()(){
        Tower* newPlant;
        QPointF p = QPointF(root->grid_l*(0.5+root->focusPos.second),root->grid_l*(0.5+root->focusPos.first));
        qDebug()<<p;
        switch(t){
        case 0:newPlant = new BottleTower(p,root);break;
        }
        root->planting_window->hide();
        root->allPlants.append(newPlant);
        root->scene.addItem(newPlant);
        root->isPlant[root->focusPos.first][root->focusPos.second] = newPlant;
    };
};

void mainPage::popup_setting(){
    planting_window = new QWidget(this);
    planting_window->setWindowFlags(Qt::FramelessWindowHint);
    planting_window->hide();
    upgrading_window = new QWidget(this);
    upgrading_window->setWindowFlags(Qt::FramelessWindowHint);
    upgrading_window->hide();

    std::function<void(int)> setPlantButton = [&](int k){
        QPushButton* newBar = new QPushButton(planting_window);
        newBar->setGeometry(QRect(50*k,0,150,50));
        newBar->setText("BottleTower");
        newBar->setFont(QFont("STHupo",15));
        QObject::connect(newBar, &QPushButton::clicked, this, plantAct(this,k));
    };
    std::function<void()> setShovel = [&](){
        QPushButton* newBar = new QPushButton(upgrading_window);
        newBar->setGeometry(QRect(0,0,150,50));
        newBar->setText("Shovel");
        newBar->setFont(QFont("STHupo",15));
        QObject::connect(newBar, &QPushButton::clicked, this, [&](){
            upgrading_window->hide();
            scene.removeItem(isPlant[focusPos.first][focusPos.second]);
            allPlants.removeOne(isPlant[focusPos.first][focusPos.second]);
            delete isPlant[focusPos.first][focusPos.second];
            isPlant[focusPos.first][focusPos.second] = nullptr;
        });
    };
    setPlantButton(0);
    setShovel();
//    for(int k=0;k<3;k++)
//        setupgrade(k);
}

template<typename T1>
QPushButton* mainPage::setButton(QString url, T1 slot, QRect pos){
    if(pos==QRect(-1,-1,-1,-1))
        pos = QRect(left,labelpadding,width,buttonsize);
    QPushButton* newBar = new QPushButton(this);
    newBar->setGeometry(pos);
    newBar->setIconSize(QSize(newBar->size()));
    newBar->setIcon(QPixmap(url).scaled(QSize(newBar->size())));
    QObject::connect(newBar, &QPushButton::clicked, this, slot);
    newBar->show();
    return newBar;
}

QLabel* mainPage::setLabel(QString url,QRect pos){
    if(pos==QRect(-1,-1,-1,-1))
        pos = QRect(left,labelpadding,width,buttonsize);
    QLabel* newLabel = new QLabel(this);
    newLabel->setGeometry(pos);
    newLabel->setStyleSheet(QString("border-image:url(%1)").arg(url));
    newLabel->show();
//    allLabel.append(newLabel);
    return newLabel;
}

void mainPage::closeAct() {
    rootPage->show();
    this->close();
};

void mainPage::speedAct(){
    if(!timer->isActive())  return;
    switch(speed){
    case 1:speed=2;break;
    case 2:speed=4;break;
    case 4:speed=1;break;
    }
    speedBar->setIcon(QIcon(QPixmap(QString(":/img/src/img/speed%1.png").arg(speed)).scaled(QSize(2*buttonsize,buttonsize))));
    timer->start(1000/fps/speed);
    waveGener->start(1000/speed);
}

void mainPage::stopAct(){
    if(timer->isActive()){
        timer->stop();
        waveGener->stop();
    }
    else{
        timer->start();
        waveGener->stop();
    }
}

void mainPage::cureAct(int t){
    totalGPA+=t;
    if(totalGPA<0){
        emit _gameFail();
        return;
    }
    theLionPic->setStyleSheet(QString("border-image:url(:/plant/src/plant/%1.png)").arg(totalGPA));
}

void mainPage::bombAct(){

}

void mainPage::iceAct(){

}

void mainPage::cheerAct(){

}

mainPage::~mainPage(){
    rootPage->gameOut = true;
    for(auto p: allPlants)
        delete p;
    for(auto p: allMons)
        if(monster::memoryExisted[p])
            delete p;
}

void mainPage::mousePressEvent(QMouseEvent* e){
    int r = floor((1.0*e->y()-padding_top)/grid_l), c = floor((1.0*e->x()-padding_left)/grid_l);
    planting_window->hide();
    upgrading_window->hide();
    focusPic->hide();
    rangePic->hide();
    focusPos = {r,c};
    if(r<0||c<0||r>=grid_h_num||c>=grid_w_num||notEmpty[r][c])
        return QWidget::mousePressEvent(e);
    focusPic->move(padding_left+grid_l*c,padding_top+grid_l*r);
    focusPic->show();
    if(isPlant[r][c]!=nullptr){
        upgrading_window->move(padding_left+grid_l*(c+1),padding_top+grid_l*r);
        upgrading_window->show();
        double rad = isPlant[r][c]->data(Tower::DATA_RANGE).toDouble()*2;
        rangePic->setGeometry(QRect(padding_left+grid_l*(c+0.5)-rad*0.5,padding_top+grid_l*(r+0.5)-rad*0.5,rad,rad));
        rangePic->setStyleSheet(QString("min-width:%1px;min-height:%2px;max-width:%3px;max-height:%4px;border-radius:%5px;background-color:grey").arg(rad).arg(rad).arg(rad).arg(rad).arg(rad/2));
        rangePic->show();
    }
    else{
        planting_window->move(padding_left+grid_l*(c+1),padding_top+grid_l*r);
        planting_window->show();
    }
    return QWidget::mousePressEvent(e);
}

void mainPage::oneWave(){
    curWave++;
    if(curWave>totalWave){
        emit _gameWin();
        return;
    }
    newgenermons = 0;
    progressBar->setText(QString("%1 / %2").arg(curWave).arg(totalWave));
    waveGener->start(1000/speed);
}

void mainPage::gameFail(){
    timer->stop();
    waveGener->stop();
    closeAct();
}

void mainPage::gameWin(){
    timer->stop();
    waveGener->stop();
    closeAct();
}
