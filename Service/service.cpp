// === Service/service.cpp ===
#include "service.h"
#include "jsonHandlers.cpp"
#include <QDebug>
#include <QFile>

Service::Service(QObject* parent)
    : QObject(parent)
{
    // Load initial configuration
    QFile file("com.system.configurationManager/confManagerApplication1.json");
    if (file.open(QIODevice::ReadOnly)) {
        QString jsonText = file.readAll();
        conf = unMarshal(jsonText);
    } else {
        qWarning() << "Cannot open initial config file";
    }
}

Service::~Service() {}

QMap<QString, QVariant> Service::GetConfiguration() {
    return conf;
}

void Service::ChangeConfiguration(const QString& key, const QVariant& value) {
    if (!conf.contains(key)) {
        qWarning() << "Configuration key not found:" << key;
        return;
    }
    conf.insert(key, value);
    // Save back to JSON
    QFile file("com.system.configurationManager/confManagerApplication1.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(Marshal(conf).toUtf8());
    } else {
        qWarning() << "Cannot write config file";
    }
    emit configurationChanged(conf);
}
