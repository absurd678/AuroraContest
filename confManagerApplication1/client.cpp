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
    // Настройка клиента как watcher'а
    QDBusServiceWatcher* watcher = new QDBusServiceWatcher(m_serviceName,
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForRegistration | QDBusServiceWatcher::WatchForUnregistration,
        this);
    connect(watcher, &QDBusServiceWatcher::serviceRegistered,
            this, &Client::connectToService);
    connect(watcher, &QDBusServiceWatcher::serviceUnregistered,
            this, &Client::disconnectFromService);

    QStringList services = QDBusConnection::sessionBus().interface()->registeredServiceNames();
    if (services.contains(m_serviceName)) {
        connectToService(m_serviceName);
    }

    // Настроить таймер
    connect(&m_timer, &QTimer::timeout, this, &Client::showPhrase);
}

void Client::connectToService(const QString& serviceName) {
    if (serviceName != m_serviceName)
        return;
    qDebug() << "Client: connecting to service";
    m_interface = new QDBusInterface(m_serviceName, m_objectPath,
                                     m_interfaceName,
                                     QDBusConnection::sessionBus(), this);

    // Подписка на сигнал
    QDBusConnection::sessionBus().connect(
        m_serviceName,
        m_objectPath,
        m_interfaceName,
        "configurationChanged",
        this,
        SLOT(appFunction(QVariantMap))
    );

    // Вывод текущей конфигурации
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
    qDebug() << "m_timeoutPhrase";
    if (configuration.contains("TimeoutPhrase")) {
        int newTimeout = configuration.value("Timeout").toInt();
        if (newTimeout > 0 && newTimeout != m_timeout) {
            m_timeout = newTimeout;
            m_timer.setInterval(m_timeout);
            m_timer.start();    // запустить таймер
        }
    }

    // обновляем фразу
    if (configuration.contains("TimeoutPhrase")) {
        m_timeoutPhrase = configuration.value("TimeoutPhrase").toString();
        qDebug() << m_timeoutPhrase;
    }
}

void Client::showPhrase()
{
    if (!m_timeoutPhrase.isEmpty()) {
        qDebug() << m_timeoutPhrase;
    }
}
