/**
 * Created by hachikuji on 2022/1/8.
 */

#include "CvLineUtil.h"

void CvLineUtil::drawVirtualLine(cv::Mat img, const cv::Point2f& p1, const cv::Point2f& p2, cv::Scalar color, int thickness) {
    float n = 5;
    float w = p2.x - p1.x, h = p2.y - p1.y;
    float l = sqrtf(w * w + h * h);

    int m = l / n;
    m = m % 2 ? m : m + 1;
    n = l / m;

    circle(img, p1, 1, color, thickness);
    circle(img, p2, 1, color, thickness);
    // 画中间点
    if (p1.y == p2.y)
    {
        float x1 = fmin(p1.x, p2.x);
        float x2 = fmax(p1.x, p2.x);
        for (float x = x1, n1 = 2 * n; x < x2; x = x + n1)
            line(img, cv::Point2f(x, p1.y), cv::Point2f(x + n, p1.y), color, thickness);
    }
    else if (p1.x == p2.x)
    {
        //垂直线, x = m
        float y1 = fmin(p1.y, p2.y);
        float y2 = fmax(p1.y, p2.y);
        for (float y = y1, n1 = 2 * n; y < y2; y = y + n1)
            line(img, cv::Point2f(p1.x, y), cv::Point2f(p1.x, y + n), color, thickness);
    }
    else
    {
        // 直线方程的两点式：(y-y1)/(y2-y1)=(x-x1)/(x2-x1) -> y = (y2-y1)*(x-x1)/(x2-x1)+y1
        float n1 = n * abs(w) / l;
        float k = h / w;
        float x1 = fmin(p1.x, p2.x);
        float x2 = fmax(p1.x, p2.x);
        for (float x = x1, n2 = 2 * n1; x < x2; x = x + n2)
        {
            cv::Point p3 = cv::Point2f(x, k * (x - p1.x) + p1.y);
            cv::Point p4 = cv::Point2f(x + n1, k * (x + n1 - p1.x) + p1.y);
            line(img, p3, p4, color, thickness);
        }
    }
}