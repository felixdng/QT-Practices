TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    protob.cpp \
    sdcp_common.cpp \
    sdcp_server.cpp

HEADERS += \
    protob.h \
    sdcp_common.h \
    sdcp_server.h
