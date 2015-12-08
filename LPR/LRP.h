/**
  **************************************************************
  * @file       LRP.h
  * @author     高明飞
  * @version    V0.1
  * @date       2015-12-8
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
  * @brief  此类用于进行车牌识别
  */
class LRP
{
public:
  LRP();

  void binary(cv::Mat & Img);

  void test(cv::Mat Img);

  ~LRP();
};

