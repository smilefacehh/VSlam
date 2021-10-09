#pragma once

#include <eigen3/Eigen/Geometry>

#include <opencv2/opencv.hpp>

#include "Feature.h"
#include "Config.h"
#include "Util.h"

class Tracking
{
public:
    Tracking();
    ~Tracking();

    /**
     * 处理一帧
    */
    void Process(const cv::Mat& image);

private:
    /**
     * @brief 提取特征点、描述子
     * @param image 图像
     * @param keypoints output 特征点
     * @param descriptors output 描述子
    */
    void ExtractFeatureAndDescriptor(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);

    /**
     * @brief 初始化
    */
    bool Initialize();

    /**
     * @brief 三角化
    */
    void Tracking::Triangulation(const Eigen::Isometry3d& pose1, const std::vector<cv::KeyPoint>& keypoints1, const Eigen::Isometry3d& pose2, const std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::Point3f>& worldPt);

    // 是否初始化
    bool mbInitialized;

    // 用于初始化的一些帧数据
    std::vector<cv::Mat> mFrameImagesForInitialize;
    std::vector<std::vector<cv::KeyPoint>> mFrameKeypointsForInitialize;
    std::vector<cv::Mat> mFrameDescriptorsForInitialize;

    // 前一帧数据
    cv::Mat mLastFrameImage;
    std::vector<cv::KeyPoint> mLastFrameKeypoints;
    cv::Mat mLastFrameDescriptors;
    Eigen::Isometry3d mLastFramePose;

    // 当前帧数据
    cv::Mat mCurrFrameImage;
    std::vector<cv::KeyPoint> mCurrFrameKeypoints;
    cv::Mat mCurrFrameDescriptors;
    Eigen::Isometry3d mCurrFramePose;

    // 特征提取、匹配
    std::shared_ptr<Feature> mFeaturePtr;

    // 相机内参
    cv::Mat mCameraK;
};