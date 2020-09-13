#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <time.h>
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
    //VideoCapture cap;
    //cap.open(argv[1]);
    //img read
    string arg = argv[1];
    Mat image = imread(arg,IMREAD_COLOR);

    // Variable to store a video frame and its grayscale 
    //Mat image;

    //int width = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    //int height = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int width = (int) image.rows;
    int height = (int) image.cols;

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
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_COARSE,&ts);
  int LOOK = 0;
  int TIME = ts.tv_sec;
  std::vector<int> LK_IDs = {5,8,0,8,0,6,0,7,1,7,0,7,2,8,0,7,10};
  
    // Read a frame
    for(;;)
      {
        if(TIME + 5 == ts.tv_sec){
	  LOOK +=1;
	  TIME = ts.tv_sec;
	}
       	clock_gettime(CLOCK_MONOTONIC_COARSE,&ts);
	if(image.empty() == true)
	  break;


	
	
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
      //cv::aruco::drawDetectedMarkers(image, marker_corners, marker_ids);
      cv::imshow("marker_detection", image);



      std::vector<int> RDV(marker_ids.size(),0);
      for(int i=0;i<marker_ids.size();i++){
	for(int j=0;marker_ids.size();j++){
	  if(i == marker_ids[j]){
	    RDV[i] = j;
	    break;
	  }
	}
      }
      for(int i=0;i<marker_ids.size();i++)
	cout << marker_ids[i] << " " << RDV[i] << endl;


	      
      if(marker_corners.size() > 0){
	rl = (((marker_corners[RDV[LK_IDs[LOOK]]][0].x + marker_corners[RDV[LK_IDs[LOOK]]][1].x)/2 /width)-0.5)*15;
	ud = (((marker_corners[RDV[LK_IDs[LOOK]]][0].y + marker_corners[RDV[LK_IDs[LOOK]]][2].y)/2 /height)-0.5)*15;
      }
      cout << rl << " " << ud << endl;
      cout <<"time:" <<  ts.tv_sec << endl;
      
      cout << "LOOKING:" << LOOK << endl;
      cout << "ID:" << LK_IDs[LOOK]+1 << endl;
      
      EyeLeft=(Mat_<double>(2,3)<<1,0,263+rl,0,1,261+ud);
      EyeRight=(Mat_<double>(2,3)<<1,0,325+rl,0,1,261+ud);
      Export=Merging(lena_eye,lena_base,EyeLeft);
      Export=Merging(lena_eye,Export,EyeRight);
      Export=Merging(lena_non_eyes,Export,Masking);
      imshow("lena",Export);
      moveWindow("lena",0,0);
      moveWindow("marker_detection",720,0);
      // Exit loop if ESC is pressed
      if (waitKey(1) == 27 || LK_IDs[LOOK] == 10) break;
      }
    return 0;
      }
