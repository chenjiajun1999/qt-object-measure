/**
 * Created by hachikuji on 2022/1/1.
 */

#pragma once

#include <QObject>
#include <QUrl>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler()= default;

    Q_INVOKABLE static bool validateUrl(const QUrl &url) ;
    Q_INVOKABLE static QString getCurrentPath() ;
    Q_INVOKABLE static QString cdUp(const QString& dir) ;
    Q_INVOKABLE static QString convertPathToName(const QString& dir);
    Q_INVOKABLE static QString convertPathToType(const QString& dir);
    Q_INVOKABLE static bool isImage(const QString& dir);

    Q_INVOKABLE static QString fileToIcon(const QUrl &url);
    Q_INVOKABLE static QString imageToIcon(const QUrl &url);
    Q_INVOKABLE static QString fileName(const QUrl &url);
    Q_INVOKABLE static QString convertByte(int byte);

signals:

};
