#pragma once

#include <QObject>
#include <QQuickImageProvider>
#include <QCoreApplication>

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
private:
    QHash<QString, QImage> imageMap;
private:
    ImageProvider(): QQuickImageProvider(QQuickImageProvider::Image){}
    ~ImageProvider() override = default;
public:
    static ImageProvider &getInstance();

    QString push(const QImage &src);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize& requestedSize) override;

};
