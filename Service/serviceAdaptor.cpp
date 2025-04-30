// === Service/serviceAdaptor.cpp ===
#include "serviceAdaptor.h"

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
