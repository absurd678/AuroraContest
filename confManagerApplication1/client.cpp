// === confManagerApplication1/client.cpp ===
#include "client.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>
#include <QDBusServiceWatcher>
#include <QDebug>

Client::Client(QObject* parent)
    : QObject(parent), m_interface(nullptr)
{
    // Watch for service registration/unregistration
    QDBusServiceWatcher* watcher = new QDBusServiceWatcher(m_serviceName,
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration,
        this);
    connect(watcher, &QDBusServiceWatcher::serviceRegistered,
            this, &Client::connectToService);
    connect(watcher, &QDBusServiceWatcher::serviceUnregistered,
            this, &Client::disconnectFromService);

    // If already running
    QStringList services = QDBusConnection::sessionBus().interface()->registeredServiceNames();
    if (services.contains(m_serviceName)) {
        connectToService(m_serviceName);
    }
}

void Client::connectToService(const QString& serviceName) {
    if (serviceName != m_serviceName)
        return;
    qDebug() << "Client: connecting to service";
    m_interface = new QDBusInterface(m_serviceName, m_objectPath,
                                     m_interfaceName,
                                     QDBusConnection::sessionBus(), this);
    // Subscribe to configurationChanged signal
    QDBusConnection::sessionBus().connect(
        m_serviceName, m_objectPath, m_interfaceName,
        "configurationChanged", this, SLOT(appFunction(QVariantMap)));
    
    // Fetch initial configuration
    QDBusReply<QVariantMap> reply = m_interface->call("GetConfiguration");
    if (reply.isValid()) {
        appFunction(reply.value());
    } else {
        qWarning() << "Failed to call GetConfiguration:" << reply.error().message();
    }
}

void Client::disconnectFromService(const QString& serviceName) {
    if (serviceName != m_serviceName)
        return;
    qDebug() << "Client: service disconnected";
    delete m_interface;
    m_interface = nullptr;
}

void Client::appFunction(const QVariantMap& configuration) {
    if (configuration.contains("TimeoutPhrase")) {
        qDebug() << "TimeoutPhrase:" << configuration.value("TimeoutPhrase").toString();
    }
}
