#include "ImageProvider.h"

#include <QUuid>

ImageProvider &ImageProvider::getInstance()
{
    static ImageProvider imageProvider;
    return imageProvider;
}

QString ImageProvider::push(const QImage &src) {

    QString key = QUuid::createUuid().toString().
            replace("{", "").
            replace("}", "").
            replace("-", "");

    imageMap.insert(key,src);
    return key;
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {

    return imageMap[id];

}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {

    return QPixmap::fromImage(imageMap[id]);

}


