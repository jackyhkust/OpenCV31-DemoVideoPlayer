#include "matMsgBox.h"
#include <string>
#include <cstdlib>


MatMsgBox::MatMsgBox()
{
	
}

MatMsgBox::MatMsgBox(int w, int h, Scalar color)
{
	Mat colorMAT(w, h, CV_8UC3,color);
	colorMAT.copyTo(*this);
	//*this = colorMAT;<=That's super wrong

}

MatMsgBox::MatMsgBox(int w, int h, Scalar color, String inputwindowName)
{
	MatMsgBox(w, h, color);
	setWindowName(inputwindowName);
}

MatMsgBox::MatMsgBox(Mat inputMat)
{
	*this = inputMat;
}

void MatMsgBox::setColor(Scalar)
{
}

void MatMsgBox::setRowHeight(int rowh)
{
	rowHeight = rowh;
}

void MatMsgBox::addString(String inputStr,Scalar color= Scalar::all(255))
{
	rowNo++;
	putText(*this, inputStr, Point(0, rowNo*rowHeight), FONT_HERSHEY_SIMPLEX, dblFontSize, color);
}

void MatMsgBox::setFontSize(double fontSize)
{
	dblFontSize = fontSize;
}

void MatMsgBox::setWindowName(string inputWindowName)
{
	windowName = inputWindowName;
}

void MatMsgBox::update()
{
	cv::imshow(windowName, *this);
}







MatMsgBox::~MatMsgBox()
{
}
