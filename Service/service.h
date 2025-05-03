// === Service/service.h ===
#pragma once
#include <QObject>
#include <QMap>
#include <QVariant>

// === Service/serviceAdaptor.h ===
#pragma once
#include <QDBusAbstractAdaptor>
#include <QMap>
#include <QVariant>
#include <QDBusVariant>

class Service;

class ServiceAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.system.configurationManager.Application.Configuration")
public:
    explicit ServiceAdaptor(Service* parent);


public slots:
    QVariantMap GetConfiguration();
    Q_SCRIPTABLE void ChangeConfiguration(const QString& key, const QDBusVariant& value);

signals:
    void configurationChanged(const QVariantMap& configuration);

private:
    Service* m_service;
};



class Service : public QObject {
    Q_OBJECT
public:
    explicit Service(const QString& appName,
                         const QString& configFilePath,
                         QObject* parent = nullptr);
    ~Service();

    QVariantMap GetConfiguration();

public slots:
    void ChangeConfiguration(const QString& key, const QVariant& value);

signals:
    void configurationChanged(const QVariantMap& configuration);

private:
    QString     m_appName;
    QVariantMap conf;
    QString m_configPath;   // путь к конфиг файлу приложения
    ServiceAdaptor* m_adaptor;     // указатель на адаптер
};


