/**
 * Created by hachikuji on 2021/12/28.
 */
#ifndef OBJECT_MEASURE_CAMERA_H
#define OBJECT_MEASURE_CAMERA_H

#include <QQuickFramebufferObject>
#include <QTimer>
#include <QUuid>

#include <stack>

#include <opencv2/opencv.hpp>

#include "common/PropertyHelpers.h"
#include "controller/CvFrameController.h"
#include "CvMediaPlayerRenderer.h"

class CvMediaPlayer : public QQuickFramebufferObject {

Q_OBJECT

AUTO_PROPERTY(int, srcWidth, getWidth, setWidth, widthChanged)
AUTO_PROPERTY(int, srcHeight, getHeight, setHeight, heightChanged)
AUTO_PROPERTY(int, fps, getFps, setFps, fpsChanged)

    // video parameters
AUTO_PROPERTY(double, duration, getDuration, setDuration, durationChanged)
AUTO_PROPERTY(double, position, getPosition, setPosition, positionChanged)
public:
    explicit CvMediaPlayer(QQuickItem *parent = nullptr);

    Renderer *createRenderer() const override;

    Q_INVOKABLE void openPicture(QString path);

    Q_INVOKABLE void openVideo(QString path);

    Q_INVOKABLE void openWebPicture(const QString &path);

    Q_INVOKABLE void openCamera();

    Q_INVOKABLE void openWebCam(const QString &ip);

    Q_INVOKABLE QString savePicture(QString path);

    Q_INVOKABLE void stop();


private:

    void updateFrame();

    void cvMat2QImage(const cv::Mat &src);



    void initializeCam();

private slots:

    void run();

private:

    std::stack<QImage *> *frameCache{};
    cv::Mat frame;
    cv::VideoCapture capture;
    QTimer *timer{};
    int TIMER_TIMEOUT = 30;

};

#endif