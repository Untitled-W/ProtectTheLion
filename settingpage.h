
#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "startpage.h"
#include "utils.h"

class settingPage : public QWidget
{
    Q_OBJECT

private:
    startPage* rootPage;
    QPushButton* settingBar;
    QLabel text;
public:
    settingPage(startPage *root, QWidget* parent);
    ~settingPage();
    void myClose();

    template<typename T1>
    QPushButton* setButton(QRect pos, QString url, T1 slot);

signals:

};

#endif // SETTINGPAGE_H
