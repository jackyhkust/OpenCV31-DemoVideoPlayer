#include "interactWindow.h"
#include "matMsgbox.h"
using namespace std;
using namespace cv;

interactWindow::interactWindow ()
{
    createWindow();
    installCallback ();
}


interactWindow::interactWindow(string input, int intUpdateStep)
{
	updateStep = intUpdateStep;
	inputFileName = input;
	open(input);
	createWindow();
	installCallback();
}

void interactWindow::mouseEventCallback (int evt, int x, int y, int flags, void* param)
    {
        interactWindow* ptrImageInWindow = (interactWindow*)param;
        ptrImageInWindow->mouseEvent ( evt, x, y, flags );
    }
    
    
    void interactWindow::installCallback ( void )
    {
    	setMouseCallback( windowName, mouseEventCallback, this );
    }
    
    void interactWindow::createWindow ( void )
    {
        static int id = 1;

		//set the name of each windows;
        stringstream name;
        stringstream selColor;
        stringstream infoBoxWindow;
		stringstream trackbar;
		stringstream histHSVNameSS;
		stringstream histRGBNameSS;
        name << "Window " << id;
        selColor<<"SelectedColorBox"<<id;
        infoBoxWindow<<"InfoBox"<<id;
		trackbar << "Trackbar" << id;
		histHSVNameSS << "HistHSV" << id;
		histRGBNameSS << "HistRGB" << id;


        this->windowName = name.str();
        this->selColorWindowName= selColor.str();
        this->infoBoxWindowName= infoBoxWindow.str();
		this->histHSVName = histHSVNameSS.str();
		this->histRGBName = histRGBNameSS.str();
		//builf the windows
        cv::namedWindow(infoBoxWindowName,1);
        cv::namedWindow( windowName , 1);
        cv::namedWindow (selColorWindowName,1);
		cv::namedWindow(histRGBName, 1);
		cv::namedWindow(histHSVName, 1);
		//create trackbar on main windows
		createTrackbar(trackbarName, windowName, &currentPos, get(CAP_PROP_FRAME_COUNT));
        id++;
    }
    

    bool interactWindow::loadImage ( Mat image )
    {
        this->cv::Mat::operator = (image);
        //if fail to read the image
        if ( this->empty() )
        {
            cout << "Error loading the image" << endl;
            return false;
        }
        return true;
    }
    void interactWindow::showWindow ( void )
    {
        cv::imshow( windowName, *this );
    }
 void interactWindow::rumMainLoop ( void )
    {
        // Wait until user press some key
        cv::waitKey(0);
    }

 
 void interactWindow::playVideo()
 {

	 int nStep = 0;
	 //this.VideoCapture::open();
	 cout << isOpened() << endl;
	 int currentPosition=0;
	 int currentCursor = 2;
	 cv::setTrackbarPos(trackbarName, windowName, currentPosition);
	 for (;;)
	 {
		 currentCursor = getTrackbarPos(trackbarName, windowName);
		 currentPosition= get(CAP_PROP_POS_FRAMES);
		 
		 if (currentCursor == currentPosition-1) {
			 cv::setTrackbarPos(trackbarName, windowName, currentPosition);
		 }
		 else {
			 cout << "click:" << currentPosition << endl;
			 set(CAP_PROP_POS_FRAMES, currentCursor);
		 }

		 nStep++;
		 Frame frame;
		 *this>> frame;
		 
		 loadImage(frame);
		 showWindow();
		 imshow(histHSVName, frame.getHistMat(HSV, 200, 200));
		 imshow(histRGBName, frame.getHistMat(RGB, 200, 200));
		 if (nStep%updateStep == 0) {
		 frame.updateInfoMat(infoBoxWindowName);
		 }

		 //imshow("InfoBox",frame.getInfoBoxMat());
		 if (waitKey(30) >= 0) break;
		 if (get(CAP_PROP_POS_FRAMES) >= get(CAP_PROP_FRAME_COUNT)) {
			 break;
		 }
	 }

 }
    


void interactWindow::mouseEvent( int evt, int x, int y, int flags )
{
    cout << windowName << " -> ";
    switch ( evt )
    {
        case cv::EVENT_LBUTTONDOWN :
        {
        	Scalar color=Scalar((int)at<cv::Vec3b>(y, x)[0],(int)at<cv::Vec3b>(y, x)[1],(int)at<cv::Vec3b>(y, x)[2],255);
			//using MatMsgBox class to simipfty the repetitive codes
			MatMsgBox colorBox(100, 100, color);
			colorBox.setWindowName(selColorWindowName);
			colorBox.setFontSize(0.3);


			Mat hsvColorMat;
			cvtColor(Mat(1, 1, CV_8UC3, color), hsvColorMat, CV_BGR2HSV);
			Scalar hsvColor = Scalar(hsvColorMat.data[0, 0, 0], hsvColorMat.data[0, 0, 1], hsvColorMat.data[0, 0, 2], 255);
			colorBox.setRowHeight(10);
			colorBox.addString("R: " + std::to_string(int(color[2])),Scalar::all(255));
			colorBox.addString("G: " + std::to_string(int(color[1])), Scalar::all(255));
			colorBox.addString("B: " + std::to_string(int(color[0])), Scalar::all(255));
			colorBox.addString("H: " + std::to_string(int(hsvColor[0])), Scalar::all(255));
			colorBox.addString("S: " + std::to_string(int(hsvColor[1])), Scalar::all(255));
			colorBox.addString("V: " + std::to_string(int(hsvColor[2])), Scalar::all(255));
			colorBox.update();
			
        	cout << "Left Button at position (" << x << "," << y << ") : " << (int)at<cv::Vec3b>(y, x)[0] << " : "
            << (int)at<cv::Vec3b>(y, x)[1] << " : " <<  (int)at<cv::Vec3b>(y, x)[2] << endl;
        }
        case cv::EVENT_MOUSEMOVE :
        {
        	cout << "Move (" << x << "," << y << ") : " << (int)at<cv::Vec3b>(y, x)[0] << " : "
            << (int)at<cv::Vec3b>(y, x)[1] << " : " <<  (int)at<cv::Vec3b>(y, x)[2] << endl;
        }
    }
}


//original testing code
//int main(int argc, char** argv)
//{
//
//    ImageInWindow image1;
//    ImageInWindow image2;
//
//    image1.loadImage ( argv[1] );
//    image2.loadImage ( argv[2] );
//
//    image1.showWindow ();
//    image2.showWindow ();
//
//    ImageInWindow::rumMainLoop ();
//
//    return 0;
//}
