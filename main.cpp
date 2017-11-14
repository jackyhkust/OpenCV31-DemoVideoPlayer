#include "Frame.h"
#include <array>
#include <cstdlib>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "VideoSrc.h"
#include "interactWindow.h"
using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //There are two modes in the program. Selected by the no. of arguments
	//1. The mode of videoOutput, 3 arguments are required.
	//	a.video input path
	//	b.video output path
	//	c.no. of skipping frame: a solution try to improve the speed of process
	//		for example: if 10 is used, then only one out of ten frames will give a statistics
	if(argc==4){
		//this code is intentionally not built in OOP style. We can see the code is less organized then the another one.

		VideoCapture cap(argv[1]); //open the input file
		//get file properties
		Frame firstFrame;
		cap>>firstFrame;
		double fps=cap.get(CAP_PROP_FPS);
		Size videoFrameSize=Size(firstFrame.getInfoMat().size[1],firstFrame.getInfoMat().size[0]);
		//Size videoFrameSize=Size(cap.get(CAP_PROP_FRAME_WIDTH),cap.get(CAP_PROP_FRAME_HEIGHT));
		//build the VideoWriter, set it accroding to the original format
		cv::VideoWriter videoExport(argv[2], cap.get(CAP_PROP_FOURCC),fps,videoFrameSize);

		if(!cap.isOpened())  // check if we succeeded
			return -1;
		//take the argument of updateStep
		int updateStep = atoi(argv[3]);
		MatMsgBox tempInfoBox;
		int nStep = 0;
		//use a while loop  to get all the frame
		for(;;)
		{
			//Read and then write it to the new file
			Frame frame;
			cap >> frame;
			if (nStep%updateStep == 0) {
				tempInfoBox = frame.getInfoBoxMat(frame.rows/ 3, frame.cols/2, 20, 0.5);
			}
			videoExport.write(frame.getInfoMat(tempInfoBox));

			if(waitKey(30) >= 0) break;
			//check the end of video
			if(cap.get(CAP_PROP_POS_FRAMES)>=cap.get(CAP_PROP_FRAME_COUNT)){
				break;
			}
			nStep++;
		}
		return 0;
    }else if(argc==3){
		//this one is OOP, the function are all embeded into the member function class.
		int updateStep = atoi(argv[2]);
		//show the window and play it 
		interactWindow mainWindow(argv[1],updateStep);
		mainWindow.playVideo();
    }else{

    	cout<<"Incorrect No. of Input"<<endl;
    }

}
