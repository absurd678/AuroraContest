// === Service/serviceAdaptor.h ===
#pragma once
#include <QDBusAbstractAdaptor>
#include <QMap>
#include <QVariant>
#include "service.h"

class ServiceAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.system.configurationManager.Application.Configuration")
public:
    explicit ServiceAdaptor(Service* parent);

public slots:
    QMap<QString, QVariant> GetConfiguration();
    void ChangeConfiguration(const QString& key, const QVariant& value);

signals:
    void configurationChanged(const QMap<QString, QVariant>& configuration);

private:
    Service* m_service;
};
