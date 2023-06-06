/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_MEASURECOINSERVICE_H
#define QT_OBJECT_MEASURE_MEASURECOINSERVICE_H

#include "CvBaseService.h"

class MeasureCoinService : public CvBaseService {
Q_OBJECT

AUTO_PROPERTY(double, pixelsPerMetric, getPixelsPerMetric, setPixelsPerMetric, pixelsPerMetricChanged);
AUTO_PROPERTY(double, refer, getRefer, setRefer, referChanged);


private:


public:
    explicit MeasureCoinService();

    ~MeasureCoinService() override = default;

    void processImage(const cv::Mat &image) override;

};

#endif //QT_OBJECT_MEASURE_MEASURECOINSERVICE_H
