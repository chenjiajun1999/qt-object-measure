/**
 * Created by hachikuji on 2022/1/8.
 */
#include "WipeOffService.h"
WipeOffService::WipeOffService() : CvBaseService("WipeOffService") {

    //setIsActive(true);

}
/**
 * remove the watermark from DroidCam
 * @param image
 */
void WipeOffService::processImage(const cv::Mat &image){

    auto  col =  (float)image.cols;
    auto row = (float)image.rows;

    cv::Rect rect(col*0.03f, row*0.03f, col*0.97f, row*0.97f);
    cv::Mat part =  cv::Mat(image, rect);

    cv::Mat temp;
    cv::resize(part,temp,cv::Size(col,row));
    temp.copyTo(image);
}