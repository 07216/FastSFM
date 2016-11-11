#-------------------------------------------------
#
# Project created by QtCreator 2015-12-12T19:31:59
#
#-------------------------------------------------

QT       += core gui  multimediawidgets

#LIBS += -LD:\stfile\Documents\GitHub\FastSFM\libs -lpython27


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USFM
TEMPLATE = app

DEFINES += _USE_MATH_DEFINES

SOURCES += main.cpp\
        mainwindow.cpp\
        arcball.cpp\
    settingdialog.cpp \
    readdialog.cpp \
    videodialog.cpp


HEADERS  += mainwindow.h\
        arcball.h\
    settingdialog.h \
    readdialog.h \
    videodialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    readdialog.ui \
    videoplayer.ui \
    videodialog.ui

INCLUDEPATH +=  D:\lib\opencv\build\include\    libs
LIBS+=D:\lib\opencv\build\x64\vc10\lib\opencv_core2411d

# openmp
QMAKE_CXXFLAGS *= /openmp
