// === Service/serviceMain.cpp ===
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusVariant>
#include <QDBusMetaType>
#include "service.h"
//#include "serviceAdaptor.h"

static const char* OBJECT_PATH = "/com/system/configurationManager/Application/confManagerApplication1";

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qDBusRegisterMetaType<QDBusVariant>();
    qDBusRegisterMetaType<QVariantMap>();

    Service s1;
   // ServiceAdaptor adaptor(&s1);
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService("com.system.configurationManager")) {
        qFatal("Failed to register D-Bus service");
    }
    if (!conn.registerObject(OBJECT_PATH, &s1,
        QDBusConnection::ExportAdaptors | QDBusConnection::ExportScriptableSlots)) {
        qFatal("Failed to register object on D-Bus");
    }
    qDebug() << "ConfigurationManager service started";

    return a.exec();
}


