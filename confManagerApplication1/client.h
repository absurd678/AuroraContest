// === confManagerApplication1/client.h ===
#pragma once
#include <QObject>
#include <QDBusInterface>
#include <QVariantMap>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);

public slots:
    void connectToService(const QString& serviceName);
    void disconnectFromService(const QString& serviceName);
    void appFunction(const QVariantMap& configuration);

private:
    QDBusInterface* m_interface;
    const QString m_serviceName = "com.system.configurationManager";
    const QString m_objectPath = "/";
    const QString m_interfaceName = "com.system.configurationManager.Application.Configuration";
};
