#include "MediaController.h"

#include <QTime>
MediaController::MediaController() = default;

QString MediaController::convertVideoTimeToTimeString(int value)
{
    int seconds = (value/1000) % 60;
    int minutes = (value/60000) % 60;
    int hours = (value/3600000) % 24;

    QTime time(hours, minutes,seconds);
    return time.toString();
}

MediaController &MediaController::getInstance()
{
    static MediaController mediaController;
    return mediaController;
}


