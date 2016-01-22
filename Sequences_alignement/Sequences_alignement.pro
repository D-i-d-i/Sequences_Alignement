QT += core

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

SOURCES += main.cpp \
    aligner.cpp \
    helper.cpp

HEADERS += \
    aligner.h \
    helper.h

