/**
 * @FilePath     : /cola/cola/Vision/Cluster/VFlann.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-12-12 15:50:52
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-12-12 16:19:43
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAONAO_VFlANN_H
#define NAONAO_VFlANN_H

#include "../VisionObject.h"

NAO_NAMESPACE_BEGIN
NAO_VISION_NAMESPACE_BEGIN

template<typename T = NFloat>
class VFlann : public VisionObject
{
public:
    VFlann(const std::vector<std::vector<T>>& feature)
    {
        size_t rows  = feature.size();
        size_t cols  = feature[0].size();
        feature_map_ = cv::Mat::zeros(rows, cols, CV_32F);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                feature_map_.at<float>(i, j) = feature[i][j];
            }
        }
    };

    /**
     * @brief
     * @param src
     * @return
     */

    NInt find_idx_v1(std::vector<T> refPoint)
    {
        size_t  cols  = refPoint.size();
        cv::Mat query = cv::Mat::zeros(1, cols, CV_32F);
        for (size_t j = 0; j < cols; ++j) {
            query.at<float>(0, j) = refPoint[j];
        }
        cv::flann::Index flannIndex(feature_map_, cv::flann::KDTreeIndexParams(2), cvflann::FLANN_DIST_L2);
        cv::Mat          indices, dists;
        flannIndex.knnSearch(query, indices, dists, 1, cv::flann::SearchParams(32));
        // NDouble minDist = std::sqrt(dists.at<float>(0, 0));
        return indices.at<int>(0, 0);
    };

    NDouble find_dists_v1(std::vector<T> refPoint)
    {
        size_t  cols  = refPoint.size();
        cv::Mat query = cv::Mat::zeros(1, cols, CV_32F);
        for (size_t j = 0; j < cols; ++j) {
            query.at<float>(0, j) = refPoint[j];
        }
        cv::flann::Index flannIndex(feature_map_, cv::flann::KDTreeIndexParams(2), cvflann::FLANN_DIST_L2);
        cv::Mat          indices, dists;
        flannIndex.knnSearch(query, indices, dists, 1, cv::flann::SearchParams(32));
        NDouble minDist = std::sqrt(dists.at<float>(0, 0));
        return minDist;
    };


    /**
     * @brief
     * @param src
     * @return
     */

    NInt find_idx_v2(std::vector<T> refPoint, NFloat sigma)
    {
        size_t  cols  = refPoint.size();
        cv::Mat query = cv::Mat::zeros(1, cols, CV_32F);
        for (size_t j = 0; j < cols; ++j) {
            query.at<float>(0, j) = refPoint[j];
        }

        cv::FlannBasedMatcher   matcher;
        std::vector<cv::DMatch> matches;
        matcher.match(query, feature_map_, matches);

        double minDist = 0, maxDist = 0;
        for (size_t i = 0; i < matches.size(); i++) {
            double dist = matches[i].distance;
            if (dist > maxDist) {
                maxDist = dist;
            }
            if (dist < minDist) {
                minDist = dist;
            }
        }

        std::vector<cv::DMatch> goodMatches;
        for (size_t i = 0; i < matches.size(); i++) {
            double dist = matches[i].distance;
            if (dist < std::max(3 * minDist, 0.02)) {
                goodMatches.push_back(matches[i]);
            }
        }
        return -1;
    };

    NDouble find_dists_v2(std::vector<T> refPoint)
    {
        size_t  cols  = refPoint.size();
        cv::Mat query = cv::Mat::zeros(1, cols, CV_32F);
        for (size_t j = 0; j < cols; ++j) {
            query.at<float>(0, j) = refPoint[j];
        }

        cv::FlannBasedMatcher   matcher;
        std::vector<cv::DMatch> matches;
        matcher.match(query, feature_map_, matches);

        double minDist = 0, maxDist = 0;
        for (size_t i = 0; i < matches.size(); i++) {
            double dist = matches[i].distance;
            if (dist > maxDist) {
                maxDist = dist;
            }
            if (dist < minDist) {
                minDist = dist;
            }
        }
        std::vector<cv::DMatch> goodMatches;
        for (size_t i = 0; i < matches.size(); i++) {
            double dist = matches[i].distance;
            if (dist < std::max(3 * minDist, 0.02)) {
                goodMatches.push_back(matches[i]);
            }
        }
        return minDist;
    };

    cv::Mat feature_map_;
};

NAO_VISION_NAMESPACE_END
NAO_NAMESPACE_END

#endif   // NAONAO_VFlANN_H