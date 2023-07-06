
#include "startpage.h"
#include "mainpage.h"
#include "teachingpage.h"
#include "settingpage.h"
#include <math.h>

bool startPage::gameOut = true;

startPage::startPage(QWidget *parent)
    : QMainWindow(parent)
{
    //qDebug()<<atan2(1,1)/atan2(0,-1)*180<<atan2(-1,1)/atan2(0,-1)*180<<atan2(-1,-1)/atan2(0,-1)*180<<atan2(1,-1)/atan2(0,-1)*180;
    mainPage::rootPage = this;
    this->setFixedSize(1100, 610);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    this->setObjectName("stpage");
    this->setStyleSheet("startPage#stpage{border-image:url(:/img/src/img/startpage.png);}");

    int col=450,row=350,height=50;
    newGameBar = setButton(QRect(col,row,1100-2*col,height),"NEW GAME",&startPage::on_newGame_clicked);
    teachingBar = setButton(QRect(col,row+2*height,1100-2*col,height),"TUTORIAL",&startPage::on_newGame_clicked);
    settingBar = setButton(QRect(col,row+4*height,1100-2*col,height),"SETTING",&startPage::on_newGame_clicked);
}

startPage::~startPage()
{

}

template<typename T1>
QPushButton* startPage::setButton(QRect pos, QString url, T1 slot){
    QPushButton* newBar = new QPushButton(this);
    newBar->setGeometry(pos);
    newBar->setText(url);
    newBar->setFont(QFont("STHupo",20));
    QObject::connect(newBar, &QPushButton::clicked, this, slot);
    newBar->show();
    return newBar;
}

void startPage::on_newGame_clicked(){
    mainPage* newGame = new mainPage(nullptr);
    gameOut = false;
    newGame->show();
    this->hide();
    QLabel* shade = new QLabel(newGame);
    QLabel* animate = new QLabel(newGame);
    QMovie* m = new QMovie(":/gif/src/gif/start.gif");
    shade->setGeometry(QRect(0,0,newGame->window_width,newGame->window_height));
    shade->setStyleSheet(
        "background:rgba(0,0,0,0.5);");
    shade->show();
    animate->setGeometry(QRect((newGame->window_width-300)/2,(newGame->window_height-200)/2,300,200));
    animate->setMovie(m);
    animate->show();
    m->start();
    QEventLoop loop;
    QTimer::singleShot(3500, &loop, SLOT(quit()));
    loop.exec();
    delete m;
    delete shade;
    delete animate;
    newGame->oneWave();
    QTimer::singleShot(3500, &loop, SLOT(quit()));
    loop.exec();
}

void startPage::on_teaching_clicked(){
    teachingPage* newGame = new teachingPage(this,nullptr);
    newGame->show();
    this->hide();
}

void startPage::on_setting_clicked(){
    settingPage* newGame = new settingPage(this,nullptr);
    newGame->show();
    this->hide();
}
