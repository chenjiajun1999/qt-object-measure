/**
 * Created by hachikuji on 2022/1/9.
 */


#ifndef QT_OBJECT_MEASURE_CONTOURINFO_H
#define QT_OBJECT_MEASURE_CONTOURINFO_H

struct ContourInfo
{
    std::vector<cv::Point> finalContours;
    int area;
    std::vector<cv::Point> apex;
    ContourInfo(std::vector<cv::Point> finalContours,int area,std::vector<cv::Point> apex):
            finalContours(std::move(finalContours)),
            area(area),
            apex(std::move(apex)){}
};

#endif
