/**
  **************************************************************
  * @file       main.cpp
  * @author     高明飞
  * @version    V0.2
  * @date       2015-12-11
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
#define IMG_PATH ("D:\\LPR\\Image")
/**
  * @brief  标准字符图像根目录，最后不要加"\\"
  */
#define STANDARD_PATH ("D:\\LPR\\Standard")


/**
  * @brief  程序主入口
  *
  * @param  None
  *
  * @retval None
  */
int main()
{
  PlateImg ImgProvider = PlateImg(IMG_PATH);
  LRP myLRP = LRP();

  namedWindow("LRP", CV_WINDOW_NORMAL);

  myLRP.Standard(STANDARD_PATH, FeatureSVD);

  Mat img;
  string result;
  for (int i = 0; i < ImgProvider.ImgNum; i++)
  {
    img = imread(ImgProvider.GetImgPath(i));
    result = myLRP.Identify(img, IdentifyNeighbor);
    cout << result << endl;

    imshow("LRP", img);
    waitKey();
  }

  system("pause");

  return 0;
}



