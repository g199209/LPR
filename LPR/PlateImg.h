/**
  **************************************************************
  * @file       PlateImg.h
  * @author     高明飞
  * @version    V1.0
  * @date       2015-12-8
  *
  * @brief      用于提供车牌图像名称及路径的类
  *
  * @details 
  * @verbatim
  * 根据索引文件枚举文件夹中所有的车牌图像。
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
#include <fstream>
#include <vector>

using namespace std;

/**
  * @brief  用于提供车牌图像名称及路径的类
  */
class PlateImg
{
public:
  PlateImg(string path);

  string GetImgName(int index);

  string GetImgPath(int index);

  int ImgNum = 0;           /*!< 图像总数 */

private:
  string ImgPath;           /*!< 图像根目录 */

  vector <string> ImgName;  /*!< 图像名称 */
};

