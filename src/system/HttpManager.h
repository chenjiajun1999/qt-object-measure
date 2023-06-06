#pragma once

#include <QString>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVariantMap>

#include "common/PropertyHelpers.h"
#include "model/ObjectListModel.h"

class HttpManager : public QObject {
Q_OBJECT

    QString currentProcessedUrl;

AUTO_PROPERTY(QImage*, networkImage, getNetworkImage, setNetworkImage, networkImageChanged);

public:
    explicit HttpManager(QObject *parent = nullptr);

    Q_INVOKABLE void processNetworkImage(const QString &url);

private slots:

    void processImageLinkAnswer(QNetworkReply *reply);
};
