
#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QMainWindow>
#include <QPushButton>
#include <QThread>
#include <QMovie>

class mainPage;
class teachingPage;
class settingPage;

class startPage : public QMainWindow

{
    Q_OBJECT

public:
    static bool gameOut;
    QPushButton* newGameBar;
    QPushButton* teachingBar;
    QPushButton* settingBar;

    startPage(QWidget *parent = nullptr);
    ~startPage();
    template<typename T1>
    QPushButton* setButton(QRect pos, QString url, T1 slot);
    void on_newGame_clicked();
    void on_teaching_clicked();
    void on_setting_clicked();
};

#endif // STARTPAGE_H
