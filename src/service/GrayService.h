/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_GRAYSERVICE_H
#define QT_OBJECT_MEASURE_GRAYSERVICE_H

#include "CvBaseService.h"

class GrayService : public CvBaseService {
Q_OBJECT

public:
    explicit GrayService();

    ~GrayService() override = default;

    void processImage(const cv::Mat &image) override;
};


#endif //QT_OBJECT_MEASURE_GRAYSERVICE_H
