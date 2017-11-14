#pragma once

#define RGB 1
#define HSV 2

#include <iostream>
#include <cstdlib>
#include <string>
#include <array>
#include "matMsgBox.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

//Frame is a class to contain a image and also generate relavent info and image
class Frame: public cv::Mat
{
public:
	Frame();
	Frame(const cv::Mat&);
	//Get the inftoBox for the image in mat
	cv::Mat getInfoMat();
	cv::Mat getInfoMat(MatMsgBox&);
	//output the histogram
	cv::Mat getHistMat( int, int, int iOutputChannel);
	//Return the data of Histogram
	std::vector<cv::Mat> getHistVec(int iOutputChannel); //get Hist array
	//show the hist window
	void showHist( int, int, int iOutputChannel); //get Hist array + show Hist window
	//Color channel information
	//create a infobox MatMsgBox
	MatMsgBox getInfoBoxMat(int w, int h, int rh,double font);
	//update the infobox
	void updateInfoMat();
	//overload version of the previous one, with string input as infoBox window name
	void updateInfoMat(string);
	~Frame();
private:
	float chMean(int channelNo,int iOutputChannel);
	float chSD(int channelNo,int iOutputChannel);
	Scalar hueConvert(float);
	std::vector<float> dominantColorHue();
	//MatMsgBox infoBoxMat;

};

