
#ifndef MAINPAGE_H
#define MAINPAGE_H


#include <QWidget>
#include <QLabel>
#include <QDialog>
#include <QMouseEvent>
#include <QPushButton>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>


#include "startpage.h"
#include "bottle_tower.h"
#include "monster.h"

class mainPage : public QWidget
{
    Q_OBJECT

public:

    //场景元素
    QLabel* GPABar;
    QLabel* moneyBar;
    QLabel* focusPic;
    QLabel* rangePic;
    QLabel* theLionPic;
    QLabel* progressBar;
    QPushButton* backBar;
    QPushButton* speedBar;
    QPushButton* stopBar;
    QLabel* backgroundpic;

    QPushButton* iceBar;
    QPushButton* boomBar;
    QPushButton* cureBar;
    QPushButton* cheerBar;

    QTimer* timer;
    QTimer* waveGener;
    QGraphicsView* view;
    QGraphicsScene scene;
    QThread workerThread;
    QWidget* planting_window;    //种植菜单
    QWidget* upgrading_window;  //升级菜单

    QList<Tower*> allPlants;
    QList<monster*> allMons;

    int left,width;
    const int fps = 23;
    const int labeltop = 10;
    const int skillsize = 60;
    const int buttonsize = 50;
    const int labelstart = 100;
    const int labelpadding = 20;
    static startPage* rootPage;
    const int padding_left = 150;
    static const int grid_l = 70;
    static const int grid_h_num = 10;
    static const int grid_w_num = 16;
    const int padding_bottom = skillsize+20;
    const int padding_top = labeltop*2+buttonsize;
    const int window_width = padding_left*2+grid_w_num*grid_l;
    const int window_height = padding_top+padding_bottom+grid_h_num*grid_l;


    int speed = 1;
    int money = 1000;
    int totalGPA = 9;
    int newgenermons = 0;
    std::pair<int,int> focusPos;
    std::pair<int,int> startpos;
    std::pair<int,int> endpos;


    int curWave = 0;
    int totalWave = 3;
    int monsWave[100]={
        10,10,10
    };
    int monsHP[100][100]={
        {100,100,100,100,100,100,100},
        {100,100,100,100,100,100,100},
        {100,100,100,100,100,100,100}
    };
    int monsType[100][100]={
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1},
    };
    int monsSpeed[100][100]={
        {1,1,1,2,2,2,3,3},
        {1,1,1,2,2,2,3,3},
        {1,1,1,2,2,2,3,3},
    };
    int monsScale[100][100]={
        {2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2}
    };


    bool notEmpty[grid_h_num][grid_w_num]{};
    Tower* isPlant[grid_h_num][grid_w_num]{};
    std::pair<int,int> stepdir[grid_h_num][grid_w_num]{};   //方向向量

    mainPage(QWidget* parent);
    ~mainPage();

    //场景布置函数
    void graphics_setting();
    void ui_setting();
    void popup_setting();
    void generate_road();
    void generate_buildings();

    void mousePressEvent(QMouseEvent* e);
    QLabel* setLabel(QString url,QRect pos=QRect(-1,-1,-1,-1));
    template<typename T1>
    QPushButton* setButton(QString url, T1 slot, QRect pos=QRect(-1,-1,-1,-1));
    std::pair<int,int> add(std::pair<int,int> x, std::pair<int,int> y){
        return {x.first+y.first, x.second+y.second};
    }


signals:
    void _gameFail();
    void _gameWin();

    void frozenAll();
    void killAll();
    void cheerAll();
    void pauseGame();
    void continueGame();
    void speedChange(int muls);

public slots:
    void oneWave();
    void gameFail();
    void gameWin();

    void closeAct();
    void speedAct();
    void stopAct();

    void cureAct(int);
    void bombAct();
    void iceAct();
    void cheerAct();

};

#endif // MAINPAGE_H
