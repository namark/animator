#-------------------------------------------------
#
# Project created by QtCreator 2011-12-31T04:09:35
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

QT       += core gui widgets multimedia

TARGET = animator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colordialog.cpp \
    saveasdialog.cpp \
    opendialog.cpp \
    sizedialog.cpp \
    attachmusicdialog.cpp \
    morecolordialog.cpp \
    loadingdialog.cpp

HEADERS  += mainwindow.h \
    dot_list.h \
    colordialog.h \
    saveasdialog.h \
    opendialog.h \
    sizedialog.h \
    attachmusicdialog.h \
    morecolordialog.h \
    loadingdialog.h

FORMS    += mainwindow.ui \
    colordialog.ui \
    saveasdialog.ui \
    opendialog.ui \
    sizedialog.ui \
    attachmusicdialog.ui \
    morecolordialog.ui \
    loadingdialog.ui

RESOURCES += \
    rec.qrc

 LIBS += -lhuffmancoder
