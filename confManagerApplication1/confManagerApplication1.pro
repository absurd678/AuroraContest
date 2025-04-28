QT       += core dbus
CONFIG   += c++11 console
TEMPLATE = app
TARGET   = confManagerApplication1

SOURCES += \
    mainApp1.cpp \
    client.cpp

HEADERS += \
    client.h

# Если нужно, можно добавить ресурс с JSON:
# RESOURCES += confManagerApplication1.qrc
