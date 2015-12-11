/**
  **************************************************************
  * @file       LRP.cpp
  * @author     高明飞
  * @version    V0.3
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

#include "LRP.h"
#include "PlateImg.h"

using namespace cv;
using namespace std;

LRP::LRP()
{
}

LRP::~LRP()
{
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
  // 中值滤波
  medianBlur(Img, Img, 7);

  // 提取B通道转为灰度图
  vector<Mat> mv;
  split(Img, mv);
  Img = mv[2];

  // 计算平均灰度值
  uchar ThreVal = (uchar)cv::mean(Img).val[0];

  // 二值化
  threshold(Img, Img, ThreVal, 255, THRESH_BINARY);

  // 进行形态学膨胀与腐蚀处理以去掉噪点
  erode(Img, Img, getStructuringElement(MORPH_RECT, Size(3, 3)));
  dilate(Img, Img, getStructuringElement(MORPH_RECT, Size(3, 3)));
  erode(Img, Img, getStructuringElement(MORPH_RECT, Size(2, 2)));
}

/**
  * @brief  递归的提取二值化图像中的每个字符
  *
  * @param  Img: 待处理的二值化图像
  * @param  vec: 归一化后的字符图像vector，结果储存于其中
  *
  * @retval None
  *
  * @note
  * 现在这种写法效率不高，而且程序也不简洁……
  */
void LRP::Extract(cv::Mat & Img, std::vector<cv::Mat> & vec)
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
    /* 对字符图片大小进行归一化处理 */
    Mat tmp;
    resize(Img(Range(Top, Bottom), Range(Left, Right)), tmp, Size(CharImgWidth, CharImgHeight));

    /* 归一化后，需要重新进行二值化 */
    // 计算平均灰度值
    // 计算平均灰度值
    uchar ThreVal = (uchar)cv::mean(Img).val[0];
    // 二值化
    threshold(tmp, tmp, ThreVal, 255, THRESH_BINARY);

    vec.push_back(tmp);
  }

  /* 当且仅当flag_c == Exit时才可能会有更多的字符 */
  if (flag_c != Exit)
    return;
  else
    /* 递归。注意：不要裁剪行，仅仅裁剪列 */
    return Extract(Img(Range(0, Img.rows), Range(Right, Img.cols)), vec);
}

/**
  * @brief  提取特征向量
  *
  * @param  Img: 之前分割好的归一化后的字符图片
  *
  * @retval cv::Mat 提取到的特征向量
  *
  */
cv::Mat LRP::Feature(cv::Mat Img)
{
  Mat w;
  switch (CurrentFeatureMethod)
  {
    /* SVD */
  case FeatureSVD:
    Img.convertTo(Img, CV_32F, 1 / 255.0);
    SVD::compute(Img, w);
    return w;

  case FeatureVec:
    w = Mat(Img.rows * Img.cols, 1, CV_32F);
    for (int i = 0; i < Img.rows; i++)
    {
      for (int j = 0; j < Img.cols; j++)
      {
        w.at<float>(i*Img.cols + j, 0) = (float)Img.at<uchar>(i, j);
      }
    }
    return w;
  }
}

/**
  * @brief  建立标准样本特征向量
  *
  * @param  Path: 标准图片路径
  * @param  method: 提取特征向量使用的方法
  *   @arg  FeatureSVD: SVD
  *   @arg  FeatureVec: 直接拉直为向量
  *
  * @retval None
  */
void LRP::Standard(std::string Path, FeatureMethod method)
{
  CurrentFeatureMethod = method;
  PlateImg ImgProvider = PlateImg(Path);

  vector<Mat> characters;

  #pragma omp parallel for 
  for (int i = 0; i < ImgProvider.ImgNum; i++)
  {
    cout << "Computing  " << ImgProvider.GetImgName(i) << endl;

    Mat img = imread(ImgProvider.GetImgPath(i));

    binary(img);
    characters.clear();
    Extract(img, characters);
    Mat feature = Feature(characters[0]);

    #pragma omp critical
    {
      StdName.push_back(ImgProvider.GetImgName(i));
      StdFeature.push_back(feature);
    }
  }
}

/**
  * @brief  识别一个字符
  *
  * @param  Img: 之前分割好的归一化后的字符图片
  *
  * @retval std::string 识别结果
  *
  */
std::string LRP::IdentifyChar(cv::Mat Img)
{
  Mat feature = Feature(Img);

  switch (CurrentIdentifyMethod)
  {
    /* 近邻法（欧氏距离） */
  case IdentifyNeighbor:
    double minDis = norm(feature, StdFeature[0], CV_L2), Dis;
    int minIndex = 0;

    #ifdef _DEBUG
    cout << "--------------------------" << endl;
    cout << StdName[minIndex] << " : " << minDis << endl;
    #endif

    for (uint i = 1; i < StdFeature.size(); i++)
    {
      Dis = norm(feature, StdFeature[i], CV_L2);

      if (Dis < minDis)
      {
        minDis = Dis;
        minIndex = i;
      }

      #ifdef _DEBUG
      cout << StdName[i] << " : " << Dis << endl;
      #endif
    }

    #ifdef _DEBUG
    cout << "Neighbor : "<< StdName[minIndex] << " = " << minDis << endl;
    cout << "--------------------------" << endl;
    imshow("LRP", Img);
    waitKey();
    #endif
    return StdName[minIndex];

  }
}


/**
  * @brief  识别一副车牌图像
  *
  * @param  Img:    待识别的车牌图像
  * @param  method: 模式识别使用的方法
  *   @arg  IdentifyNeighbor:     近邻法（欧氏距离）
  *
  * @retval std::string
  */
std::string LRP::Identify(cv::Mat Img, IdentifyMethod method)
{
  CurrentIdentifyMethod = method;

  vector<Mat> characters;
  vector<Mat>::iterator IteImg;

  #ifdef _DEBUG
  imshow("LRP", Img);
  waitKey();
  #endif

  binary(Img);

  #ifdef _DEBUG
  imshow("LRP", Img);
  waitKey();
  #endif

  Extract(Img, characters);

  string result = "";
  for (IteImg = characters.begin(); IteImg != characters.end(); IteImg++)
  {
    result += IdentifyChar(*IteImg);
  }

  return result;
}

