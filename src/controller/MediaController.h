#pragma once

#include <QObject>
#include "system/ImageProvider.h"
#include "common/PropertyHelpers.h"

class MediaController: public QObject
{
    Q_OBJECT
    MediaController();
    AUTO_PROPERTY(int, imageWidth, getImageWidth,setImageWidth, imageWidthChanged)
    AUTO_PROPERTY(int, imageHeigth, getImageHeigth,setImageHeigth, imageHeigthChanged)


public:
    static MediaController& getInstance();

    Q_INVOKABLE QString convertVideoTimeToTimeString(int value);
};
