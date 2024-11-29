/**
 * @FilePath     : /cola/cola/Vision/DimReduct/PCA/VPca.h
 * @Description  :
 * @Author       : naonao
 * @Date         : 2024-08-02 16:22:45
 * @Version      : 0.0.1
 * @LastEditors  : naonao
 * @LastEditTime : 2024-11-26 14:09:45
 * @Copyright (c) 2024 by G, All Rights Reserved.
 **/
#ifndef NAONAO_VPCA_H
#define NAONAO_VPCA_H

#include "../../VisionObject.h"

NAO_NAMESPACE_BEGIN
NAO_VISION_NAMESPACE_BEGIN

class VPCA : public VisionObject
{
public:
    VPCA()           = default;
    ~VPCA() override = default;

    /**
     * @brief: 降维
     * @param data
     * @param dimension
     * @param Retained
     * @return
     * @note :
     **/
    cv::Mat reduce(const cv::Mat& data, NInt dimension = 0, NDouble Retained = 0.9);

    /**
     * @brief: 将pca 数据写入文件
     * @param path
     * @return
     * @note :
     **/
    NVoid write(std::string path);

    /**
     * @brief: 将pca 数据从文件读取
     * @param path
     * @return
     * @note :
     **/
    NVoid read(std::string path);

    /**
     * @brief: 降维
     * @param data
     * @return
     * @note :
     **/
    cv::Mat reduce_single(const cv::Mat& data);

    NAO_NO_ALLOWED_COPY(VPCA)

public:
    cv::Mat eigenvalues_;
    cv::Mat eigenvectors_;
    cv::Mat eigenmean_;
    cv::Mat back_mat_;
    struct Eigenvector
    {
        NFloat  eigenvalue;
        cv::Mat eigenvector;
    };
    std::vector<Eigenvector> eigenvecWithVal_;
};

NAO_VISION_NAMESPACE_END
NAO_NAMESPACE_END

#endif   // NAONAO_VPCA_H