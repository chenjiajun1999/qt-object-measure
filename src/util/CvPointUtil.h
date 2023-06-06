/**
 * Created by hachikuji on 2022/1/9.
 */
#ifndef QT_OBJECT_MEASURE_CVPOINTUTIL_H
#define QT_OBJECT_MEASURE_CVPOINTUTIL_H

#include <opencv2/opencv.hpp>

class CvPointUtil {

public:

    static float getDistance(const cv::Point2f &pointA, const cv::Point2f &pointB);

    static cv::Point2f createMidPoint(cv::Point2f &pointA, cv::Point2f &pointB);

    static double getDistance(const cv::Point &pointA, const cv::Point &pointB);

    static cv::Point createMidPoint(cv::Point &pointA, cv::Point &pointB);

};


#endif //QT_OBJECT_MEASURE_CVPOINTUTIL_H
