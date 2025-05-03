// === Service/serviceMain.cpp ===

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusVariant>
#include <QDBusMetaType>
#include <QDir>
#include "service.h"

static const char *SERVICE_NAME = "com.system.configurationManager";
static const char *BASE_PATH    = "/com/system/configurationManager/Application";

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    qDBusRegisterMetaType<QDBusVariant>();
    qDBusRegisterMetaType<QVariantMap>();

    // Регистрация сервиса на шине
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(SERVICE_NAME)) {
        qFatal("Failed to register D-Bus service");
    }

    // Path to the configuration directory in the user's home
    const QString configDir = QDir::homePath() + "/com.system.configurationManager";
    QDir dir(configDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qFatal("Cannot create config directory");
        }
    }

    // Создаем для каждого JSON-файла объект
    QStringList jsonFiles = dir.entryList(QStringList() << "*.json", QDir::Files);
    for (const QString &fileName : jsonFiles) {
        QString appName  = fileName.section('.', 0, 0);
        QString fullPath = dir.filePath(fileName);

        Service *svc = new Service(appName, fullPath, &a);

        QString objectPath = QString(BASE_PATH) + "/" + appName;
        bool ok = conn.registerObject(
            objectPath,
            svc,
            QDBusConnection::ExportAdaptors | QDBusConnection::ExportScriptableSlots
        );
        if (!ok) {
            qWarning() << "Cannot register object for" << appName
                       << "at path" << objectPath;
            delete svc;
        } else {
            qDebug() << "Registered" << appName << "at" << objectPath;
        }
    }

    qDebug() << "ConfigurationManager multi-instance service started";
    return a.exec();
}
