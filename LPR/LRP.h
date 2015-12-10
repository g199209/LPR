/**
  **************************************************************
  * @file       LRP.h
  * @author     高明飞
  * @version    V0.2
  * @date       2015-12-11
  *
  * @brief      车牌识别核心算法部分
  *
  * @details 
  * @verbatim
  * 车牌识别核心算法部分
  *
  * 修改记录：
  * 2015-12-8 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
#include <vector>

/**
  * @brief  字符图片归一化后的图像宽度
  */
#define CharImgWidth 60
/**
  * @brief  字符图片归一化后的图像高度
  */
#define CharImgHeight 100

/**
  * @brief  提取特征向量可以使用的方法
  */
typedef enum
{
  FeatureSVD          /*!< SVD方法 */
} FeatureMethod;

/**
  * @brief  进行模式识别可以使用的方法
  */
typedef enum
{
  IdentifyNeighbor    /*!< 近邻法 */
} IdentifyMethod;

/**
  * @brief  此类用于进行车牌识别
  */
class LRP
{
public:
  LRP();

  void Standard(std::string Path, FeatureMethod method);
  std::string Identify(cv::Mat Img, IdentifyMethod method);

  void binary(cv::Mat & Img);
  void Extract(cv::Mat & Img, std::vector<cv::Mat> & vec);
  cv::Mat Feature(cv::Mat Img);
  std::string IdentifyChar(cv::Mat Img);

  ~LRP();
private:
  std::vector<std::string> StdName;     /*!< 标准样本名称 */
  std::vector<cv::Mat> StdFeature;      /*!< 标准样本特征向量 */
  FeatureMethod CurrentFeatureMethod;   /*!< 当前使用的特征向量提取方法 */
  IdentifyMethod CurrentIdentifyMethod; /*!< 当前使用的模式识别方法 */
};

