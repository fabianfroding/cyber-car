/*
 * Copyright (C) 2019  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};

    Point centerOfRectangle;
    vector<vector<Point>> contours, polygons;
    vector<Rect> rectangle;
    vector<Vec4i> hierarchy;
    Rect temp;
    Mat img, img_hsv, frame_threshold, detected_edges, blur, drawing;
    Scalar color = Scalar( 255,255,255 );
    int borderType=BORDER_DEFAULT, maxAreaIndex=0;
    float x,y;
    double area=0;//,perimeter=0, maxArea=0;
    double stop_low_H=130, stop_high_H=166, stop_low_S=87, stop_high_S=255, stop_low_V=69, stop_high_V=255; //PURPLE STOP SIGN COLORS //,sensitivity=0;
    double car_low_H=40, car_high_H=80, car_low_S=70, car_high_S=255, car_low_V=65, car_high_V=255; //GREEN STICKERS COLOR //,sensitivity=0;


    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("cid")) ||
         (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << endl;
        cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << endl;
        cerr << "         --cid:    CID of the OD4Session to send and receive messages" << endl;
        cerr << "         --name:   name of the shared memory area to attach" << endl;
        cerr << "         --width:  width of the frame" << endl;
        cerr << "         --height: height of the frame" << endl;
        cerr << "Example: " << argv[0] << " --cid=112 --name=img.i420 --width=640 --height=480" << endl;
    }
    else {
        const string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};

        // Attach to the shared memory.
        unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << endl;
	   // cout<<"hello!"<<flush;
            // Interface to a running OpenDaVINCI session; here, you can send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(stoi(commandlineArguments["cid"]))};

            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning()) {
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
		                cvtColor(img,img_hsv, CV_BGR2HSV);

                }
                sharedMemory->unlock();

                // TODO: Do something with the frame.
            		GaussianBlur(img_hsv,blur,Size(1,1),0,0,borderType);
            		inRange(blur, Scalar(car_low_H, car_low_S, car_low_V), Scalar(car_high_H, car_high_S, car_high_V), frame_threshold);
            		Canny (frame_threshold, detected_edges, 0, 0, 5,true);
            		findContours(detected_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

            		polygons.resize(contours.size());
            		rectangle.resize(contours.size());

            		//approximate the curve of the polygon
            		for(size_t k = 0; k < contours.size(); k++){
                  approxPolyDP(contours[k], polygons[k], 3, true);
                  rectangle[k]=boundingRect(polygons[k]);
                  x=minAreaRect(contours[k]).center.x;
                  y=minAreaRect(contours[k]).center.y;
                  centerOfRectangle=minAreaRect(contours[k]).center;
                  if(x<=(img.size().width)/3)
                    cout<<"The rectangle is on the left and the center is "<<x<<","<<y<<flush<<endl;
                  else if (x >= (img.size().width)/3*2)
                    cout<<"The rectangle is on the right and the center is "<<x<<","<<y<<flush<<endl;
                  else cout<<"The rectangle is in front of you and the center is "<<x<<","<<y<<flush<<endl;
                  groupRectangles(rectangle,3,0.7);

                  cv::rectangle(img, rectangle[k].tl(), rectangle[k].br(), color, 1, 8, 0);
                }
                //group overlapping rectangles


                //draw the rectangles
              /*  for (size_t i=0; i<rectangle.size(); i++){
                  area=rectangle[i].area();
                  cout<< "DETECTED! Area= " <<area<< perimeter << endl << flush;
                  cv::rectangle(img, rectangle[i].tl(), rectangle[i].br(), color, 1, 8, 0);
                }*/
            		// Display image.
                if (VERBOSE) {
                    imshow(sharedMemory->name().c_str(), img);
                    waitKey(1);
                }
            }
        }
        retCode = 0;
    }
    return retCode;
}
