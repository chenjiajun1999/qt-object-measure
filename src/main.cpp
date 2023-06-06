#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "component/CvMediaPlayer.h"

#include "system/HttpManager.h"
#include "system/FileHandler.h"
#include "system/ImageProvider.h"

#include "controller/MediaController.h"



int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Personal");
    QGuiApplication::setOrganizationDomain("China");

    qmlRegisterType<CvMediaPlayer>("online.hachikuji.component", 1, 0, "CvMediaPlayer");
    qmlRegisterType<ObjectListModel>("online.hachikuji.model", 1, 0, "ObjectListModel");

    QQmlApplicationEngine engine;

    //Register managers
    engine.rootContext()->setContextProperty("cvFrameController", &CvFrameController::getInstance());
    engine.rootContext()->setContextProperty("mediaController", &MediaController::getInstance());
    engine.rootContext()->setContextProperty("httpManager", new HttpManager());
    engine.rootContext()->setContextProperty("fileHandler", new FileHandler());


    //Register processed video output
    engine.rootContext()->engine()->addImageProvider(QLatin1String("imageProvider"), &ImageProvider::getInstance());
    engine.rootContext()->setContextProperty("imageProvider", &MediaController::getInstance());

    const QUrl url("qrc:/views/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}