#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include "drawLandmarks.hpp"

using namespace std;
using namespace cv;

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
  int camnum=0;
  if(argc > 1)
    camnum = std::stoi(argv[1]);
    // Set up webcam for video capture
  VideoCapture cap(camnum);
    //cap.open(argv[1]);
    //img read
    //string arg = argv[1];
    //Mat image = imread(arg,IMREAD_COLOR);

    // Variable to store a video frame and its grayscale 
    Mat image;

    int width = (int) cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int) cap.get(cv::CAP_PROP_FRAME_HEIGHT);

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
    /*for(;;)
      {cap >> image;
	if(image.empty() == true){
	  cerr << "Err";
	  break;
	  }*/
    while(cap.read(image)){
      
	Mat input_image = image;//imread("sample.png", IMREAD_COLOR);
	Mat output_image = input_image.clone();
	vector<Point> points;
	Mat straight_qrcode;
	// QRコード検出器
	QRCodeDetector detector;

	string data = detector.detectAndDecode(input_image, points, straight_qrcode);
	cout << points.size() << endl;
	//cout << points << endl;
	
	if (data.length() > 0) {
	  //cout << points[0] << endl;
	Point QRcenter = (points[0] + points[1] + points[2] + points[3])/4;
	cout << QRcenter << endl;
	      if(data == "hoge"){
		//cout << "X:"<< QRcenter.x << "Percent:"<<float(QRcenter.x /width) << endl;
		float PercentRL = 1-(float(QRcenter.x) /width)-0.5;
		float PercentUD = (float(QRcenter.y) /height)-0.5;
		cout << PercentRL <<" "<< PercentUD << endl;
	rl = PercentRL*15;
	ud = PercentUD*15;
      }
	  // 復号化情報(文字列)の出力
	  cout << "decoded data: " << data << endl;
	  // 検出結果の矩形描画
	  for (size_t i = 0; i < points.size(); ++i) {
	    cv::line(output_image, points[i], points[(i + 1) % points.size()], cv::Scalar(0, 0, 255), 4);
	  }
	}
	else {
	  cout << "QR code not detected" << endl;
	}

      cout << rl << " " << ud << endl;
      
      EyeLeft=(Mat_<double>(2,3)<<1,0,263+rl,0,1,261+ud);
      EyeRight=(Mat_<double>(2,3)<<1,0,325+rl,0,1,261+ud);
      Export=Merging(lena_eye,lena_base,EyeLeft);
      Export=Merging(lena_eye,Export,EyeRight);
      Export=Merging(lena_non_eyes,Export,Masking);
      imshow("QR",output_image);
      imshow("lena",Export);
      moveWindow("lena",0,0);
      moveWindow("marker_detection",720,0);
      // Exit loop if ESC is pressed
      if (waitKey(1) == 27) break;
      }
    cout << width <<" "<< height << endl;
    return 0;
}
