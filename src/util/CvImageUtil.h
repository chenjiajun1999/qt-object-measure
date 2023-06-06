/**
 * Created by hachikuji on 2021/1/7.
 */

#ifndef QT_OBJECT_MEASURE_CVIMAGEUTIL_H
#define QT_OBJECT_MEASURE_CVIMAGEUTIL_H

#include <QImage>

#include <opencv2/opencv.hpp>

class CvImageUtil {

public:

    static cv::Mat loadCvImage(QString path);
    static cv::Mat qImage2CvMat(const QImage &image);

};


#endif
