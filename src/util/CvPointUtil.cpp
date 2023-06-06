/**
 * Created by hachikuji on 2022/1/8.
 */


#include "CvPointUtil.h"
cv::Point2f CvPointUtil::createMidPoint(cv::Point2f &pointA, cv::Point2f &pointB) {

    cv::Point2f point;
    point.x = (pointA.x + pointB.x) * 0.5;
    point.y = (pointA.y + pointB.y) * 0.5;
    return point;

}

float CvPointUtil::getDistance(const cv::Point2f &pointA, const cv::Point2f &pointB) {

    float distance;
    distance = powf((pointA.x - pointB.x), 2) + powf((pointA.y - pointB.y), 2);
    distance = sqrtf(distance);
    return distance;

}

cv::Point CvPointUtil::createMidPoint(cv::Point &pointA, cv::Point &pointB) {

    cv::Point point;
    point.x = (pointA.x + pointB.x) /2;
    point.y = (pointA.y + pointB.y) /2;
    return point;

}

double CvPointUtil::getDistance(const cv::Point &pointA, const cv::Point &pointB) {

    double distance;
    distance = pow((pointA.x - pointB.x), 2) + pow((pointA.y - pointB.y), 2);
    distance = sqrt(distance);
    return distance;

}