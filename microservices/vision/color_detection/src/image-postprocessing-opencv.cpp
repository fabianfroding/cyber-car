/**
 * Copyright (C) 2019 Carlos, the car
*/

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
using namespace std;
using namespace cv;
using namespace cluon;

//return the center of a contour -> Point(x,y)
Point getCenterOfContour(vector<Point> contour)
{
  Point center = minAreaRect(contour).center;
  return center;
}

//area
double getAreaOfContour(vector<Point> contour)
{
  return contourArea(contour);
}

//perimeter
double getPerimeterOfContour(vector<Point> contour)
{
  return arcLength(contour, true);
}
//draw the rectangle on the frame
void drawRectangle(Rect rectangle, Mat image, Scalar color)
{
  cv::rectangle(image, rectangle.tl(), rectangle.br(), color, 2, 8, 0);
}

float getPercentageOfWidth(vector<Point> contour, Mat image)
{
  float x, percentage; //y;
                       //  Point centerOfObject;
  x = getCenterOfContour(contour).x;
  //  y=getCenterOfContour(contour).y;
  //  centerOfObject=minAreaRect(contour).center;
  percentage = x / static_cast<float>(image.size().width); //percentage till the end of the frame

  return percentage;
}

//print the location of a detected color according to its name
void printRectangleLocation(vector<Point> contour, Mat image, String object)
{
  Moments moment = moments((Mat)contour);
  double area = moment.m00;
  float x, y, percentage = 0;
  Point centerOfObject;
  x = getCenterOfContour(contour).x;
  y = getCenterOfContour(contour).y;
  centerOfObject = minAreaRect(contour).center;      //center of the rectangle <Point>
  percentage = getPercentageOfWidth(contour, image); //percentage till the end of the frame
  if (object == "stop")
  {
    if (x <= (image.size().width) / 3) //center is on the left
      cout << "Detected STOP SIGN - LEFT |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << area << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
    else if (x >= (image.size().width) / 3 * 2) //center on the right
      cout << "Detected STOP SIGN - RIGHT |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << area << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
    else //center in the middle
      cout << "Detected STOP SIGN - CENTER |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << area << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
  }
  else if (object == "car")
  {
    if (x <= (image.size().width) / 3) //center is on the left
      cout << "Detected CAR - LEFT |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << getAreaOfContour(contour) << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
    else if (x >= (image.size().width) / 3 * 2) //center on the right
      cout << "Detected CAR - RIGHT |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << getAreaOfContour(contour) << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
    else //center in the middle
      cout << "Detected CAR - CENTER |" << x << "," << y << "|"
           << " %" << static_cast<int>(percentage * 100) << " - Area= " << getAreaOfContour(contour) << " - Perimeter= " << getPerimeterOfContour(contour) << flush << endl;
  }
}

