/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_CVBASESERVICE_H
#define QT_OBJECT_MEASURE_CVBASESERVICE_H

#include "common/PropertyHelpers.h"
#include <opencv2/opencv.hpp>

class CvBaseService : public QObject {
Q_OBJECT

AUTO_PROPERTY(QString, name, getName, setName, nameChanged)
AUTO_PROPERTY(QString, repoLink, getRepoLink, setRepoLink, repoLinkChanged)
AUTO_PROPERTY(QString, iconSource, getIconSource, setIconSource, iconSourceChanged)
AUTO_PROPERTY(bool, isActive, getIsActive, setIsActive, isActiveChanged);

public:
    explicit CvBaseService(QString name);

    ~CvBaseService() override = default;

    virtual void processImage(const cv::Mat &image) = 0;

};


#endif //QT_OBJECT_MEASURE_CVBASESERVICE_H
