/**
 * Created by hachikuji on 2022/1/6.
 */

#include "GrayService.h"

GrayService::GrayService() : CvBaseService("GrayService") {}

void GrayService::processImage(const cv::Mat &image) {

    cv::Mat gray;

    cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cvtColor(gray, image, cv::COLOR_GRAY2BGR);


}


