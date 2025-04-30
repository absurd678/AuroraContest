QT       += core dbus
CONFIG   += c++11 console
TEMPLATE = app
TARGET   = configurationManagerService

# Пути к исходникам и заголовкам
SOURCES += \
    serviceMain.cpp \
    service.cpp \
    jsonHandlers.cpp \
    serviceAdaptor.cpp

HEADERS += \
    service.h \
    serviceAdaptor.h

# Указываем, что экспортируем адаптеры и слоты для D-Bus
QMAKE_CXXFLAGS += -DQT_DBUS_LIB
#RESOURCES += config.qrc
