QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bottle.cpp \
    bottle_tower.cpp \
    main.cpp \
    mainpage.cpp \
    monster.cpp \
    settingpage.cpp \
    startpage.cpp \
    teachingpage.cpp \
    tower.cpp

HEADERS += \
    bottle.h \
    bottle_tower.h \
    mainpage.h \
    monster.h \
    settingpage.h \
    startpage.h \
    teachingpage.h \
    tower.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
