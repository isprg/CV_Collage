#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
//#include "drawLandmarks.hpp"


using namespace std;
using namespace cv;
using namespace cv::face;


int main(int argc,char** argv)
{

  Mat lena_base=imread("lena_std.bmp",IMREAD_COLOR);
  Mat lena_eye=imread("lena_eye.png",IMREAD_UNCHANGED);
  Mat lena_non_eyes=imread("lena_noneyes.png",IMREAD_UNCHANGED);

  imshow("base",lena_base);

  Mat EyeLeft=(Mat_<double>(2,3)<<1,0,263,0,1,261);
  Mat EyeRight=(Mat_<double>(2,3)<<1,0,325,0,1,261);
  Mat lena_left_eye,lena_right_eye;
  lena_eye.copyTo(lena_left_eye);
  lena_eye.copyTo(lena_right_eye);

  
  Mat BGRA_left,BGRA_right;
  Mat Alpha_left(lena_base.size(),CV_8UC1);
  Mat BGR_left(lena_base.size(),CV_8UC3);
  Mat splitchannels_left[] ={BGR_left,Alpha_left};
  Mat Alpha_right(lena_base.size(),CV_8UC1);
  Mat BGR_right(lena_base.size(),CV_8UC3);
  Mat splitchannels_right[] ={BGR_right,Alpha_right};
  int from_to[] = { 0,0,  1,1,  2,2,  3,3 };
  Mat BGRA_Mask;
  Mat Alpha_Mask(lena_base.size(),CV_8UC1);
  Mat BGR_Mask(lena_base.size(),CV_8UC3);
  Mat splitchannels_Mask[] ={BGR_Mask,Alpha_Mask};

  BGRA_Mask=imread("lena_noneyes.png",IMREAD_UNCHANGED);
  imshow("lena_left_eye",lena_left_eye);
  warpAffine(lena_left_eye,BGRA_left,EyeLeft,lena_base.size());
  imshow("eye",BGRA_left);
  mixChannels(&BGRA_left,1,splitchannels_left,2,from_to,4);

  warpAffine(lena_right_eye,BGRA_right,EyeRight,lena_base.size());
  mixChannels(&BGRA_right,1,splitchannels_right,2,from_to,4);
  imshow("BGR",BGR_left);
  BGR_left.copyTo(lena_base,Alpha_left);
  BGR_right.copyTo(lena_base,Alpha_right);

  imshow("lena",lena_base);

  mixChannels(&BGRA_Mask,1,splitchannels_Mask,2,from_to,4);
  BGR_Mask.copyTo(lena_base,Alpha_Mask);
  imshow("lene_copy",lena_base);
  waitKey();
  /*
    // Load Face Detector
    CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");
    // Create an instance of Facemark
    Ptr<Facemark> facemark = FacemarkLBF::create();

    // Load landmark detector
    facemark->loadModel("lbfmodel.yaml");

    // Set up webcam for video capture
    //VideoCapture cam(0);
    //img read
    string arg = argv[1];
    Mat src = imread(arg,IMREAD_COLOR);
    
    // Variable to store a video frame and its grayscale 
    Mat frame, gray;
    
    // Read a frame
    //while(cam.read(frame))
    //{
      
      // Find face
      vector<Rect> faces;
      // Convert frame to grayscale because
      // faceDetector requires grayscale image.
      cvtColor(src, gray, COLOR_BGR2GRAY);

      // Detect faces
      faceDetector.detectMultiScale(gray, faces);
      
      // Variable for landmarks. 
      // Landmarks for one face is a vector of points
      // There can be more than one face in the image. Hence, we 
      // use a vector of vector of points. 
      vector< vector<Point2f> > landmarks;
      
      // Run landmark detector
      bool success = facemark->fit(src,faces,landmarks);
      
      if(success)
      {
        // If successful, render the landmarks on the face
        for(int i = 0; i < landmarks.size(); i++)
	  {
	  vector <Point2f> landmark = landmarks[i];
          //drawLandmarks(frame, landmarks[i]);
	  for(int j = 0; j < landmark.size(); j++)
	    {
	      circle(src,landmark[j],3, Scalar(255,200,0), FILLED);
	      cout <<"facenumber "  << j <<" x:"<< landmark[j].x << " y:"<< landmark[j].y<<endl;
	    }
	  }
      }
      // Display results 
      imshow("Facial Landmark Detection", src);
      // Exit loop if ESC is pressed
      //if (waitKey(1) == 27) break;
      waitKey(); 
      //}*/
    return 0;
}
