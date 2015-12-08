/**
  **************************************************************
  * @file       PlateImg.h
  * @author     高明飞
  * @version    V0.1
  * @date       2015-12-8
  *
  * @brief      用于提供车牌图像的类
  *
  * @details 
  * @verbatim
  * 用于枚举文件夹中所有的车牌图像。
  *
  * 修改记录：
  * 2015-12-8 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#pragma once

#include <string>
#include <afx.h>
#include <afxstr.h>

using namespace std;

/**
  * @brief  用于提供车牌图像的类
  */
class PlateImg
{
public:
  PlateImg(string path);

  ~PlateImg();

private:
  int num;              /*!< 车牌图片数量 */
  CFileFind Finder;     /*!< CFileFind */

};

