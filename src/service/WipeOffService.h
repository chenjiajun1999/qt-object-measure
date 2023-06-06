/**
 * Created by hachikuji on 2022/1/8.
 */
#include "CvBaseService.h"

#ifndef QT_OBJECT_MEASURE_WIPEOFFSERVICE_H
#define QT_OBJECT_MEASURE_WIPEOFFSERVICE_H


class WipeOffService : public CvBaseService{

public:
    explicit WipeOffService();

    ~WipeOffService() override = default;

    void processImage(const cv::Mat &image) override;

};


#endif //QT_OBJECT_MEASURE_WIPEOFFSERVICE_H
