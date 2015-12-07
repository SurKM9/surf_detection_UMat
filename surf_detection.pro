#-------------------------------------------------
#
# Project created by QtCreator 2015-11-16T22:28:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = surf_detection
TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp\
        surf_detection.cpp

HEADERS  += surf_detection.h

FORMS    += surf_detection.ui

LIBS += -L/home/surya/libraries/opencv-3.0.0/CV/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect -lopencv_ml -lopencv_videostab -lopencv_calib3d -lopencv_videoio -lopencv_xfeatures2d -lopencv_features2d -lopencv_flann
#LIBS += -L/home/surya/libraries/opencv-2.4.9/CV/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect -lopencv_ml -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_nonfree

LIBS += \
        -lboost_system\
        -lboost_thread\

INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/include/boost
