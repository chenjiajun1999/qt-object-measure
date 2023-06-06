#include "FileHandler.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileIconProvider>
#include <QQmlFile>

#include "ImageProvider.h"

bool FileHandler::validateUrl(const QUrl &url)
{
    if (url.isEmpty()) {
        qDebug()<<tr("Must specify a file");
    }
    else if (!QFile::exists(url.toLocalFile())) {
        qDebug()<<tr("File doesn't exist");
    }
    else{
        return true;
    }
    return false;
}

QString FileHandler::getCurrentPath()
{
    return QDir::currentPath();
}

QString FileHandler::cdUp(const QString& dir)
{
    QDir tDir(dir);
    tDir.cdUp();
    return tDir.path();
}

QString FileHandler::convertPathToName(const QString& dir) {
    if (dir.isNull())
        return "";

    int index = dir.lastIndexOf("/");
    return dir.right(dir.length() - index - 1);
}
QString FileHandler::convertPathToType(const QString& dir) {
    if (dir.isNull())
        return "";

    int index = dir.lastIndexOf(".");
    return dir.right(dir.length() - index - 1);
}
bool FileHandler::isImage(const QString& dir) {
    if (dir.isNull())
        return "";
    QString type = convertPathToType(dir);

    return type == "jpg"||type == "jpeg"||type == "png";
}
QString FileHandler::fileToIcon(const QUrl &url)
{
    QString fileName = QQmlFile::urlToLocalFileOrQrc(url);

    qDebug()<<fileName;

    QFileIconProvider provider;
    QFileInfo info = QFileInfo(fileName);

    QIcon icon = provider.icon(info);
    QPixmap pixmap = QPixmap(icon.pixmap(32, 32));

    return ImageProvider::getInstance().push(pixmap.toImage());
}
QString FileHandler::imageToIcon(const QUrl &url)
{
    QString imagePath = QQmlFile::urlToLocalFileOrQrc(url);

    QImage image(imagePath);
    QPixmap pixmap = QPixmap::fromImage(image);

    pixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return ImageProvider::getInstance().push(pixmap.toImage());
}

QString FileHandler::fileName(const QUrl &url)
{
    QString fileName = QQmlFile::urlToLocalFileOrQrc(url);
    return QFileInfo(fileName).fileName();
}

QString FileHandler::convertByte(int byte)
{
    if (byte > (1024 * 1024 * 1024))
        return QString::asprintf("%.2fGB", byte / (1024.0 * 1024.0 * 1024.0));
    else if (byte > (1024 * 1024))
        return QString::asprintf("%.2fMB", byte / (1024.0 * 1024.0));
    else if (byte > (1024))
        return QString::asprintf("%.2fKB", byte / (1024.0));
    else return QString::asprintf("%d bytes", int(byte));
}