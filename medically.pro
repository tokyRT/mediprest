#-------------------------------------------------
#
# Project created by QtCreator 2020-01-06T14:44:02
#
#-------------------------------------------------

QT       += core gui printsupport charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = medically
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        addmeddialog.cpp \
        addpatdialog.cpp \
        addtraitdialog.cpp \
        editcriteredialog.cpp \
        main.cpp \
        mainwindow.cpp \
        medecin.cpp \
        navbtn.cpp \
        patient.cpp \
        prestdialog.cpp \
        traitement.cpp \
        updatemeddialog.cpp \
        updatepatdialog.cpp \
        voirpatdumed.cpp \
        voirtrait.cpp

HEADERS += \
        addmeddialog.h \
        addpatdialog.h \
        addtraitdialog.h \
        editcriteredialog.h \
        mainwindow.h \
        medecin.h \
        navbtn.h \
        patient.h \
        prestdialog.h \
        traitement.h \
        updatemeddialog.h \
        updatepatdialog.h \
        voirpatdumed.h \
        voirtrait.h

FORMS += \
        addmeddialog.ui \
        addpatdialog.ui \
        addtraitdialog.ui \
        editcriteredialog.ui \
        mainwindow.ui \
        prestdialog.ui \
        updatemeddialog.ui \
        updatepatdialog.ui \
        voirpatdumed.ui \
        voirtrait.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
