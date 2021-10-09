#include "Util.h"

void DrawImageWithKeypoints(const cv::Mat& image, const std::vector<cv::KeyPoint>& keypoints, bool save, const std::string& savePath)
{

}

void DrawImageWithMatchKeypoints(
    const cv::Mat& image1, 
    const std::vector<cv::KeyPoint>& keypoints1, 
    const cv::Mat& image2, 
    const std::vector<cv::KeyPoint>& keypoints2,
    const std::vector<cv::DMatch>& matches,
    cv::Mat& outputImage,
    bool save, 
    const std::string& savePath)
{
    cv::drawMatches(image1, keypoints1, image2, keypoints2, matches, outputImage);
    
    if(save & !savePath.empty())
    {
        cv::imwrite(savePath, outputImage);
    };

    cv::imshow("matching-image", outputImage);
    cv::waitKey(-1);
    cv::destroyWindow("matching-image");
}
