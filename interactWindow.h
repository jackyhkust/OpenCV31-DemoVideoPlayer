/*
 * interactWindow.h
 *
 *  Created on: Jan 19, 2017
 *      Author: jacky
 */

#ifndef SRC_INTERACTWINDOW_H_
#define SRC_INTERACTWINDOW_H_
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "Frame.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
//An example of using multiple inhertiance, the class interactWindow has the propeties of a video and an image.
// Partly adpated from code provided on the class
class interactWindow : public cv::Mat,cv::VideoCapture
{
private:
	//declaration of all windows, all of them have an unique names.
    string windowName;
    string selColorWindowName;
    string infoBoxWindowName;
	string trackbarName;
	string histHSVName;
	string histRGBName;

	//Frame currentFrame;
	int currentPos;
	int finalPos;
	string inputFileName;
	
	VideoCapture cap1;
public:
	bool bVideoOpened=false;
	int updateStep = 1;

private:
    static void mouseEventCallback(int, int, int, int, void*);
    void mouseEvent ( int evt, int x, int y, int flags );
    void installCallback ( void );
    void createWindow ( void );


public:
    interactWindow ( void );
	
	//contructor with input video
	interactWindow(string,int);
	//load Mat on window
    bool loadImage ( cv::Mat);
	//Show the windows
    void showWindow ( void );
    static void rumMainLoop ( void );
	//put different info on windows time to time
	void playVideo();


};



#endif /* SRC_INTERACTWINDOW_H_ */
