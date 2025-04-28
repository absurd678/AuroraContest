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

    QMap<QString, QVariant> GetConfiguration();

public slots:
    void ChangeConfiguration(const QString& key, const QVariant& value);

signals:
    void configurationChanged(const QMap<QString, QVariant>& configuration);

private:
    QMap<QString, QVariant> conf;
};
