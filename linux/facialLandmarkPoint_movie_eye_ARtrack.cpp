#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
//#include "drawLandmarks.hpp"

using namespace std;
using namespace cv;
using namespace cv::aruco;

Mat Merging(Mat &Masked,Mat &Based_img,Mat &Matrix){
  Mat copied,BGRA,Export;
  Mat BGR(Based_img.size(),CV_8UC3);
  Mat Alpha(Based_img.size(),CV_8UC1);
  Mat splitchannels[] ={BGR,Alpha};
  int from_to[] = { 0,0,  1,1,  2,2,  3,3 };
  
  Masked.copyTo(copied);
  Based_img.copyTo(Export);
  warpAffine(copied,BGRA,Matrix,Based_img.size());
  mixChannels(&BGRA,1,splitchannels,2,from_to,4);
  BGR.copyTo(Export,Alpha);

  return Export;
}

int main(int argc,char** argv)
{
    // Set up webcam for video capture
    VideoCapture cam(0);
    //img read
    //string arg = argv[1];
    //Mat image = imread(arg,IMREAD_COLOR);

    // Variable to store a video frame and its grayscale 
    Mat image;

    int width = (int) cam.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int) cam.get(cv::CAP_PROP_FRAME_HEIGHT);

    Mat lena_base=imread("lena_std.bmp",IMREAD_COLOR);
    Mat lena_eye=imread("lena_eye.png",IMREAD_UNCHANGED);
    Mat lena_non_eyes=imread("lena_noneyes.png",IMREAD_UNCHANGED);

    Mat EyeLeft=(Mat_<double>(2,3)<<1,0,263,0,1,261);
    Mat EyeRight=(Mat_<double>(2,3)<<1,0,325,0,1,261);
    Mat Masking=(Mat_<double>(2,3)<<1,0,0,0,1,0);
    
    Mat Export;
    int key=0;
    int ud=0;
    int rl=0;

    
    // Read a frame
    while(cam.read(image))
      {
      // Display results 
      const cv::aruco::PREDEFINED_DICTIONARY_NAME dictionary_name = cv::aruco::DICT_4X4_50;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(dictionary_name);

    // マーカーの検出
    std::vector<int> marker_ids;
    std::vector<std::vector<cv::Point2f>> marker_corners;
    cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
    cv::aruco::detectMarkers(image, dictionary, marker_corners, marker_ids, parameters);

    for(int i=0;i<marker_corners.size();i++){
      for(int j=0;j<marker_corners[i].size();j++){
      cout << marker_corners[i][j] << endl;
      }
    }
    
    // 検出したマーカーの描画
    cv::aruco::drawDetectedMarkers(image, marker_corners, marker_ids);
    cv::imshow("marker_detection", image);

    if(marker_corners.size() > 0){
      rl = (((marker_corners[0][0].x + marker_corners[0][1].x)/2 /width)-0.5)*15;
      ud = (((marker_corners[0][0].y + marker_corners[0][2].y)/2 /height)-0.5)*15;
    }
    cout << rl << " " << ud << endl;
    
    EyeLeft=(Mat_<double>(2,3)<<1,0,263-rl,0,1,261+ud);
    EyeRight=(Mat_<double>(2,3)<<1,0,325-rl,0,1,261+ud);
    Export=Merging(lena_eye,lena_base,EyeLeft);
    Export=Merging(lena_eye,Export,EyeRight);
    Export=Merging(lena_non_eyes,Export,Masking);
    imshow("lena",Export);
    
      // Exit loop if ESC is pressed
      if (waitKey(1) == 27) break;
      }
    return 0;
}
