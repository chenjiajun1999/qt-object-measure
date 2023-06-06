/**
 * Created by hachikuji on 2021/12/28.
 */

#include "CvMediaPlayer.h"

#include <utility>

#include "util/CvImageUtil.h"
#include "service/CvBaseService.h"

CvMediaPlayer::CvMediaPlayer(QQuickItem *parent) : QQuickFramebufferObject(parent) {

    frameCache = new std::stack<QImage *>();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    openCamera();

}

QQuickFramebufferObject::Renderer *CvMediaPlayer::createRenderer() const {

    auto renderer = new CvMediaPlayerRenderer();
    renderer->setWindow(window());
    renderer->setFrameCache(frameCache);
    return renderer;

}

void CvMediaPlayer::run() {

    updateFrame();
    CvFrameController::getInstance().processImageByService(frame);
    cvMat2QImage(frame);
    update();
}

void CvMediaPlayer::openPicture(QString path) {

    stop();

    frame = CvImageUtil::loadCvImage(std::move(path));
    srcWidth = frame.cols;
    srcHeight = frame.rows;

    CvFrameController::getInstance().processImageByService(frame);
    cvMat2QImage(frame);
    update();
}

void CvMediaPlayer::openWebPicture(const QString &path) {

    stop();
    capture.open(path.toStdString());
    if (!capture.isOpened()) {
        qDebug() << "Failed to open video";
        return;
    }
    initializeCam();
    run();
}


/**
 * 差进度条
 * @param path
 */
void CvMediaPlayer::openVideo(QString path) {

    stop();
    path.replace("file:///", "");

    capture.open(path.toStdString());
    if (!capture.isOpened()) {
        qDebug() << "Failed to open video";
        return;
    }
    initializeCam();
    updateFrame();
    cvMat2QImage(frame);
    timer->start(TIMER_TIMEOUT);

}


void CvMediaPlayer::openCamera() {

    stop();
    capture.open(0);

    if (!capture.isOpened()) {
        qDebug() << "Failed to open camera";
        return;
    }

    initializeCam();
    updateFrame();
    cvMat2QImage(frame);
    timer->start(TIMER_TIMEOUT);

}

void CvMediaPlayer::openWebCam(const QString &ip) {

    stop();
    capture.open(ip.toStdString());

    if (!capture.isOpened()) {
        qDebug() << "Failed to open camera";
        return;
    }

    initializeCam();
    updateFrame();
    cvMat2QImage(frame);
    timer->start(TIMER_TIMEOUT);

}

QString CvMediaPlayer::savePicture(QString path) {

    path.replace("file:///", "");

    timer->stop();

    QString uuid = QUuid::createUuid().toString().
            replace("{", "").
            replace("}", "").
            replace("-", "");

    cv::Mat temp;

    //  correct color
    cv::cvtColor(frame, temp, cv::COLOR_BGR2RGB);

    cv::imwrite(path.toStdString() + "/" + uuid.toStdString() + ".jpg", temp);

    qDebug() << "success to save " + uuid + ".jpg";

    timer->start(TIMER_TIMEOUT);

    return uuid + ".jpg";
}


void CvMediaPlayer::stop() {

    timer->stop();
    capture.release();

}

void CvMediaPlayer::initializeCam() {


    // Try to set 1280 X 720
    capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    capture.set(cv::CAP_PROP_FPS, 10);

    // Retrieve the width and height of the captured frame
    srcWidth = (int) capture.get(cv::CAP_PROP_FRAME_WIDTH);
    srcHeight = (int) capture.get(cv::CAP_PROP_FRAME_HEIGHT);

    qDebug() << "capture size=" << srcWidth << "x" << srcHeight;

    //Be determined by interrupt timer
    fps = 1000 / TIMER_TIMEOUT;

}

void CvMediaPlayer::updateFrame() {

    if (!capture.isOpened()) {
        qDebug() << "Failed to open camera";
        return;
    }

    capture >> frame;

    if (frame.empty()) {
        qDebug() << "Failed to retrieve frame";
        return;
    }
}

void CvMediaPlayer::cvMat2QImage(const cv::Mat &src) {

    if (!frameCache) {
        delete frameCache->top();
        frameCache->pop();
    }

    QImage *image;

    // fix correct
    cv::cvtColor(src, src, cv::COLOR_BGR2RGB);

    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (src.type() == CV_8UC1) {
        image = new QImage(src.cols, src.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image->setColorCount(256);
        for (int i = 0; i < 256; i++) {
            image->setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = src.data;
        for (int row = 0; row < src.rows; row++) {
            uchar *pDest = image->scanLine(row);
            memcpy(pDest, pSrc, src.cols);
            pSrc += src.step;
        }
    }
        // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (src.type() == CV_8UC3) {
        // Copy input Mat
        const auto *pSrc = (const uchar *) src.data;
        // Create QImage with same dimensions as input Mat
        image = new QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
        image->rgbSwapped();
    } else if (src.type() == CV_8UC4) {
        // Copy input Mat
        const auto *pSrc = (const uchar *) src.data;
        // Create QImage with same dimensions as input Mat
        image = new QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        image->copy();
    } else {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        image = new QImage();
    }

    frameCache->push(image);
}







