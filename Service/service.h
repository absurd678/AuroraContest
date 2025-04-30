// === Service/service.h ===
#pragma once
#include <QObject>
#include <QMap>
#include <QVariant>

class Service : public QObject {
    Q_OBJECT
public:
    Service(QObject* parent = nullptr);
    ~Service();

    QVariantMap GetConfiguration();

public slots:
    void ChangeConfiguration(const QString& key, const QVariant& value);

signals:
    void configurationChanged(const QVariantMap& configuration);

private:
    QVariantMap conf;
    QString m_configPath;   // путь к конфиг файлу приложения
};
