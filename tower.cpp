
#include "tower.h"


Tower::Tower(int _type, QPointF _location, mainPage* _root)
    : type(_type),root(_root)
{
    x = _location.x()/50;
    y = _location.y()/50;

}

Tower::~Tower(){}

