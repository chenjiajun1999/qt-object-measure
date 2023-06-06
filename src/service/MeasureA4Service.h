/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_MEASUREA4SERVICE_H
#define QT_OBJECT_MEASURE_MEASUREA4SERVICE_H

#include <utility>

#include "CvBaseService.h"
#include "entity/ContourInfo.h"

class MeasureA4Service : public CvBaseService {
Q_OBJECT
AUTO_PROPERTY(int, refWidth, getRefWidth, setRefWidth, refWidthChanged)
AUTO_PROPERTY(int, refHeight, getRefHeight, setRefHeight, refHeightChanged)
AUTO_PROPERTY(int, scale, getScale, setScale, scaleChanged)
private:

public:
    explicit MeasureA4Service();

    ~MeasureA4Service() override = default;

    void processImage(const cv::Mat &image) override;

    std::vector<ContourInfo> getContours(const cv::Mat &image,
                                         std::pair<int, int> threshold, int minArea, int apexFilter,
                                         bool isCircular, bool isDraw) const;

    static void
    warpImage(const cv::Mat &image, std::vector<cv::Point> &pointList, int width, int height, float partRatio);

    static void reorderRectangleApex(std::vector<cv::Point> &pointList);

};


#endif
