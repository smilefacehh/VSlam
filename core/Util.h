#pragma once

#include <string>
#include <opencv2/opencv.hpp>

/**
 * @brief 绘制特征点
 * @param image 图像
 * @param keypoints 特征点
 * @param save 是否保存图像，默认false
 * @param savePath 保存图像路径
*/
void DrawImageWithKeypoints(
    const cv::Mat& image, 
    const std::vector<cv::KeyPoint>& keypoints, 
    bool save=false, 
    const std::string& savePath=std::string());


/**
 * @brief 绘制两幅图像匹配特征点
 * @param image1 图像1
 * @param keypoints1 特征点1
 * @param image2 图像2
 * @param keypoints2 特征点2
 * @param matches 特征点匹配对
 * @param outputImage output 输出图像
 * @param save 是否保存图像，默认false
 * @param savePath 保存图像路径
*/
void DrawImageWithMatchKeypoints(
    const cv::Mat& image1, 
    const std::vector<cv::KeyPoint>& keypoints1, 
    const cv::Mat& image2, 
    const std::vector<cv::KeyPoint>& keypoints2,
    const std::vector<cv::DMatch>& matches,
    cv::Mat& outputImage,
    bool save=false, 
    const std::string& savePath=std::string());

