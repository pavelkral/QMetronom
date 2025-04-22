
QT += core gui network xml multimedia

#android: QT += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TEMPLATE = app
RESOURCES += qmetronom.qrc
TARGET = qmetronom
DESTDIR = $$OUT_PWD/bin

SOURCES += main.cpp \
    widget.cpp \ 
#  androidfiledialog.cpp \
    dialog.cpp

HEADERS += widget.h \
#   androidfiledialog.h \
    dialog.h

FORMS += widget.ui \
    dialog.ui

RC_FILE = qmetronom.rc




