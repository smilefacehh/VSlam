#pragma once

#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

enum FEATURE_TYPE
{
    FAST,
    SURF,
    ORB
};

enum DESCRIPTOR_TYPE
{
    BREIF
};


class Feature
{
public:
    Feature(
        const FEATURE_TYPE& featureType, 
        const DESCRIPTOR_TYPE& descriptorType,
        const int& maxNumFeature
    );

    ~Feature();

    /**
     * @brief 特征点提取
     * @param image 图像
     * @param keypoints output 特征点
    */
    void ExtractKeypoint(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints);

    /**
     * @brief 计算描述子
     * @param image 图像
     * @param keypoints in/output 特征点，当无法计算描述子时会被删除
     * @param descriptors output 描述子
    */
    void ExtractDescriptor(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);

    /**
     * @brief 描述子匹配
     * @param queryDescriptors 查询帧描述子
     * @param refDescriptors 参考帧描述子
     * @param matches output 匹配对
    */
    void FeatureMatching(const cv::Mat& queryDescriptors, const cv::Mat& refDescriptors, std::vector<cv::DMatch>& matches);

private:
    FEATURE_TYPE mFeatureType;
    DESCRIPTOR_TYPE mDescriptorType;
    int mMaxNumFeature;

    cv::Ptr<cv::FeatureDetector> mFeatureDetector;
    cv::Ptr<cv::DescriptorExtractor> mDescriptorExtractor;
    cv::Ptr<cv::DescriptorMatcher> mDescriptorMatcher;
};