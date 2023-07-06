
#ifndef TEACHINGPAGE_H
#define TEACHINGPAGE_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "startpage.h"

class teachingPage : public QWidget
{
    Q_OBJECT
private:
    startPage* rootPage;
public:
    teachingPage(startPage *root, QWidget* parent);
signals:

};

#endif // TEACHINGPAGE_H
