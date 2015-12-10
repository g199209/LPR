/**
  **************************************************************
  * @file       main.cpp
  * @author     高明飞
  * @version    V0.1
  * @date       2015-12-8
  *
  * @brief      车牌模式识别主程序入口
  *
  * @details 
  * @verbatim
  * 主程序入口
  *
  * 修改记录：
  * 2015-12-8 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "PlateImg.h"
#include "LRP.h"
#include <windows.h>

using namespace cv;
using namespace std;

/**
  * @brief  车牌图像根目录，最后不要加"\\"
  */
#define IMG_PATH ("D:\\LPR\\Standard")
#define IMG_PATH ("D:\\LPR\\Image")

int main()
{
  PlateImg ImgProvider = PlateImg(IMG_PATH);
  LRP myLRP = LRP();

  namedWindow("LRP", CV_WINDOW_NORMAL);

  Mat img;
  for (int i = 0; i < ImgProvider.ImgNum; i++)
  {
    img = imread(ImgProvider.GetImgPath(i));
    myLRP.test(img);
  }

  //system("pause");

  return 0;
}