//get contours of a detected range of colors in hsv image
vector<vector<Point>> getContours(Mat hsvImage, Scalar color_low, Scalar color_high)
{
  Mat blur, frame_threshold, detected_edges;
  vector<Vec4i> hierarchy;
  vector<vector<Point>> contours;
  medianBlur(hsvImage, blur, 11);
  inRange(blur, Scalar(color_low), Scalar(color_high), frame_threshold);
  Canny(frame_threshold, detected_edges, 0, 0, 5, true);
  findContours(detected_edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
  //cout<<"there are "<<hierarchy.size()<<" objects detected"<<endl<<flush;
  return contours;
}

int32_t main(int32_t argc, char **argv)
{ //**VARIABLES**//
  int32_t retCode{1};
  vector<vector<Point>> car_contours, car_polygons, stop_contours, stop_polygons;
  vector<Rect> car_rectangle, stop_rectangle;
  vector<Vec4i> car_hierarchy, stop_hierarchy;
  Rect temp, empty;
  Mat img, img_hsv, car_frame_threshold, car_detected_edges, blur;
  Mat stop_frame_threshold, stop_detected_edges;
  Scalar greenEdge = Scalar(0, 255, 0);
  Scalar redEdge = Scalar(0, 0, 255);
  //stop sign colors
  //  double area=0, perimeter=0;
  double stop_low_H = 130, stop_high_H = 160, stop_low_S = 87, stop_high_S = 255, stop_low_V = 69, stop_high_V = 255; //,sensitivity=0;
  //car sticker colors
  double car_low_H = 40, car_high_H = 80, car_low_S = 75, car_high_S = 255, car_low_V = 75, car_high_V = 255; //,sensitivity=0;
  Scalar car_low = Scalar(car_low_H, car_low_S, car_low_V), car_high = Scalar(car_high_H, car_high_S, car_high_V);
  Scalar stop_low = Scalar(stop_low_H, stop_low_S, stop_low_V), stop_high = Scalar(stop_high_H, stop_high_S, stop_high_V);
  //**END VARIABLES**//

  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ((0 == commandlineArguments.count("cid")) ||
      (0 == commandlineArguments.count("name")) ||
      (0 == commandlineArguments.count("width")) ||
      (0 == commandlineArguments.count("height")))
  {
    cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << endl;
    cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << endl;
    cerr << "         --cid:    CID of the OD4Session to send and receive messages" << endl;
    cerr << "         --name:   name of the shared memory area to attach" << endl;
    cerr << "         --width:  width of the frame" << endl;
    cerr << "         --height: height of the frame" << endl;
    cerr << "Example: " << argv[0] << " --cid=112 --name=img.i420 --width=640 --height=480" << endl;
  }
  else
  {
    const string NAME{commandlineArguments["name"]};
    const uint32_t WIDTH{static_cast<uint32_t>(stoi(commandlineArguments["width"]))};
    const uint32_t HEIGHT{static_cast<uint32_t>(stoi(commandlineArguments["height"]))};
    const bool VERBOSE{commandlineArguments.count("verbose") != 0};

    // Attach to the shared memory.
    unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
    if (sharedMemory && sharedMemory->valid())
    {
      clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << endl;
      // cout<<"hello!"<<flush;
      // Interface to a running OpenDaVINCI session; here, you can send and receive messages.
      cluon::OD4Session vision_color{static_cast<uint16_t>(stoi(commandlineArguments["cid"]))};
      carlos::vision::car tracker;
      // Endless loop; end the program by pressing Ctrl-C.
      while (vision_color.isRunning())
      {
        // Wait for a notification of a new frame.
        sharedMemory->wait();

        // Lock the shared memory.
        sharedMemory->lock();
        {
          // Copy image into cvMat structure.
          // Be aware of that any code between lock/unlock is blocking
          // the camera to provide the next frame. Thus, any
          // computationally heavy algorithms should be placed outside
          // lock/unlock.
          Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
          img = wrapped.clone();
          cvtColor(img, img_hsv, CV_BGR2HSV);
        }
        sharedMemory->unlock();

        //GaussianBlur(img_hsv,blur,Size(1,1),0,0,borderType);
        car_contours = getContours(img_hsv, car_low, car_high);
        stop_contours = getContours(img_hsv, stop_low, stop_high);
        car_polygons.resize(car_contours.size());
        car_rectangle.resize(car_contours.size());

        stop_polygons.resize(stop_contours.size());
        stop_rectangle.resize(stop_contours.size());

        //**PROCESS STOP SIGNAL DETECTION**
        int numberOfStops;
        for (size_t k = 0; k < stop_contours.size(); k++)
        {
          approxPolyDP(stop_contours[k], stop_polygons[k], 3, true);
          if (boundingRect(stop_polygons[k]).area() > 100 && arcLength(stop_contours[k], true) > 100)
          {
            stop_rectangle[k] = boundingRect(stop_polygons[k]);
            printRectangleLocation(stop_contours[k], img, "stop"); //coordinates and position of the center of each rectangle
            numberOfStops++;
          }
          groupRectangles(stop_rectangle, 1, 0.6); //group overlapping rectangles
          //cout<<"There are currently "<<stop_rectangle.size()<<" rectangles"<<endl<<flush;
          drawRectangle(stop_rectangle[k], img, redEdge);
        }
        numberOfStops = 0;
        //**PROCESS CAR GREEN DETECTION**
        //if(car_contours.size()!=0){
        int numberOfCars = 0;
        for (size_t k = 0; k < car_contours.size(); k++)
        {
          approxPolyDP(car_contours[k], car_polygons[k], 3, true); //approximate the curve of the polygon
          if (boundingRect(car_polygons[k]).area() > 100 && arcLength(car_contours[k], true) > 100)
          {
            car_rectangle[k] = boundingRect(car_polygons[k]);
            printRectangleLocation(car_contours[k], img, "car"); //coordinates and position of the center of each rectangle
            numberOfCars++;
          }
          groupRectangles(car_rectangle, 1, 0.6); //group overlapping rectangles
          cout << "There are currently " << numberOfRectangles << " cars detected" << endl
               << flush;
          drawRectangle(car_rectangle[k], img, greenEdge);

          //create the envelope containing this data
          tracker.coc(getPercentageOfWidth(car_contours[k], img)); //center of car
          tracker.area(car_rectangle[k].area());                   //area
          tracker.queue(-1);                                       //number of cars queued

          vision_color.send(tracker); //send the message
        }
        numberOfCars = 0;

        //message sending stopped
        // Display image.
        if (VERBOSE)
        {
          imshow(sharedMemory->name().c_str(), img);
          waitKey(1);
        }
      }
    }
    retCode = 0;
  }

  return retCode;
}
