// === confManagerApplication1/client.h ===
#pragma once
#include <QObject>
#include <QDBusInterface>
#include <QVariantMap>
#include <QTimer>

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);

public slots:
    void connectToService(const QString& serviceName);
    void disconnectFromService(const QString& serviceName);
    void appFunction(const QVariantMap& configuration);
    void showPhrase();                    // Вывод TimeoutPhrase раз в Timeout мс

private:
    QDBusInterface* m_interface;
    const QString m_serviceName = "com.system.configurationManager";
    const QString m_objectPath = "/com/system/configurationManager/Application/confManagerApplication1";
    const QString m_interfaceName = "com.system.configurationManager.Application.Configuration";

    // для вывода
    QTimer    m_timer;
    int       m_timeout      = 0;        // Timeout в миллисекундах
    QString   m_timeoutPhrase;           // TimeoutPhrase
};
