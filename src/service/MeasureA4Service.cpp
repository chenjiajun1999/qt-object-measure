/**
 * Created by hachikuji on 2022/1/8.
 */

#include "MeasureA4Service.h"
#include "util/CvPointUtil.h"

#include <utility>

MeasureA4Service::MeasureA4Service() : CvBaseService("MeasureA4Service") {

    scale = 1;
    refWidth = 297 * scale;
    refHeight = 210 * scale;

}

std::vector<ContourInfo>
MeasureA4Service::getContours(const cv::Mat &image, std::pair<int, int> threshold, int minArea = 1000,
                              int apexFilter = 0,
                              bool isCircular = false, bool isDraw = false) const {

    cv::Mat gray;

    //convert it to grayscale, and blur it slightly
    cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, cv::Size(5, 5), 1);


    // find circles
    if (isCircular) {

        auto violet = cv::Scalar(238, 130, 238);
        auto darkRed = cv::Scalar(220, 20, 60);
        std::vector<cv::Vec3f> circleList;
        HoughCircles(gray, circleList, cv::HOUGH_GRADIENT, 1, 50, 100, 50, 10, 150);

        for (auto &circle : circleList) {
            cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
            int radius = cvRound(circle[2]);
            cv::circle(image, center, 5, violet, -1, 8, 0);
            cv::circle(image, center, radius, violet, 3, 8, 0);

            std::vector<cv::Point2f> midPoint;
            midPoint.emplace_back(center.x + radius, center.y);
            midPoint.emplace_back(center.x - radius, center.y);
            midPoint.emplace_back(center.x, center.y - radius);
            midPoint.emplace_back(center.x, center.y + radius);

            line(image, midPoint[0], midPoint[1], violet, 2);
            line(image, midPoint[2], midPoint[3], violet, 2);


            //float refer = (((float) refHeight / (float) image.rows) + ((float) refWidth / (float) image.cols)) * 0.5f;
            float refer = ((float) refHeight / (float) image.rows);
            float r = (float) radius * refer / 10.0f;

            center.x += radius + 10;
            center.y += radius + 10;

            cv::putText(image, cv::format("R = %.2f cm", r), center,
                        cv::FONT_HERSHEY_COMPLEX, 0.8, darkRed);
        }
    }


    // perform edge detection, then perform a dilation + erosion to
    // close gaps in between object edges
    Canny(gray, gray, threshold.first, threshold.second);

    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    dilate(gray, gray, element);
    erode(gray, gray, element);

    // find contours in the edge map
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(gray, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<ContourInfo> finalContourList;

    for (const auto &c : contours) {

        double area = contourArea(c);
        if (area > minArea) {

            // get the area and apex from contours
            std::vector<cv::Point> approx;
            approxPolyDP(c, approx, 0.02 * arcLength(c, true), true);

            if (apexFilter > 0) {
                if (approx.size() == apexFilter) {
                    finalContourList.emplace_back(c, area, approx);
                }
            } else {
                finalContourList.emplace_back(c, area, approx);
            }
        }
    }
    std::sort(finalContourList.begin(), finalContourList.end(), [](const ContourInfo &c1, const ContourInfo &c2) {
        return c1.area > c2.area;
    });


    if (isDraw && !finalContourList.empty()) {
        std::vector<std::vector<cv::Point>> tempContourList;
        for (const auto &c :finalContourList)
            tempContourList.emplace_back(c.finalContours);

        drawContours(image, tempContourList, -1, cv::Scalar(0, 0, 255), 3);
    }

    return finalContourList;
}

void
MeasureA4Service::warpImage(const cv::Mat &image, std::vector<cv::Point> &pointList, int width, int height,
                            float partRatio) {

    reorderRectangleApex(pointList);

    std::vector<cv::Point2f> point2fList;
    for (auto &p : pointList)
        point2fList.emplace_back(p.x, p.y);

    std::vector<cv::Point2f> tempList;
    tempList.emplace_back(0, 0);
    tempList.emplace_back(width, 0);
    tempList.emplace_back(0, height);
    tempList.emplace_back(width, height);

    auto matrix = getPerspectiveTransform(point2fList, tempList);

    cv::Mat warp;
    warpPerspective(image, warp, matrix, cv::Size(width, height));

    auto col = (float) warp.cols;
    auto row = (float) warp.rows;

    cv::Rect rect(col * (1.0f - partRatio) * 0.5f, row * (1.0f - partRatio) * 0.5f, col * partRatio, row * partRatio);
    cv::Mat part = cv::Mat(warp, rect);

    cv::resize(part, warp, image.size());
    warp.copyTo(image);
}

void MeasureA4Service::reorderRectangleApex(std::vector<cv::Point> &pointList) {

    // Index 0 and 3 are determined by sum of x and y
    std::sort(pointList.begin(), pointList.end(), [](const cv::Point &p1, const cv::Point &p2) {
        return (p1.x + p1.y) < (p2.x + p2.y);
    });
    // Index 1 and 2 are determined by division of x and y
    std::sort(pointList.begin() + 1, pointList.end() - 1, [](const cv::Point &p1, const cv::Point &p2) {
        return ((float) p1.x / (float) p1.y) > ((float) p2.x / (float) p2.y);
    });
}

void MeasureA4Service::processImage(const cv::Mat &image) {

    // get A4 paper
    auto contours = getContours(image, std::pair(100, 100), 50000, 4);
    if (contours.empty())
        return;
    auto biggest = contours[0].apex;
    warpImage(image, biggest, refWidth, refHeight, 0.96f);

    // measure rectangle
    contours = getContours(image, std::pair(50, 50), 2000, 4, true);
    if (contours.empty())
        return;

    for (auto c:contours) {

        auto violet = cv::Scalar(238, 130, 238);
        auto darkRed = cv::Scalar(220, 20, 60);

        cv::polylines(image, c.apex, true, violet, 3);
        reorderRectangleApex(c.apex);


        std::vector<cv::Point2f> midPoint;
        midPoint.push_back(CvPointUtil::createMidPoint(c.apex[0], c.apex[1]));
        midPoint.push_back(CvPointUtil::createMidPoint(c.apex[1], c.apex[3]));
        midPoint.push_back(CvPointUtil::createMidPoint(c.apex[2], c.apex[3]));
        midPoint.push_back(CvPointUtil::createMidPoint(c.apex[0], c.apex[2]));
        for (const auto &p:midPoint)
            circle(image, p, 4, violet);

        line(image, midPoint[0], midPoint[2], violet, 2);
        line(image, midPoint[1], midPoint[3], violet, 2);

        float dWidth =
                CvPointUtil::getDistance(midPoint[1], midPoint[3]) * (float) refWidth / (float) image.cols / 10.0f;
        float dHeight =
                CvPointUtil::getDistance(midPoint[0], midPoint[2]) * (float) refHeight / (float) image.rows / 10.0f;

        midPoint[0].x -= 30;
        midPoint[0].y -= 30;
        midPoint[3].x -= 100;

        cv::putText(image, cv::format("%.2f cm", dWidth), midPoint[0],
                    cv::FONT_HERSHEY_COMPLEX, 0.8, darkRed);
        cv::putText(image, cv::format("%.2f cm", dHeight), midPoint[3],
                    cv::FONT_HERSHEY_COMPLEX, 0.8, darkRed);


    }

}