/**
  **************************************************************
  * @file       LRP.cpp
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

#include "LRP.h"

using namespace cv;
using namespace std;

LRP::LRP()
{
}

LRP::~LRP()
{
}

void LRP::test(Mat Img)
{
  binary(Img);
  imshow("LRP", Img);
  waitKey();
}


/**
  * @brief  二值化图像
  *
  * @param  Img: 待处理图像，使用引用进行传递
  *
  * @retval None
  */
void LRP::binary(cv::Mat & Img)
{
#ifdef PROGRESS
  imshow("LRP", Img);
  waitKey();
#endif

  // 中值滤波
  medianBlur(Img, Img, 7);
#ifdef PROGRESS
  imshow("LRP", Img);
  waitKey();
#endif

  // 提取B通道转为灰度图
  vector<Mat> mv;
  split(Img, mv);
  Img = mv[2];
#ifdef PROGRESS
  imshow("LRP", Img);
  waitKey();
#endif

  // 计算平均灰度值
  long ThreVal = 0;
  for (int i = 0; i < Img.rows; i++)
  {
    for (int j = 0; j < Img.cols; j++)
    {
      ThreVal += Img.at<uchar>(i, j);
    }
  }
  ThreVal = ThreVal / (Img.rows * Img.cols);

  // 二值化
  threshold(Img, Img, ThreVal, 255, THRESH_BINARY);
#ifdef PROGRESS
  imshow("LRP", Img);
  waitKey();
#endif

  // 进行形态学膨胀与腐蚀处理以去掉噪点
  erode(Img, Img, getStructuringElement(MORPH_RECT, Size(3, 3)));
  dilate(Img, Img, getStructuringElement(MORPH_RECT, Size(3, 3)));
  erode(Img, Img, getStructuringElement(MORPH_RECT, Size(2, 2)));

#ifdef PROGRESS
  imshow("LRP", Img);
  waitKey();
#endif
}
