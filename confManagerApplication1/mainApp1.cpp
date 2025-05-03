// === confManagerApplication1/mainApp1.cpp ===

#include <QCoreApplication>
#include "client.h"
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QDebug>


int main(int argc, char* argv[]) {

    QCoreApplication a(argc, argv);
    qDBusRegisterMetaType<QVariantMap>();
    qDBusRegisterMetaType<QDBusVariant>();
    Client client;
    return a.exec();

}

