/**
 * Created by hachikuji on 2022/1/6.
 */
#include "MeasureCoinService.h"

#include "util/CvPointUtil.h"

MeasureCoinService::MeasureCoinService() : CvBaseService("MeasureCoinService") {

    pixelsPerMetric = 1.0f;
    refer = 25.0f;

}


void MeasureCoinService::processImage(const cv::Mat &image) {


    cv::Mat gray;

    //convert it to grayscale, and blur it slightly
    cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, cv::Size(7, 7), 0);

    // find circles
    std::vector<cv::Vec3f> circleList;
    HoughCircles(gray, circleList, cv::HOUGH_GRADIENT, 1, 40, 100, 70, 40, 150);
    for (auto & circle : circleList)
    {
        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        int radius = cvRound(circle[2]);
        cv::circle(image, center, 5, cv::Scalar(0, 255, 0), -1, 8, 0);
        cv::circle(image, center, radius, cv::Scalar(155, 50, 255), 3, 8, 0);
    }

    // perform edge detection, then perform a dilation + erosion to
    // close gaps in between object edges
    Canny(gray, gray, 50, 100);

    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)); //隔开物体
    dilate(gray, gray, element);
    erode(gray, gray, element);

    // find contours in the edge map
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    std::sort(contours.begin(), contours.end(), [](std::vector<cv::Point> c1, std::vector<cv::Point> c2) {
        return c1[0].x < c2[0].x;
    });



    // loop over the contours individually
    bool  isRefer = false;

    for (const auto &c:contours) {

        //if the contour is not sufficiently large, ignore it
        if (contourArea(c) < 500) continue;

        // compute the rotated bounding box of the contour
        cv::RotatedRect box = minAreaRect(c);
        cv::Point2f boxPoints[4];
        box.points(boxPoints);

        cv::Point2f pointA = CvPointUtil::createMidPoint(boxPoints[0], boxPoints[1]);
        cv::Point2f pointB = CvPointUtil::createMidPoint(boxPoints[1], boxPoints[2]);
        cv::Point2f pointC = CvPointUtil::createMidPoint(boxPoints[2], boxPoints[3]);
        cv::Point2f pointD = CvPointUtil::createMidPoint(boxPoints[3], boxPoints[0]);

        circle(image, pointA, 2, cv::Scalar(0, 0, 255));
        circle(image, pointB, 2, cv::Scalar(0, 0, 255));
        circle(image, pointC, 2, cv::Scalar(0, 0, 255));
        circle(image, pointD, 2, cv::Scalar(0, 0, 255));

        line(image, pointA, pointC, cv::Scalar(255, 0, 0),2);
        line(image, pointD, pointB, cv::Scalar(255, 0, 0),2);

        double dWidth = CvPointUtil::getDistance(pointA, pointC);
        double dHeight = CvPointUtil::getDistance(pointD, pointB);

        // get pixelsPerMetric by the smallest object
        if(!isRefer){
            pixelsPerMetric = dWidth / refer;
            isRefer = true;
        }

        // filter
        if(dHeight / pixelsPerMetric  <=2.5 || dWidth / pixelsPerMetric<=2.5)
            continue;

        // classify round or rectangle
        if (abs(dWidth - dHeight) < 2 * pixelsPerMetric )
        {
            putText(image, cv::format("R=%.0f mm", dWidth / pixelsPerMetric), boxPoints[1],
                    cv::FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0, 0, 0));

        }else{

        pointA.x = pointA.x - 15;
        pointA.y = pointA.y - 10;
        pointB.x = pointB.x + 10;

        cv::putText(image, cv::format("%.0f mm", dHeight / pixelsPerMetric), pointA,
                    cv::FONT_HERSHEY_COMPLEX, 0.8,cv::Scalar(0, 0, 0));
        cv::putText(image, cv::format("%.0f mm", dWidth / pixelsPerMetric), pointB,
                    cv::FONT_HERSHEY_COMPLEX, 0.8,cv::Scalar(0, 0, 0));

        }

        for (int i = 0; i <= 3; i++) {
            line(image, boxPoints[i], boxPoints[(i + 1) % 4], cv::Scalar(0, 255, 0),2);
        }




    }

}
