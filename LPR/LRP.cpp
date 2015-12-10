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

#define PROGRESS

  binary(Img);

  vector<Mat> characters;

  Extract(Img, characters);
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

/**
  * @brief  递归的提取二值化图像中的每个字符
  *
  * @param  Img: 待处理的二值化图像
  * @param  vec: 字符图像vector，结果储存于其中
  *
  * @retval 字符图像vector
  *
  * @note
  * 现在这种写法效率不高，而且程序也不简洁……
  */
void LRP::Extract(cv::Mat & Img, std::vector<cv::Mat> vec)
{
  /* 字符边界 */
  int Left = 0, Right = Img.cols, Top = 0, Bottom = Img.rows;
  /* 像素累加变量 */
  int sum = 0;
  /* 计数变量，用于VerifyStart过程中 */
  int count = 0;
  const uchar CountMax = 3;

  /* 当前执行状态 */
  enum{ FindStart, VerifyStart, FindEnd, Exit } flag_r = FindStart, flag_c = FindStart;
  
  /* 逐列扫描，确定左右边界 */
  for (int j = 0; j < Img.cols; j++)
  {
    sum = 0;

    if (flag_c == Exit)
      break;

    /* 累加所有行 */
    for (int i = 0; i < Img.rows; i++)
    {
      sum += Img.at<uchar>(i, j);
    }

    switch (flag_c)
    {
      /* 起始列判断条件：有一个以上白色像素点 */
    case FindStart:
      if (sum > 255)
      {
        flag_c = VerifyStart;
        Left = j;
        count = 0;
      }
      break;

      /* 起始列后要有至少CountMax个有一个以上白色像素点的列，用来消除噪点 */
    case VerifyStart:
      if (count > CountMax)
        flag_c = FindEnd;
      else if (sum > 255)
        count++;
      else
        flag_c = FindStart;
      break;

      /* 结束列判断条件：仅有一个以下白色像素点 */
    case FindEnd:
      if (sum <= 255)
      {
        Right = j;
        flag_c = Exit;
      }
      break;
    }
  }

  /* 逐行扫描，确定上下边界 */
  for (int i = 0; i < Img.rows; i++)
  {
    sum = 0;

    if (flag_r == Exit)
      break;

    /* 累加Left~Right之间的列 */
    /* 注意：此处不要累加所有列，否则在图像有倾斜的时候结果是错误的 */
    for (int j = Left; j < Right; j++)
    {
      sum += Img.at<uchar>(i, j);
    }

    switch (flag_r)
    {
      /* 起始行判断条件：有一个以上白色像素点 */
    case FindStart:
      if (sum > 255)
      {
        flag_r = VerifyStart;
        Top = i;
        count = 0;
      }
      break;

      /* 起始行后要有至少CountMax个有一个以上白色像素点的行，用来消除噪点 */
    case VerifyStart:
      if (count > CountMax)
        flag_r = FindEnd;
      else if (sum > 255)
        count++;
      else
        flag_r = FindStart;
      break;

      /* 结束行判断条件：仅有一个以下白色像素点 */
    case FindEnd:
      if (sum <= 255)
      {
        Bottom = i;
        flag_r = Exit;
      }
      break;
    }
  }

  /* 找到有效字符 */
  if (flag_c == Exit || flag_c == FindEnd)
  {
    vec.push_back(Img(Range(Top, Bottom), Range(Left, Right)));
    imshow("LRP", Img(Range(Top, Bottom), Range(Left, Right)));
    waitKey();
  }

  /* 当且仅当flag_c == Exit时才可能会有更多的字符 */
  if (flag_c != Exit)
    return;
  else
    /* 递归。注意：不要裁剪行，仅仅裁剪列 */
    return Extract(Img(Range(0, Img.rows), Range(Right, Img.cols)), vec);
}

