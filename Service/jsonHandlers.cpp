// === Service/jsonHandlers.cpp ===

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

QMap<QString, QVariant> unMarshal(const QString& jsonText) {
    QMap<QString, QVariant> map;
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    if (!doc.isObject())
        return map;
    QJsonObject obj = doc.object();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
        map.insert(it.key(), it.value().toVariant());
    }
    return map;
}

QString Marshal(const QMap<QString, QVariant>& someStruct) {
    QJsonObject obj;
    for (auto it = someStruct.constBegin(); it != someStruct.constEnd(); ++it) {
        obj.insert(it.key(), QJsonValue::fromVariant(it.value()));
    }
    QJsonDocument doc(obj);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
