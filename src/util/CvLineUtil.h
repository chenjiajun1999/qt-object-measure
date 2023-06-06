//
// Created by Alberta on 2022/1/9.
//

#ifndef QT_OBJECT_MEASURE_CVLINEUTIL_H
#define QT_OBJECT_MEASURE_CVLINEUTIL_H

#include <opencv2/opencv.hpp>


class CvLineUtil {

public:
    static void drawVirtualLine(cv::Mat img, const cv::Point2f& p1, const cv::Point2f& p2, cv::Scalar color, int thickness);

};


#endif //QT_OBJECT_MEASURE_CVLINEUTIL_H
