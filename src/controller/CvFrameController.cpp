/**
 * Created by hachikuji on 2022/1/6.
 */

#include <service/UnDistortService.h>
#include <service/GrayService.h>
#include <service/MeasureCoinService.h>
#include <service/WipeOffService.h>
#include <service/MeasureA4Service.h>
#include "CvFrameController.h"

CvFrameController::CvFrameController(QObject *parent) :
        QObject(parent),
        cvFrameServiceList(new ObjectListModel()) {

    initializeService();

}

CvFrameController &CvFrameController::getInstance() {

    static CvFrameController cvFrameController;
    return cvFrameController;

}

void CvFrameController::initializeService() {

    cvFrameServiceList->append(new WipeOffService());
    cvFrameServiceList->append(new UnDistortService());
    cvFrameServiceList->append(new GrayService());
    cvFrameServiceList->append(new MeasureA4Service());
    cvFrameServiceList->append(new MeasureCoinService());

}

QObject *CvFrameController::getServiceObjectByName(const QString &name) const {
    for (auto *serviceObj : cvFrameServiceList->getRawData()) {
        auto obj = qobject_cast<CvBaseService *>(serviceObj);
        if (obj->getName().toLower() == name.toLower())
            return obj;
    }
    return nullptr;
}

void CvFrameController::processImageByService(const cv::Mat &image) const {

    for (auto service : cvFrameServiceList->getRawData()) {
        auto obj = qobject_cast<CvBaseService *>(service);

        /**
         * CvMediaPlayer is always running. When isActive() is triggered,
         * service will process every frame from media.
         */

        if (obj->getIsActive()) {
            obj->processImage(image);
        }
    }

}
