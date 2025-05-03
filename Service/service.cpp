// === Service/service.cpp ===
#include "service.h"
#include "jsonHandlers.cpp"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCoreApplication>

// === Service/serviceAdaptor.cpp ===
//#include "serviceAdaptor.h"

ServiceAdaptor::ServiceAdaptor(Service* parent)
    : QDBusAbstractAdaptor(parent), m_service(parent)
{
}

QVariantMap ServiceAdaptor::GetConfiguration() {
    return m_service->GetConfiguration();
}

void ServiceAdaptor::ChangeConfiguration(const QString& key, const QDBusVariant& value) {
    m_service->ChangeConfiguration(key, value.variant());
}



Service::Service(const QString& appName,
                 const QString& configFilePath,
                 QObject* parent)
    : QObject(parent)
    , m_appName(appName)
    , m_configPath(configFilePath)
    , m_adaptor(new ServiceAdaptor(this))
{
    // Проксируем сигнал к адаптеру
    connect(this, SIGNAL(configurationChanged(QVariantMap)),
            m_adaptor, SIGNAL(configurationChanged(QVariantMap)));

    // Читаем (или создаём) JSON
    QFile file(m_configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open config for" << m_appName << "at" << m_configPath;
        // При желании можно создать из шаблона ресурса…
    } else {
        conf = unMarshal(QString::fromUtf8(file.readAll()));
    }
}

Service::~Service() {
    delete m_adaptor;
}

QVariantMap Service::GetConfiguration() {
    return conf;
}

void Service::ChangeConfiguration(const QString& key, const QVariant& value) {
    if (!conf.contains(key)) {
         qWarning() << "[" << m_appName << "] Key not found:" << key;
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
