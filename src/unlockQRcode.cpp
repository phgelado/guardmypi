#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int Unlock::QR(Mat frame) {

  //QRcode detection 
  std::string data = qrDecoder.detectAndDecode(frame, bbox, rectifiedImage);
  if(data.length()>0)
  {
    if(data=="user1234"){
        cout << "Valid unlock key detected, decoded data: " << data << endl;
        //display(frame, bbox);
        //rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
        //imshow("Rectified QRCode", rectifiedImage);

        //waitKey(0);

        //Call unlock function/change flags for unlock
    }
    else {
    
    cout << "Invalid unlock key detected, decoded data: " << data << endl;
    //display(frame, bbox);
    //rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
    //imshow("Rectified QRCode", rectifiedImage);

    //waitKey(0);}

    //Call unlock function/change flags

  }
  else
    cout << "QR Code not detected" << endl;
}


//display function
void display(Mat &im, Mat &bbox)
{
  int n = bbox.rows;
  for(int i = 0 ; i < n ; i++)
  {
    line(im, Point2i(bbox.at<float>(i,0),bbox.at<float>(i,1)), Point2i(bbox.at<float>((i+1) % n,0), bbox.at<float>((i+1) % n,1)), Scalar(255,0,0), 3);
  }
  imshow("Result", im);
}

int main(int argc, char* argv[])
{
  // Read image
  Mat inputImage;
  if(argc>1)
    inputImage = imread(argv[1]);
  else
  //error message if camera not loaded
    cout << "No input camera object correctly loaded" << endl;
    //inputImage = imread("qrcode-learnopencv.jpg"); <- can also load images like this

//QR CodeDetector obje#include <opencv2/imgcodecs.hpp>
  

//QRcode detection 
  std::string data = qrDecoder.detectAndDecode(inputImage, bbox, rectifiedImage);
  if(data.length()>0)
  {
    if(data=="user1234"){
        cout << "Valid unlock key detected, decoded data: " << data << endl;
        display(inputImage, bbox);
        rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
        imshow("Rectified QRCode", rectifiedImage);

        waitKey(0);
    }
    else {
    
    cout << "Invalid unlock key detected, decoded data: " << data << endl;
    display(inputImage, bbox);
    rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
    imshow("Rectified QRCode", rectifiedImage);

    waitKey(0);}

  }
  else
    cout << "QR Code not detected" << endl;
}