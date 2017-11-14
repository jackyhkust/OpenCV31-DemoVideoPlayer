#include "Frame.h"
#include "matMsgBox.h"
#include <array>
#include <cstdlib>
using namespace std;

Frame::Frame(){

}
Frame::Frame(const cv::Mat& src)
{
	src.copyTo(*this);

}


Frame::~Frame()
{
}
//get Histogram data in vector form
vector<Mat> Frame::getHistVec(int iOutputChannel = RGB)
{

	Mat channelPlanes[3];

	bool uniform = true; bool accumulate = false;
	vector<Mat> outHist;
	//spliting part
	//RGB channel1:B, Channel2:G,Channel3:R
	if (iOutputChannel == RGB) {
		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		split(*this, channelPlanes);
		Mat b_hist, g_hist, r_hist;
		calcHist(&channelPlanes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&channelPlanes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&channelPlanes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
		outHist = vector<Mat>{ b_hist, g_hist, r_hist };
	}
	else if (iOutputChannel == HSV) {
		cv::Mat hsvImage;
		cv::cvtColor(*this, hsvImage, COLOR_BGR2HSV);
		split(hsvImage, channelPlanes);
		Mat h_hist, s_hist, v_hist;
		int histSizeH = 180;
		int histSizeS = 100;
		int histSizeV = 100;
		float rangeH[] = { 0, histSizeH };
		float rangeS[] = { 0, histSizeS };
		float rangeV[] = { 0, histSizeV };
		const float* histRangeH = { rangeH };
		const float* histRangeS = { rangeS };
		const float* histRangeV = { rangeV };
		calcHist(&channelPlanes[0], 1, 0, Mat(), h_hist, 1, &histSizeH, &histRangeH, uniform, accumulate);
		calcHist(&channelPlanes[1], 1, 0, Mat(), s_hist, 1, &histSizeS, &histRangeS, uniform, accumulate);
		calcHist(&channelPlanes[2], 1, 0, Mat(), v_hist, 1, &histSizeV, &histRangeV, uniform, accumulate);
		outHist = vector<Mat>{ h_hist, s_hist, v_hist };
	}
	else {
		cout << "Wrong iOutputChannel" << endl;
	}
	
	return outHist;
}
//get the image of info box
cv::Mat Frame::getInfoMat(MatMsgBox& infoTemp)
{
	int oriRow = this->rows;
	int oriCol = this->cols;
	Mat histRGB = this->getHistMat(RGB, oriCol / 2, oriRow / 3);

	
	Mat outputFrame = Mat::zeros(oriRow + 10, oriCol + histRGB.cols + 10, this->type());
	Mat histHSV = this->getHistMat(HSV, oriCol / 2, oriRow / 3);
	this->copyTo(outputFrame(Range(0, oriRow), Range(0, oriCol)));
	histRGB.copyTo(outputFrame(Range(0, histRGB.rows), Range(oriCol, oriCol + histRGB.cols)));
	histHSV.copyTo(outputFrame(Range(histRGB.rows, histRGB.rows + histHSV.rows), Range(oriCol, oriCol + histHSV.cols)));
	infoTemp.copyTo(outputFrame(Range(histRGB.rows + histHSV.rows, histRGB.rows + histHSV.rows + infoTemp.rows), Range(oriCol, oriCol + infoTemp.cols)));


	return outputFrame;
	return cv::Mat();
}
//get image of histograms
Mat Frame::getHistMat(int iOutputChannel = RGB,int hist_w = 512, int hist_h = 400){
	Mat ch0, ch1, ch2;
		vector<cv::Mat>histTest = getHistVec(iOutputChannel);
		ch0 = histTest[0];
		ch1 = histTest[1];
		ch2 = histTest[2];


		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

		///// Normalize the result to [ 0, histImage.rows ]
		normalize(ch0, ch0, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(ch1, ch1, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		normalize(ch2, ch2, 0, histImage.rows, NORM_MINMAX, -1, Mat());
		if (iOutputChannel == RGB){
				/// Establish the number of bins
				int histSize = 256;

				// Draw the histograms for B, G and R

				int binRGB_w = cvRound((double)hist_w / histSize);
				///// Draw for each channel
				for (int i = 1; i < histSize; i++)
				{
					line(histImage, Point(binRGB_w*(i - 1), hist_h - cvRound(ch0.at<float>(i - 1))),
						Point(binRGB_w*(i), hist_h - cvRound(ch0.at<float>(i))),
						Scalar(255, 0, 0), 2, 8, 0);
					line(histImage, Point(binRGB_w*(i - 1), hist_h - cvRound(ch1.at<float>(i - 1))),
						Point(binRGB_w*(i), hist_h - cvRound(ch1.at<float>(i))),
						Scalar(0, 255, 0), 2, 8, 0);
					line(histImage, Point(binRGB_w*(i - 1), hist_h - cvRound(ch2.at<float>(i - 1))),
						Point(binRGB_w*(i), hist_h - cvRound(ch2.at<float>(i))),
						Scalar(0, 0, 255), 2, 8, 0);
				}
			}
			else if (iOutputChannel == HSV){
				int histSizeH = 180;
				int histSizeS = 100;
				int histSizeV = 100;

				// Draw the histograms for B, G and R

				int binH_w = cvRound((double)hist_w / histSizeH);
				int binS_w = cvRound((double)hist_w / histSizeS);
				int binV_w = cvRound((double)hist_w / histSizeV);

				for (int i = 1; i < histSizeH; i++)
				{
					line(histImage, Point(binH_w*(i - 1), hist_h - cvRound(ch0.at<float>(i - 1))),
						Point(binH_w*(i), hist_h - cvRound(ch0.at<float>(i))),
						Scalar(255, 0, 0), 2, 8, 0);
				}
				for (int i = 1; i < histSizeS; i++)
				{
					line(histImage, Point(binS_w*(i - 1), hist_h - cvRound(ch1.at<float>(i - 1))),
						Point(binS_w*(i), hist_h - cvRound(ch1.at<float>(i))),
						Scalar(0, 255, 0), 2, 8, 0);
				}
				for (int i = 1; i < histSizeV; i++)
				{
					line(histImage, Point(binV_w*(i - 1), hist_h - cvRound(ch2.at<float>(i - 1))),
						Point(binV_w*(i), hist_h - cvRound(ch2.at<float>(i))),
						Scalar(0, 0, 255), 2, 8, 0);
				}
			}
		return histImage;
}

void Frame::showHist(int iOutputChannel = RGB,int hist_w = 512, int hist_h = 400)
{
	//get the image of hist
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	histImage=getHistMat(iOutputChannel,hist_w,hist_h);

	//put them on windows
		if (iOutputChannel == RGB){
			namedWindow("HistRGB", WINDOW_AUTOSIZE);
			imshow("HistRGB", histImage);
		}
		else if (iOutputChannel == HSV){
			namedWindow("HistHSV", WINDOW_AUTOSIZE);
			imshow("HistHSV", histImage);
		}
}

float Frame::chMean(int channelNo,int iOutputChannel=RGB){
	//calculate the mean of a channel
	Vec4d meanArr,stddevArr;
	if(iOutputChannel==RGB){
		cv::meanStdDev(*this,meanArr,stddevArr);
	}else if(iOutputChannel==HSV){
		Mat tempHSV;
		cv::cvtColor(*this,tempHSV,COLOR_BGR2HSV);
		cv::meanStdDev(tempHSV,meanArr,stddevArr);
	}

	return meanArr[channelNo];
}

float Frame::chSD(int channelNo,int iOutputChannel=RGB){
	//calculate the SD of a channel
	Vec4d meanArr,stddevArr;
	if(iOutputChannel==RGB){
		cv::meanStdDev(*this,meanArr,stddevArr);
	}else if(iOutputChannel==HSV){
		Mat tempHSV;
		cv::cvtColor(*this,tempHSV,COLOR_BGR2HSV);
		cv::meanStdDev(tempHSV,meanArr,stddevArr);
	}

	return stddevArr[channelNo];
}
//a handy transfromation from hue to RGB color, satuation and value are preseted
Scalar Frame::hueConvert(float hue)
{
	Mat hsvColor(1, 1, CV_8UC3, Scalar(hue, 255, 255));
	Mat rgbColor(1, 1, CV_8UC3, Scalar(0, 0, 255));
	cvtColor(hsvColor, rgbColor, COLOR_HSV2BGR);
	Scalar finalColor = Scalar(rgbColor.data[0, 0, 0], rgbColor.data[0, 0, 1], rgbColor.data[0, 0, 2], 255);
	return finalColor;
}

vector<float> Frame::dominantColorHue(){
	
		cv::Mat hsvImage;
		Size small(this->rows/64,this->cols/64);
		Mat smallImg;
	//we scale the image to a smaller one to reduce processing time 
		cv::pyrDown(*this, smallImg);
		cv::pyrDown(smallImg, smallImg);
		cv::pyrDown(smallImg, smallImg);
		cv::pyrDown(smallImg, smallImg);
		cv::pyrDown(smallImg, smallImg);
		
		cv::cvtColor(smallImg, hsvImage, COLOR_BGR2HSV);
		Mat channelPlanes[3];
		split(hsvImage, channelPlanes);
	  	Mat hueImage=channelPlanes[0];
		Mat p=Mat::zeros(hueImage.rows*hueImage.cols, 1, CV_32F);
		hueImage.convertTo(p,CV_32F);
		Mat labels;
		Mat centers;
		kmeans(p, 3, labels,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 5, 2.0),3, KMEANS_PP_CENTERS, centers);
		//output the cluster centers to vector
		vector<float> output{ centers.at<float>(0), centers.at<float>(1),centers.at<float>(2) };
		return output;

}

//using the MatMsgBox to build a image for infobox
MatMsgBox Frame::getInfoBoxMat(int h, int w, int rh, double font){

		Scalar blackBackground = Scalar::all(0);
		MatMsgBox infoBox(h, w, blackBackground);
		infoBox.setRowHeight(rh);
		infoBox.setFontSize(font);
		infoBox.addString("R Mean: " + std::to_string(chMean(2)) + " SD: " + std::to_string(chSD(2)), Scalar::all(255));
		infoBox.addString("G Mean: " + std::to_string(chMean(1)) + " SD: " + std::to_string(chSD(1)), Scalar::all(255));
		infoBox.addString("B Mean: " + std::to_string(chMean(0)) + " SD: " + std::to_string(chSD(0)), Scalar::all(255));
		infoBox.addString("H Mean: " + std::to_string(chMean(0, HSV)) + " SD: " + std::to_string(chSD(0, HSV)), Scalar::all(255));
		infoBox.addString("S Mean: " + std::to_string(chMean(1, HSV)) + " SD: " + std::to_string(chSD(1, HSV)), Scalar::all(255));
		infoBox.addString("V Mean: " + std::to_string(chMean(2, HSV)) + " SD: " + std::to_string(chSD(2, HSV)), Scalar::all(255));
		vector<float>domHue = dominantColorHue();
		float domHue1 = domHue[0];
		float domHue2 = domHue[1];
		float domHue3 = domHue[2];
		string strhueCluster1 = "Hue Cluster 1:" + std::to_string(domHue1);
		string strhueCluster2 = "Hue Cluster 2:" + std::to_string(domHue2);
		string strhueCluster3 = "Hue Cluster 3:" + std::to_string(domHue3);
		Scalar finalColor1 = hueConvert(domHue1);
		Scalar finalColor2 = hueConvert(domHue2);
		Scalar finalColor3 = hueConvert(domHue3);
		infoBox.addString(strhueCluster1, finalColor1);
		infoBox.addString(strhueCluster2, finalColor2);
		infoBox.addString(strhueCluster3, finalColor3);


		return infoBox;
}

void Frame::updateInfoMat()
{
	//update the infobox
	MatMsgBox infoBox = getInfoBoxMat(300, 400, 20, 0.5);
	infoBox.setWindowName("InfoBox1");
	infoBox.update();
}
//update the info box with windowanme input
void Frame::updateInfoMat(string input) {
	MatMsgBox infoBox = getInfoBoxMat(300, 400, 20, 0.5);
	infoBox.setWindowName(input);
	infoBox.update();
}
//get an image with the frame and all relavent information and histogram
cv::Mat Frame::getInfoMat(){
	int oriRow=this->rows;
	int oriCol=this->cols;
	Mat histRGB=this->getHistMat(RGB,oriCol/2,oriRow/3);
	
	MatMsgBox infoBoxMat = getInfoBoxMat(oriRow/3,histRGB.cols, 20, 0.5);
	Mat outputFrame= Mat::zeros(oriRow+10,oriCol+histRGB.cols+10,this->type());
	Mat histHSV=this->getHistMat(HSV,oriCol/2,oriRow/3);
	this->copyTo(outputFrame(Range(0,oriRow),Range(0,oriCol)));
	histRGB.copyTo(outputFrame(Range(0,histRGB.rows),Range(oriCol,oriCol+histRGB.cols)));
	histHSV.copyTo(outputFrame(Range(histRGB.rows,histRGB.rows+histHSV.rows),Range(oriCol,oriCol+histHSV.cols)));
	infoBoxMat.copyTo(outputFrame(Range(histRGB.rows + histHSV.rows, histRGB.rows + histHSV.rows+infoBoxMat.rows), Range(oriCol, oriCol + infoBoxMat.cols)));
	

	return outputFrame;
}