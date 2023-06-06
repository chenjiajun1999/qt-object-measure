/**
 * Created by hachikuji on 2022/1/6.
 */

#ifndef QT_OBJECT_MEASURE_UNDISTORTSERVICE_H
#define QT_OBJECT_MEASURE_UNDISTORTSERVICE_H

#include "CvBaseService.h"

class UnDistortService : public CvBaseService {
Q_OBJECT

AUTO_PROPERTY(bool, cornerRow, getCornerRow, setCornerRow, cornerRowChanged);
AUTO_PROPERTY(bool, cornerCol, getCornerCol, setCornerCol, cornerColChanged);

private:
    cv::Mat cameraMatrix{};
    cv::Mat distCoefficients{};

    QSet<QString> imagePathSet;

public:
    explicit UnDistortService();

    ~UnDistortService() override = default;

    void processImage(const cv::Mat &image) override;

    Q_INVOKABLE void calibrate();
    Q_INVOKABLE void insertImagePath(const QString& path);
    Q_INVOKABLE void deleteImagePath(const QString& path);
    Q_INVOKABLE bool isExistImagePath(const QString& path);
    Q_INVOKABLE void clearImagePath();
    Q_INVOKABLE QVariantList getImagePath();
};



#endif //QT_OBJECT_MEASURE_UNDISTORTSERVICE_H
