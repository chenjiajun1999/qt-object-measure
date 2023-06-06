/**
 * Created by hachikuji on 2022/1/6.
 */

#include "CvBaseService.h"

#include <utility>

CvBaseService::CvBaseService(QString name) :
        name(std::move(name)),
        repoLink("https://github.com/opencv/opencv"),
        iconSource("qrc:/assets/image/opencv.png") {}