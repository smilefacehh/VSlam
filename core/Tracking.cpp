#include "Tracking.h"

Tracking::Tracking()
    : mbInitialized(false)
{
    mFeaturePtr = std::shared_ptr<Feature>(new Feature(FEATURE_TYPE::ORB, DESCRIPTOR_TYPE::BREIF, Config::Instance().maxNumFeatures));
    mCameraK = (cv::Mat_<float>(3, 3) << 517.3, 0, 318.6, 0, 516.5, 255.3, 0, 0, 1);
}

Tracking::~Tracking() {}

void Tracking::Process(const cv::Mat& image)
{
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    ExtractFeatureAndDescriptor(image, keypoints, descriptors);

    mCurrFrameKeypoints = keypoints;
    mCurrFrameDescriptors = descriptors;

    if(!mbInitialized)
    {
        mFrameImagesForInitialize.push_back(image);
        mFrameKeypointsForInitialize.push_back(keypoints);
        mFrameDescriptorsForInitialize.push_back(descriptors);
        mbInitialized = Initialize();
        return;
    }


}

void Tracking::ExtractFeatureAndDescriptor(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors)
{
    if(mFeaturePtr)
    {
        mFeaturePtr->ExtractKeypoint(image, keypoints);
        mFeaturePtr->ExtractDescriptor(image, keypoints, descriptors);
    }
}

bool Tracking::Initialize()
{
    bool result = false;
    if(mFrameImagesForInitialize.size() >= 2)
    {
        // 1.特征点匹配
        std::vector<cv::DMatch> matches;
        mFeaturePtr->FeatureMatching(mFrameDescriptorsForInitialize[0], mFrameDescriptorsForInitialize.back(), matches);
        int numKpMatches = (int)matches.size();

        // 匹配特征点数太多，认为间隔不够
        if(numKpMatches >= Config::Instance().maxNumFeatures * 0.9)
        {
            mFrameImagesForInitialize.pop_back();
            mFrameKeypointsForInitialize.pop_back();
            mFrameDescriptorsForInitialize.pop_back();
            return result;
        }
        // 匹配特征点数太少，认为间隔太大
        else if(numKpMatches <= Config::Instance().maxNumFeatures * 0.1)
        {
            mFrameImagesForInitialize.erase(mFrameImagesForInitialize.begin());
            mFrameKeypointsForInitialize.erase(mFrameKeypointsForInitialize.begin());
            mFrameDescriptorsForInitialize.erase(mFrameDescriptorsForInitialize.begin());
            return result;
        }

        // 2.计算本质矩阵E，恢复R、t
        std::vector<cv::Point2f> keypointsQuery;
        std::vector<cv::Point2f> keypointsTrain;
        for(const auto& match:matches)
        {
            const cv::KeyPoint& kpQuery = mFrameKeypointsForInitialize[0][match.queryIdx];
            const cv::KeyPoint& kpTrain = mFrameKeypointsForInitialize.back()[match.trainIdx];
            keypointsQuery.push_back(cv::Point2f(kpQuery.pt.x, kpQuery.pt.y));
            keypointsTrain.push_back(cv::Point2f(kpTrain.pt.x, kpTrain.pt.y));
        }
        cv::Mat E = cv::findEssentialMat(keypointsQuery, keypointsTrain, mCameraK);
        cv::Mat R, t;
        int inliers = cv::recoverPose(E, keypointsQuery, keypointsTrain, mCameraK, R, t);

        // 内点数太少，认为匹配不够准确
        if(inliers <= numKpMatches * 0.2)
        {
            mFrameImagesForInitialize.erase(mFrameImagesForInitialize.begin());
            mFrameKeypointsForInitialize.erase(mFrameKeypointsForInitialize.begin());
            mFrameDescriptorsForInitialize.erase(mFrameDescriptorsForInitialize.begin());
        }
        // OK，认为初始化成功
        else
        {
            Eigen::Matrix3d rotation;
            Eigen::Vector3d translation;
            for(int i = 0; i < 9; i++) rotation(i/3, i%3) = R.at<double>(i/3, i%3);
            for(int i = 0; i < 3; i++) translation(i, 0) = t.at<double>(i, 0);
            mCurrFramePose.linear() = rotation;
            mCurrFramePose.translation() = translation;

            std::cout << "initialize succeed!" << std::endl;
            std::cout << "rotation:\n" << rotation << std::endl;
            std::cout << "translation:\n" << translation.transpose() << std::endl;
            result = true;
        }
        
        // cv::Mat matchImg;
        // DrawImageWithMatchKeypoints(mFrameImagesForInitialize[0], mFrameKeypointsForInitialize[0], mFrameImagesForInitialize.back(), mFrameKeypointsForInitialize.back(), matches, matchImg);

        // std::cout << R << std::endl;
        // std::cout << t << std::endl;
        // std::cout << cv::norm(t) << std::endl;
        // std::cout << "inlies:" << inliers << std::endl;
        // std::cout << "kp matches:" << matches.size() << std::endl;
    }
    return result;
}


void Tracking::Triangulation(const Eigen::Isometry3d& pose1, const std::vector<cv::KeyPoint>& keypoints1, const Eigen::Isometry3d& pose2, const std::vector<cv::KeyPoint>& keypoints2, std::vector<cv::Point3f>& worldPt)
{
    cv::triangulatePoints()
}