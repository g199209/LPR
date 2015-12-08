/**
  **************************************************************
  * @file       PlateImg.cpp
  * @author     高明飞
  * @version    V0.1
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

#include "PlateImg.h"


/**
  * @brief  构造函数
  *
  * @param  path: 图片路径，路径下必须存在Index.txt索引文件
  *
  * @retval None
  */
PlateImg::PlateImg(string path)
{
  ImgPath = path;
  path += "\\Index.txt";
  ifstream f = ifstream(path);
  string s;
  while (f >> s)
  {
    ImgName.push_back(s);
    ImgNum++;
  }
}

/**
  * @brief  获取图像名称
  *
  * @param  index:  索引号
  *
  * @retval string: 图像名称（不含后缀及路径）
  */
string PlateImg::GetImgName(int index)
{
  if (index >= ImgNum)
    return "";

  return ImgName[index];
}

/**
* @brief  获取图像完整路径
*
* @param  index:  索引号
*
* @retval string: 图像完整路径，包含后缀，后缀固定为.png
*/
string PlateImg::GetImgPath(int index)
{
  if (index >= ImgNum)
    return "";

  return (ImgPath + "\\" + GetImgName(index) + ".png");
}
