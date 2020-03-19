#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
//#include "drawLandmarks.hpp"


using namespace std;
using namespace cv;
using namespace cv::face;
//0==face 1==mouth

void Resize(Mat &img,Mat &src,vector <Point2f> mark,int parts){
  int xx,yy;
  float width,height;
  Mat Affine;
  Mat BGRA;
  Mat Alpha(src.size(),CV_8UC1);
  Mat BGR(src.size(),CV_8UC3);
  Mat splitchannels[] ={BGR,Alpha};
  int from_to[] = { 0,0,  1,1,  2,2,  3,3 };
  //vector<Mat> BGRA;
    
  cout << "Running" << endl;
  switch(parts){
  case 0:
    xx = mark[16].x -mark[2].x;
    yy = mark[8].y - mark[33].y;
    width = img.rows;
    height = img.cols;
    cout << xx << "  "<< yy <<"\n"<< width <<" "<< height <<"\n" <<  xx/width<<yy/height<<endl;
    //resize(img,img,Size(),0.5,0.5);//width*(xx/240),height*(yy/120));
    
    //cout << mark[16].x-mark[0].x <<" "<<mark[8].y-mark[33].y << "\n" <<0.6*(mark[16].x-mark[0].x) << " "<< 1.9*(mark[8].y-mark[33].y) << endl;

    //Affine =(Mat_<double>(2,3)<<(double)xx/300, 0.0,(double)0.2*(mark[16].x-mark[0].x), 0.0, (double)yy/120,(double)0.2*(mark[8].y-mark[33].y));
    Affine =(Mat_<double>(2,3)<<(double)xx/300, 0.0,mark[2].x*0.5, 0.0, (double)yy/120,yy);
    
    //warpAffine(img,Alpha,Affine,src.size(),CV_INTER_LINEAR,BORDER_TRANSPARENT);
    warpAffine(img,BGRA,Affine,src.size());
    imshow("kwsm",BGRA);
    
    //img.copyTo(src);
    //warpAffine(img,src,Affine,src.size(),CV_INTER_LINEAR);//,cv::BORDER_TRANSPARENT);
    //imshow("reslut",src);
    
    //split(Alpha,BGRA);
    mixChannels(&BGRA,1,splitchannels,2,from_to,4);
    //imshow("Alpha",Alpha);
    //imshow("src",src);
    waitKey();
    
    /*merge(BGRA[0],ORG);
    merge(BGRA[1],ORG);
    merge(BGRA[2],ORG);*/
    //imshow("DUM",BGR);
    BGR.copyTo(src,Alpha);
    //cvCopy(Alpha,src,BGRA[3]);
    imshow("copy",src);
    waitKey();
    break;
    
  case 1:
    flip(img,img,0);
    cout << "err" << endl;
    break;
  }
}

//void Paste(Mat &BaseImg,Mat &PasteImage,Point2f mapping){
  
int main(int argc,char** argv)
{
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
    Mat kwsm_face_only = imread("test_mat.png",IMREAD_UNCHANGED);
    //Mat kwsm_face_mouth = imread("kwsm_face_mouth.png",IMREAD_UNCHAGED);
    // Variable to store a video frame and its grayscale 
    Mat frame, gray,img_dst;
    
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

      int parts=0;
      // Run landmark detector
      bool success = facemark->fit(src,faces,landmarks);
      
      if(success)
      {
        // If successful, render the landmarks on the face
        for(int i = 0; i < landmarks.size(); i++)
	  {
	  vector <Point2f> landmark = landmarks[i];
	  cout << landmark[33].x-landmark[39].x << " " << landmark[42].x-landmark[33].x<< endl;

	  cout << landmark[14].x-landmark[8].x<< endl;

	  if(landmark[33].x-landmark[39].x > landmark[42].x-landmark[33].x){
	    flip(kwsm_face_only,kwsm_face_only,1);

	    cout << "fliping" << endl;
	  }

	  cout << kwsm_face_only.rows << endl;
	  cout << "240 "<< landmark[14].x-landmark[8].x << "\n120 " << landmark[8].y-landmark[33].y << endl;
	  
	  //imshow("Out",kwsm_face_only);

	  waitKey();
	  
	  Resize(kwsm_face_only,src,landmark,0);

	  for(int j = 0; j < landmark.size(); j++)
	    {
	      if(j != 33 && j != 39 && j != 42)
		circle(src,landmark[j],3, Scalar(255,200,0), FILLED);
	      else
		circle(src,landmark[j],3, Scalar(255,200,255), FILLED);
	      cout <<"facenumber "  << j <<" x:"<< landmark[j].x << " y:"<< landmark[j].y<<endl;
	      }
	  
	  }
      }
      
      waitKey();
      // Display results 
      imshow("Facial Landmark Detection", src);
      // Exit loop if ESC is pressed
      //if (waitKey(1) == 27) break;
      waitKey(); 
      //}
    return 0;
}
