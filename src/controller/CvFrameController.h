/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_CVFRAMECONTROLLER_H
#define QT_OBJECT_MEASURE_CVFRAMECONTROLLER_H

#include "common/PropertyHelpers.h"
#include "model/ObjectListModel.h"

#include <opencv2/opencv.hpp>

class CvFrameController : public QObject {

Q_OBJECT

AUTO_PROPERTY(ObjectListModel *, cvFrameServiceList, getCvFrameServiceList, setCvFrameServiceList,
              cvFrameServiceListChanged)

private:
    explicit CvFrameController(QObject *parent = nullptr);

public:
    ~CvFrameController() override = default;

    static CvFrameController &getInstance();

    void initializeService();

    Q_INVOKABLE QObject *getServiceObjectByName(const QString &name) const;

    Q_INVOKABLE void processImageByService(const cv::Mat &image) const;

};


#endif
