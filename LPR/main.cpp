/**
  **************************************************************
  * @file       main.cpp
  * @author     高明飞
  * @version    V0.3.1
  * @date       2016-01-04
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
#include "LPR.h"
#include <windows.h>

using namespace cv;
using namespace std;

/**
  * @brief  车牌图像根目录，最后不要加"\\"
  */
#define IMG_PATH (".\\Image")
/**
  * @brief  标准字符图像根目录，最后不要加"\\"
  */
#define STANDARD_PATH (".\\Standard")


/**
  * @brief  程序主入口
  *
  * @param  None
  *
  * @retval None
  */
int main()
{
  Mat img;
  string result;
  PlateImg ImgProvider = PlateImg(IMG_PATH);
  LPR myLPR = LPR();

  namedWindow("LPR", CV_WINDOW_NORMAL);

  myLPR.Standard(STANDARD_PATH, FeatureVec);
  
  for (int i = 0; i < ImgProvider.ImgNum; i++)
  {
    img = imread(ImgProvider.GetImgPath(i));
    result = myLPR.Identify(img, IdentifyNeighbor);

    cout << "==========================" << endl;
    cout << "LPR Result: "<< result << endl;
    cout << "==========================" << endl;
    imshow("LPR", img);
    waitKey();
  }


  system("pause");

  return 0;
}



