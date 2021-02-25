// download the YOLO files from: https://pjreddie.com/darknet/yolo/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;
using namespace dnn;

class HumanDetector{
    public:
        // prepare model files and set up network
        std::string model = "yolov2-tiny.weights";
        std::string config = "yolov2-tiny.cfg";        
        Net network = readNet(model, config,"Darknet"); 
        int human_flag;
        int pet_flag;

        void configure_network(){
            network.setPreferableBackend(DNN_BACKEND_DEFAULT);
            network.setPreferableTarget(DNN_TARGET_OPENCL);
        }

        void detect(Mat img){
            static Mat blobFromImg;
            bool swapRB = true;
            // function which facilitates classification
            blobFromImage(img, blobFromImg, 1, Size(416, 416), Scalar(), swapRB, false);
            //cout << blobFromImg.size() << endl; 
            
            float scale = 1.0 / 255.0;
            Scalar mean = 0;
            network.setInput(blobFromImg, "", scale, mean);

            Mat outMat;
            network.forward(outMat);
            // number of detected objects
            int rowsNoOfDetection = outMat.rows;
            int colsCoordinatesPlusClassScore = outMat.cols;
            // Loop over number of detected objects
            for (int j = 0; j < rowsNoOfDetection; ++j)
            {
                // get classifier scores
                Mat scores = outMat.row(j).colRange(5, colsCoordinatesPlusClassScore);
                Point PositionOfMax;
                double confidence;

                minMaxLoc(scores, 0, &confidence, 0, &PositionOfMax);
                if (confidence > 0.0001)
                {
                    // locate object
                    stringstream ss;
                    ss << PositionOfMax.x;
                    string clas = ss.str();
                    if ((clas =="0") || (clas=="16")){
                        int centerX = (int)(outMat.at<float>(j, 0) * img.cols); 
                        int centerY = (int)(outMat.at<float>(j, 1) * img.rows); 
                        int width =   (int)(outMat.at<float>(j, 2) * img.cols+20); 
                        int height =   (int)(outMat.at<float>(j, 3) * img.rows+100); 
                        int left = centerX - width / 2;
                        int top = centerY - height / 2;
                        
                        if (clas =="0"){
                            putText(img, "Human Detected", Point(left, top), 1, 2, Scalar(0, 255, 255), 2, false);
                            human_flag = 1;
                        }
                        else if (clas =="16"){
                            putText(img, "Dog Detected", Point(left, top), 1, 2, Scalar(0, 255, 255), 2, false);
                            pet_flag = 1;
                        }
                        stringstream ss2;
                        ss << confidence;
                        string conf = ss.str();
                        // draw rectangle around object
                        rectangle(img, Rect(left, top, width, height), Scalar(0,0,255), 2, 8, 0);
                    }
                }
            }     
        }
};

class Camera{
  public:
      Mat img;
      HumanDetector mydetector;

      // start camera method
      int start(){
            // initialise capture
            mydetector.configure_network();
            VideoCapture cap(0);   
            namedWindow("Display window", WINDOW_NORMAL);
            while(1){
               cap >> img;
               if (!cap.isOpened()) {
               cout << "Video Capture Fail" << endl;
               break;
            }
               mydetector.detect(img);
               imshow("Display window", img);
               if (waitKey(10) == 27){       
                  break;
               }   
            } 
          return 0;
      }                   
};

int main()
{
    Camera mycamera;
    mycamera.start();
    return 0;
}