// === Service/service.cpp ===
#include "service.h"
#include "jsonHandlers.cpp"
#include <QDebug>
#include <QFile>
#include <QCoreApplication>

Service::Service(QObject* parent)
    : QObject(parent)
{
    m_configPath = QCoreApplication::applicationDirPath() + "/confManagerApplication1.json";
    QFile file(m_configPath);
    if (file.open(QIODevice::ReadOnly)) {
        QString jsonText = file.readAll();
        conf = unMarshal(jsonText);
    } else {
        qWarning() << "Cannot open initial config file";
    }
}

Service::~Service() {}

QVariantMap Service::GetConfiguration() {
    return conf;
}

void Service::ChangeConfiguration(const QString& key, const QVariant& value) {
    if (!conf.contains(key)) {
        qWarning() << "Configuration key not found:" << key;
        return;
    }
    conf.insert(key, value);
    // Сохранить в JSON
    QFile file(m_configPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(Marshal(conf).toUtf8());
    } else {
        qWarning() << "Cannot write config file";
    }
    // Сигнал выплывает
    emit configurationChanged(conf);
}
