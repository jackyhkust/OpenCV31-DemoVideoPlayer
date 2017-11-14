#pragma once
#include "C:\opencv\build\include\opencv2\core\mat.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <string>
using namespace cv;
using namespace std;
//MatMsgBox is a class to build an image containing text info
class MatMsgBox :
	public cv::Mat
{
private:
	int rowNo=0;
	int rowHeight=10;
	double dblFontSize = 1.0;
	String windowName;
public:
	MatMsgBox();
	MatMsgBox(int, int, Scalar);
	MatMsgBox(int, int, Scalar, String);
	MatMsgBox(Mat);
	void setColor(Scalar);
	void setRowHeight(int);
	void addString(String,Scalar);
	void setFontSize(double);
	void setWindowName(string);
	void update();

	~MatMsgBox();
};

