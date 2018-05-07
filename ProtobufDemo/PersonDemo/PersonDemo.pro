TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../source/Person.pb.cc \
    ../source/AddressBook.pb.cc

HEADERS += \
    ../source/Person.pb.h \
    ../source/AddressBook.pb.h

INCLUDEPATH += ../source/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/local/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include
