/**
 * Created by hachikuji on 2022/1/6.
 */
#include "UnDistortService.h"

#include "util/CvImageUtil.h"

UnDistortService::UnDistortService() : CvBaseService("UnDistortService") {

}

void UnDistortService::processImage(const cv::Mat &image) {

    if (cameraMatrix.empty() || distCoefficients.empty()) {
        qDebug() << "Didn't calibrate the camera";
        return;
    }

    cv::undistort(image.clone(), image, cameraMatrix, distCoefficients);

}

void UnDistortService::calibrate() {

    if (imagePathSet.empty()) return;

    QVector<cv::Mat> imageList;


    for (const auto &path : imagePathSet) {

        cv::Mat src = CvImageUtil::loadCvImage(path);
        if (src.empty()) continue;
        imageList.append(src);

    }

    cv::Size imageSize;
    cv::Size patternSize = cv::Size(4, 8);

    std::vector<std::vector<cv::Point2f>> cornerList;

    for (const auto &src : imageList) {

        if (imageSize.width == 0) {
            imageSize.width = src.cols;
            imageSize.height = src.rows;
        }

        std::vector<cv::Point2f> cornerBuf;

        if (!findChessboardCorners(src, patternSize, cornerBuf)) {
            qDebug() << "can not find chessboard corners!";
            return;
        }

        cv::Mat gray;
        cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

        cv::find4QuadCornerSubpix(gray, cornerBuf, cv::Size(5, 5));
        cornerList.push_back(cornerBuf);

        //cv::drawChessboardCorners(gray, patternSize, cornerBuf, true);
        //cv::imshow("corner", gray);
    }

    std::vector<cv::Point3f> cornerBuf;
    std::vector<std::vector<cv::Point3f>> objectPointList;

    for (int i = 0; i < patternSize.height; i++)
        for (int j = 0; j < patternSize.width; j++)
            cornerBuf.emplace_back(j, i , 0);

    for (int i = 0; i < imageList.length(); ++i)
        objectPointList.push_back(cornerBuf);

    cameraMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    distCoefficients = cv::Mat(1, 5, CV_32FC1, cv::Scalar::all(0));
    std::vector<cv::Mat> tvecsMat;
    std::vector<cv::Mat> rvecsMat;


    cv::calibrateCamera(objectPointList, cornerList, imageSize,
                        cameraMatrix, distCoefficients,
                        rvecsMat, tvecsMat, 0);

    std::cout << cameraMatrix << std::endl;
    std::cout << distCoefficients << std::endl;

}

void UnDistortService::insertImagePath(const QString &path) {

    if (path.isNull())
        return;

    if (!path.contains("file:///")) {
        imagePathSet.insert("file:///" + path);
        return;
    }

    imagePathSet.insert(path);

}

void UnDistortService::deleteImagePath(const QString &path) {

    imagePathSet.remove(path);

}

bool UnDistortService::isExistImagePath(const QString &path) {

    return imagePathSet.contains(path);

}

void UnDistortService::clearImagePath() {

    imagePathSet.clear();

}

QVariantList UnDistortService::getImagePath() {

    QVariantList list;

    for (const auto &path :imagePathSet)
        list.append(path);

    return list;
}
