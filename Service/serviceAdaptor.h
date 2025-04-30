// === Service/serviceAdaptor.h ===
#pragma once
#include <QDBusAbstractAdaptor>
#include <QMap>
#include <QVariant>
#include <QDBusVariant>
#include "service.h"

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
