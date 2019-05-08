#-------------------------------------------------
#
# Project created by QtCreator 2018-07-23T13:12:13
#
#-------------------------------------------------

QT       += core gui sql charts
RC_ICONS = nu.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Goat
TEMPLATE = app

SOURCES += main.cpp\
    adddatadialog.cpp \
    housebindingdialog.cpp \
        mainwindow.cpp \
    mainform.cpp \
    sportdataform.cpp \
    goatinfoform.cpp \
    goatqueryform.cpp \
    devicequeryform.cpp \
    db.cpp \
    changegoatinfodialog.cpp \
    bindingdialog.cpp \
    logindialog.cpp \
    informationform.cpp \
    addgoatdialog.cpp \
    helpdialog.cpp \
    otherinfoform.cpp \
    otherdataform.cpp

HEADERS  += mainwindow.h \
    adddatadialog.h \
    housebindingdialog.h \
    mainform.h \
    sportdataform.h \
    goatinfoform.h \
    goatqueryform.h \
    devicequeryform.h \
    db.h \
    changegoatinfodialog.h \
    bindingdialog.h \
    logindialog.h \
    informationform.h \
    addgoatdialog.h \
    helpdialog.h \
    otherinfoform.h \
    otherdataform.h

FORMS    += mainwindow.ui \
    adddatadialog.ui \
    form.ui \
    housebindingdialog.ui \
    mainform.ui \
    sportdataform.ui \
    goatinfoform.ui \
    goatqueryform.ui \
    devicequeryform.ui \
    changegoatinfodialog.ui \
    bindingdialog.ui \
    logindialog.ui \
    informationform.ui \
    addgoatdialog.ui \
    helpdialog.ui \
    otherinfoform.ui \
    otherdataform.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    minus.png \
    plus.png
