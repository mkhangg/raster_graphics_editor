/***********************************************************************************************************************
* @file: gui.cpp
* @brief: graphic user interface for the raster graphic editor
* @author: khang nguyen
***********************************************************************************************************************/

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

#define NUM_COMNMAND_LINE_ARGUMENTS 1

Mat3b canvas;
Mat inputImage, cp;
static Mat originalImage;

string buttonText1("Eyedropper");
string buttonText2("Crop");
string buttonText3("Pencil");
string buttonText4("Paint Bucket");
string windowName = "Raster Graphics Editor";

Rect button1;
Rect button2;
Rect button3;
Rect button4;

// Init white values
static int blue = 255;
static int green = 255;
static int red = 255;

static void eyeDropperCallback(int event, int x, int y, int flags, void* param) {
    Mat imageIn = *(Mat *)param;

    int size = 300;
    static int flag = 0;

    if(flag == 0){
        Mat colorImg(size, size, CV_8UC3, Scalar(blue, green, red));
        imshow("colorImg", colorImg);
        flag = 1;
    }

    if(event == EVENT_LBUTTONDOWN) {        
        Vec3b pixel = imageIn.at<Vec3b>(y, x);
        cout << "Pixel values = " << pixel << endl;

        blue = pixel[0];
        green = pixel[1];
        red = pixel[2];

        Mat colorImg(size, size, CV_8UC3, Scalar(blue, green, red));
        imshow("colorImg", colorImg);

        waitKey();
    }
}

static void cropImageCallback(int event, int x, int y, int flags, void* param) {
    Mat imageIn = *(Mat *)param;
    static Mat croppedImage, cp;

    static int x1 = 0, y1 = 0;
    static int x2 = 0, y2 = 0;
    static int drag = 0;

    if(event == EVENT_LBUTTONDBLCLK) {        
        imshow("inputImage", originalImage);
        setMouseCallback("inputImage", cropImageCallback, &originalImage);
    } else if(event == EVENT_LBUTTONDOWN) {   
        x1 = x;
        y1 = y;
        drag = 1;
    } else if(event == EVENT_LBUTTONUP) {
        x2 = x;
        y2 = y; 

        if (drag == 1 && x2 != x1 && y2 != y1){
            drag = 0;
            Point p1(x1, y1);
            Point p2(x2, y2);
            Rect region(p1, p2);

            cp = imageIn.clone();
            croppedImage = cp(region);
            imshow("inputImage", croppedImage);
            setMouseCallback("inputImage", cropImageCallback, &croppedImage);
        }
    }
}

static void pencilCallback(int event, int x, int y, int flags, void* param) {
    Mat imageIn = *(Mat *)param;

    static int drag = 0;

    if(event == EVENT_LBUTTONDBLCLK) {        
        imshow("inputImage", originalImage);
        setMouseCallback("inputImage", cropImageCallback, &originalImage);
    } 

    if(event == EVENT_LBUTTONDOWN) {        
        drag = 1;
    }

    if(drag == 1 && event == EVENT_MOUSEMOVE) {
        imageIn.at<Vec3b>(y, x)[0]= blue;
        imageIn.at<Vec3b>(y, x)[1]= green;
        imageIn.at<Vec3b>(y, x)[2]= red;

        imshow("inputImage", imageIn);
    }

    if(event == EVENT_LBUTTONUP) {
        drag = 0;
    }
}

static void paintBucketCallback(int event, int x, int y, int flags, void* param) {
    Mat imageIn = *(Mat *)param;

    if(event == EVENT_LBUTTONDBLCLK) {        
        imshow("inputImage", originalImage);
        setMouseCallback("inputImage", paintBucketCallback, &originalImage);
    } else if(event == EVENT_LBUTTONDOWN) {        
        // cp = cp.clone(); 
        int filling = floodFill(imageIn, Point(x, y), Scalar(blue, green, red), (Rect*)0, Scalar(), 200);
        imshow("inputImage", imageIn);
    }
}

void callBackFuncGui(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        if (button1.contains(Point(x, y))) {
            cout << "Eyedropper Tool chosen." << endl;
            setMouseCallback("inputImage", eyeDropperCallback, &inputImage);
        }

        if (button2.contains(Point(x, y))) {
            cout << "Crop Tool chosen." << endl;
            setMouseCallback("inputImage", cropImageCallback, &inputImage);
        }

        if (button3.contains(Point(x, y))) {
            cout << "Pencil Tool chosen." << endl;
            setMouseCallback("inputImage", pencilCallback, &inputImage);
        }

        if (button4.contains(Point(x, y))) {
            cout << "Paint Bucket Tool chosen." << endl;
            setMouseCallback("inputImage", paintBucketCallback, &inputImage);
        }
    }

    imshow(windowName, canvas);
    waitKey(1);
}

int main(int argc, char **argv) {

     // Check for command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1) {
        printf("Error CLI.\n");
        return 0;
    } else {
        inputImage = imread(argv[1], IMREAD_COLOR);
        originalImage = inputImage.clone();

        // Check for file error
        if(inputImage.data == 0) {
            cout << "Error while opening file " << argv[1] << endl;
            return 0;
        }
    }

    imshow("inputImage", inputImage);
    
    Mat3b img(2, 300, Vec3b(200, 200, 200));

    button1 = Rect(0, 0, 300, 50);
    button2 = Rect(0, 50, 300, 50);
    button3 = Rect(0, 100, 300, 50);
    button4 = Rect(0, 150, 300, 50);

    canvas = Mat3b(img.rows + (button1.height)*4, img.cols, Vec3b(200,200,200));

    canvas(button1) = Vec3b(200,200,200);
    putText(canvas(button1), buttonText1, Point(button1.width*0.35, button1.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));
    img.copyTo(canvas(Rect(0, button1.height, img.cols, img.rows)));

    canvas(button2) = Vec3b(200,200,200);
    putText(canvas(button2), buttonText2, Point(button2.width*0.35, button2.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));

    canvas(button3) = Vec3b(200,200,200);
    putText(canvas(button3), buttonText3, Point(button3.width*0.35, button3.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));

    canvas(button4) = Vec3b(200,200,200);
    putText(canvas(button4), buttonText4, Point(button4.width*0.35, button4.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));

    namedWindow(windowName);
    setMouseCallback(windowName, callBackFuncGui, &inputImage);

    imshow(windowName, canvas);
    waitKey();

    return 0;
}
