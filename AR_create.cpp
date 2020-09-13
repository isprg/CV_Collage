#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <string>

int main(){
  cv::Mat markerImage;
  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
  for(int i=0;i<9;i++){
    cv::aruco::drawMarker(dictionary, i, 64, markerImage, 1);
    std::string makername = "maker" + std::to_string(i) + ".png";
    std::cout << makername << std::endl;
    cv::imwrite(makername, markerImage);
  }

  return 0;
}
