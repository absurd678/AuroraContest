// === Service/serviceMain.cpp ===
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include "service.h"
#include "serviceAdaptor.h"

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    Service s1;
    ServiceAdaptor adaptor(&s1);

    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService("com.system.configurationManager")) {
        qFatal("Failed to register D-Bus service");
    }
    if (!conn.registerObject("/", &s1,
        QDBusConnection::ExportAdaptors | QDBusConnection::ExportScriptableSlots)) {
        qFatal("Failed to register object on D-Bus");
    }
    qDebug() << "ConfigurationManager service started";

    return a.exec();
}
