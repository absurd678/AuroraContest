// === Service/serviceMain.cpp ===
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QDBusVariant>
#include <QDBusMetaType>
#include "service.h"
#include <QDir>
//#include "serviceAdaptor.h"

static const char* SERVICE_NAME = "com.system.configurationManager";
static const char* BASE_PATH    = "/com/system/configurationManager/Application";


int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qDBusRegisterMetaType<QDBusVariant>();
    qDBusRegisterMetaType<QVariantMap>();

    // 1) Объявляем сервис на шине
       QDBusConnection conn = QDBusConnection::sessionBus();
       if (!conn.registerService(SERVICE_NAME)) {
           qFatal("Failed to register D-Bus service");
       }

       // 2) Путь к папке с JSON в домашней директории
       const QString configDir = QDir::homePath()
                                 + "/com.system.configurationManager";
       QDir dir(configDir);
       if (!dir.exists()) {
           if (!dir.mkpath(".")) {
               qFatal("Cannot create config directory");
           }
       }

       // 3) Для каждого JSON-файла создаём Service и регистрируем объект
       QStringList jsonFiles = dir.entryList(QStringList() << "*.json",
                                             QDir::Files);
       for (const QString& fileName : jsonFiles) {
           QString appName = fileName.section('.', 0, 0);
           QString fullPath = dir.filePath(fileName);

           // 3.1 создаём инстанс
           Service* svc = new Service(appName, fullPath, &a);

           // 3.2 вычисляем уникальный object path
           QString objectPath = QString(BASE_PATH) + "/" + appName;

           // 3.3 регистрируем его
           bool ok = conn.registerObject(
               objectPath,
               svc,
               QDBusConnection::ExportAdaptors
             | QDBusConnection::ExportScriptableSlots
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


