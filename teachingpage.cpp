
#include "teachingpage.h"

teachingPage::teachingPage(startPage *root, QWidget* parent)
    : QWidget(parent),rootPage(root)
{
    this->setFixedSize(1100, 610);
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    QLabel* settingPic = new QLabel(this);
    settingPic->setGeometry(QRect(990, 10, 30, 30));
    settingPic->setStyleSheet("border-image:url(:/img/src/img/menupic.png);");
    QPushButton* settingBar = new QPushButton(this);
    settingBar->setGeometry(QRect(990, 10, 30, 30));
    settingBar->setFlat(true);
    connect(settingBar, &QPushButton::clicked, [&]() {
        rootPage->show();
        this->close();
    });

    QLabel text(this);
    text.setGeometry(100,100,200,200);
    text.setText("Teaching Page Not Available!");

}
