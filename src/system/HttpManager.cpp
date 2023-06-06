#include "HttpManager.h"
#include <string>
#include "controller/MediaController.h"

HttpManager::HttpManager(QObject *parent) :
        QObject(parent) {

    //Check ssl support
//    qDebug() << "SupportsSSL: " << QSslSocket::supportsSsl();
//    qDebug() << QSslSocket::sslLibraryBuildVersionString();
//    qDebug() << QSslSocket::sslLibraryVersionString();

}

void HttpManager::processNetworkImage(const QString &url) {
    if (currentProcessedUrl == url) {
        //Image cached, no need to download
        if (networkImage != nullptr) {
            //FrameworkManager::getInstance().processImageByFrameworks(*networkImage);
        }
    } else {
        currentProcessedUrl = url;
        networkImage = nullptr;

        auto *networkAccessManager = new QNetworkAccessManager(this);
        connect(networkAccessManager, SIGNAL(finished(QNetworkReply * )), this,
                SLOT(processImageLinkAnswer(QNetworkReply * )));
        connect(networkAccessManager, &QNetworkAccessManager::finished, networkAccessManager,
                &QNetworkAccessManager::deleteLater);
        networkAccessManager->get(QNetworkRequest{QUrl(url)});
    }
}

void HttpManager::processImageLinkAnswer(QNetworkReply *reply) {
    QImage img;
    img.load(reply, nullptr);
    img = img.convertToFormat(QImage::Format_RGB888);
    img = img.scaledToHeight(MediaController::getInstance().getImageHeigth());

    setNetworkImage(new QImage{img});
    //FrameworkManager::getInstance().processImageByFrameworks(*networkImage);
}