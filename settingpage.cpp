
#include "settingpage.h"

settingPage::settingPage(startPage *root, QWidget* parent)
    : QWidget(parent),rootPage(root),text(this)
{
    this->setFixedSize(1100, 610);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    settingBar = setButton(QRect(990,10,30,30),":/img/src/img/menupic.png",&settingPage::myClose);

    text.show();
    text.setGeometry(100,100,200,200);
    text.setText("setting Page Not Available!");
}

settingPage::~settingPage(){
    delete settingBar;
}

void settingPage::myClose() {
        rootPage->show();
        this->close();
};

template<typename T1>
QPushButton* settingPage::setButton(QRect pos, QString url, T1 slot){
        QPushButton* newBar = new QPushButton(this);
        newBar->setGeometry(pos);
        newBar->setIcon(QIcon(url));
        QObject::connect(newBar, &QPushButton::clicked, this, slot);
        newBar->show();
        return newBar;
}
