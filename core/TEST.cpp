#include <chrono>

#include "Util.h"
#include "Feature.h"
#include "Tracking.h"

// 测试特征点提取、描述子、匹配
void testFeatureMatching()
{
    cv::Mat image1, image2;
    image1 = cv::imread("../data/1305031102.175304.png");
    image2 = cv::imread("../data/1305031102.411258.png");

    cv::Mat grayImg1, grayImg2;
    cv::cvtColor(image1, grayImg1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image2, grayImg2, cv::COLOR_BGR2GRAY);

    std::vector<cv::KeyPoint> keypointsImg1, keypointsImg2;
    cv::Mat descriptorImg1, descriptorImg2;
    std::vector<cv::DMatch> matches;

    std::shared_ptr<Feature> featurePtr = std::shared_ptr<Feature>(new Feature(FEATURE_TYPE::ORB, DESCRIPTOR_TYPE::BREIF, 100));
    featurePtr->ExtractKeypoint(grayImg1, keypointsImg1);
    featurePtr->ExtractDescriptor(grayImg1, keypointsImg1, descriptorImg1);
    
    auto start_t = std::chrono::steady_clock::now();
    featurePtr->ExtractKeypoint(grayImg2, keypointsImg2);
    auto end_t = std::chrono::steady_clock::now();
    std::cout << "feature extract cost:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count() << "ms" << std::endl;

    start_t = std::chrono::steady_clock::now();
    featurePtr->ExtractDescriptor(grayImg2, keypointsImg2, descriptorImg2);
    end_t = std::chrono::steady_clock::now();
    std::cout << "descriptor cost:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count() << "ms" << std::endl;

    start_t = std::chrono::steady_clock::now();
    featurePtr->FeatureMatching(descriptorImg1, descriptorImg2, matches);
    end_t = std::chrono::steady_clock::now();
    std::cout << "flann matching cost:" << std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count() << "ms" << std::endl;

    cv::Mat outputMatchingImg;
    DrawImageWithMatchKeypoints(image1, keypointsImg1, image2, keypointsImg2, matches, outputMatchingImg, true, "../output/orb-matching.png");
}

void testTrackingInitialize()
{
    std::shared_ptr<Tracking> trackingPtr = std::shared_ptr<Tracking>(new Tracking());
    cv::Mat image1, image2, image3, image4;
    image1 = cv::imread("../data/1305031102.175304.png");
    image2 = cv::imread("../data/1305031102.211214.png");
    image3 = cv::imread("../data/1305031102.411258.png");
    image4 = cv::imread("../data/1305031103.343223.png");

    cv::Mat grayImg1, grayImg2, grayImg3, grayImg4;
    cv::cvtColor(image1, grayImg1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image2, grayImg2, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image3, grayImg3, cv::COLOR_BGR2GRAY);
    cv::cvtColor(image4, grayImg4, cv::COLOR_BGR2GRAY);

    trackingPtr->Process(grayImg1);
    trackingPtr->Process(grayImg2);
}

int main(int argc, char** argv)
{
    // testFeatureMatching();
    testTrackingInitialize();
    return 0;
}