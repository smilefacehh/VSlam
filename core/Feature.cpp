#include "Feature.h"

Feature::Feature(
    const FEATURE_TYPE& featureType, 
    const DESCRIPTOR_TYPE& descriptorType,
    const int& maxNumFeature)
    : mFeatureType(featureType),
      mDescriptorType(descriptorType),
      mMaxNumFeature(maxNumFeature)
{
    mFeatureDetector = cv::ORB::create(mMaxNumFeature);
    // mDescriptorExtractor = cv::xfeatures2d::BriefDescriptorExtractor::create(32, true);
    mDescriptorExtractor = cv::ORB::create(mMaxNumFeature);
    // mDescriptorMatcher = cv::FlannBasedMatcher::create();
    // mDescriptorMatcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::MatcherType::BRUTEFORCE_HAMMING);
    mDescriptorMatcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::MatcherType::FLANNBASED);
}

Feature::~Feature() {}


void Feature::ExtractKeypoint(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints)
{
    keypoints.clear();
    mFeatureDetector->detect(image, keypoints);
}

void Feature::ExtractDescriptor(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors)
{
    mFeatureDetector->compute(image, keypoints, descriptors);
}

void Feature::FeatureMatching(const cv::Mat& queryDescriptors, const cv::Mat& refDescriptors, std::vector<cv::DMatch>& matches)
{
    matches.clear();

    // 测试: Brute-Force匹配. cv::DescriptorMatcher::MatcherType::BRUTEFORCE_HAMMING
    // std::vector<cv::DMatch> orgMatches;
    // mDescriptorMatcher->match(queryDescriptors, refDescriptors, orgMatches);
    // double minDist = 1000, maxDist = 0;
    // for(size_t i = 0; i < orgMatches.size(); ++i)
    // {
    //     const double& dist = orgMatches[i].distance;
    //     if(dist > maxDist) maxDist = dist;
    //     if(dist < minDist) minDist = dist;
    // }
    // minDist = std::max(minDist, 0.02);
    // for(size_t i = 0; i < orgMatches.size(); ++i)
    // {
    //     const double& dist = orgMatches[i].distance;
    //     if(dist < minDist * 2)
    //     {
    //         matches.push_back(orgMatches[i]);
    //     }
    // }

    // FLANN要求描述子类型为CV_32F
    cv::Mat queryDescriptors32F, refDescriptors32F;
    if(queryDescriptors.type() != CV_32F) queryDescriptors.convertTo(queryDescriptors32F, CV_32F);
    if(refDescriptors.type() != CV_32F) refDescriptors.convertTo(refDescriptors32F, CV_32F);

    // 测试1: knnMatch
    std::vector<std::vector<cv::DMatch>> orgMatches;
    mDescriptorMatcher->knnMatch(queryDescriptors32F, refDescriptors32F, orgMatches, 2);
    for(size_t i = 0; i < orgMatches.size(); ++i)
    {
        if(orgMatches[i][0].distance < orgMatches[i][1].distance * 0.75)
        {
            matches.push_back(orgMatches[i][0]);
        }
    }

    // 测试2: match
    // std::vector<cv::DMatch> orgMatches;
    // mDescriptorMatcher->match(queryDescriptors32F, refDescriptors32F, orgMatches);
    // double minDist = 1000, maxDist = 0;
    // for(size_t i = 0; i < orgMatches.size(); ++i)
    // {
    //     const double& dist = orgMatches[i].distance;
    //     if(dist > maxDist) maxDist = dist;
    //     if(dist < minDist) minDist = dist;
    // }

    // minDist = std::max(minDist, 0.02);
    // for(size_t i = 0; i < orgMatches.size(); ++i)
    // {
    //     const double& dist = orgMatches[i].distance;
    //     if(dist < minDist * 0.5)
    //     {
    //         matches.push_back(orgMatches[i]);
    //     }
    // }

    // 测试3: radiusMatch
    // std::vector<std::vector<cv::DMatch>> orgMatches;
    // mDescriptorMatcher->radiusMatch(queryDescriptors32F, refDescriptors32F, orgMatches, 200);
    // for(size_t i = 0; i < orgMatches.size(); ++i)
    // {
    //     if(orgMatches[i].size() == 1 || (orgMatches[i].size() >= 2 && orgMatches[i][0].distance < orgMatches[i][1].distance * 0.75))
    //     {
    //         matches.push_back(orgMatches[i][0]);
    //     }
    // }
}