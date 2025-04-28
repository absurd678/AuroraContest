// === Service/serviceAdaptor.cpp ===
#include "serviceAdaptor.h"

ServiceAdaptor::ServiceAdaptor(Service* parent)
    : QDBusAbstractAdaptor(parent), m_service(parent)
{
}

QMap<QString, QVariant> ServiceAdaptor::GetConfiguration() {
    return m_service->GetConfiguration();
}

void ServiceAdaptor::ChangeConfiguration(const QString& key, const QDBusVariant& value) {
    m_service->ChangeConfiguration(key, value.variant());
}

// Forward the signal so that it is emitted on the bus
// (QDBusAbstractAdaptor takes care of exposing it)
