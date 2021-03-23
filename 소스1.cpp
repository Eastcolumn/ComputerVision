/*********************************************************
				visual project
				2021.03.24
				Team 3
**********************************************************/

//opencv header file include
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types_c.h"
#define M_PI 3.14159265358979323846

using namespace cv;

void sobel_part(cv::Mat input_part, double& gradient_m, double& gradient_dir);
cv::Mat sobel(cv::Mat input);
double similarlity(cv::Mat src1, cv::Mat src2);
int search_key(cv::Mat src);

//project main function


cv::Mat sobel(cv::Mat input) {  // input NxN part
	cv::Mat gray(input.rows, input.cols, CV_8UC1);
	cv::Mat gradient_direction(input.rows - 2, input.cols - 2, CV_32S);
	cv::Mat gradient_weight(input.rows - 2, input.cols - 2, CV_64F);

	if (input.channels() == 3) {

		for (int x = 0; x < input.cols; x++) {
			for (int y = 0; y < input.rows; y++) {
				int sum = input.at<cv::Vec3b>(x, y)[0] + input.at<cv::Vec3b>(x, y)[1] + input.at<cv::Vec3b>(x, y)[2];
				sum = sum / 3;
				gray.at<uchar>(x, y) = sum;
			}
		}

	}

	else if (input.channels() == 1) {
		gray.copyTo(input);

	}



	for (int x = 1; x < gray.cols - 1; x++) {
		for (int y = 1; y < gray.rows - 1; y++) {

			cv::Mat subMat = gray(cv::Range(y - 1, y + 2), cv::Range(x - 1, x + 2));

			double gradient_m, gradient_dir;
			sobel_part(subMat, gradient_m, gradient_dir);
			gradient_direction.at<int>(x - 1, y - 1) = (int)((180 * gradient_dir / M_PI + 180) / 10);
			if (gradient_direction.at<int>(x - 1, y - 1) == 36) {
				gradient_direction.at<int>(x - 1, y - 1) -= 1;
			}
			gradient_weight.at<double>(x - 1, y - 1) = gradient_m;



		}
	}

	cv::Mat gradient(36, 1, CV_64F);

	for (int i = 0; i < gradient.rows; i++) {
	
		gradient.at<double>(i, 0) = 0;
	}
	for (int x = 0; x < gradient_direction.cols; x++) {
		for (int y = 0; y < gradient_direction.rows; y++) {
			int a = gradient_direction.at<int>(x, y);
			double  b = gradient_weight.at<double>(x, y);
			gradient.at<double>(gradient_direction.at<int>(x, y),0) += gradient_weight.at<double>(x,y);
		}
	}

	
	return gradient;
}

void sobel_part(cv::Mat input_part, double& gradient_m, double& gradient_dir) { // input part = 3x3 Mat


	if ((input_part.cols == 3) && (input_part.rows == 3)) {
		int x = 1;
		int y = 1;
		unsigned char a = input_part.at<uchar>(x + 1, y);
		unsigned char b = input_part.at<uchar>(x - 1, y);
		unsigned char c = input_part.at<uchar>(x , y+1);
		unsigned char d = input_part.at<uchar>(x , y-1);
		double k = pow((a - b),2) + pow((c - d), 2);
		gradient_m = sqrt(k);
		gradient_dir = atan2(input_part.at<uchar>(x, y + 1) - input_part.at<uchar>(x, y - 1), (input_part.at<uchar>(x + 1, y) - input_part.at<uchar>(x - 1, y)));
	}



}



double similarlity(cv::Mat src1, cv::Mat src2) {
	int key1, key2;
	
	key1 = search_key(src1);
	key2 = search_key(src2);

	src1 = src1 / src1.at<double>(key1, 0);
	src2 = src2 / src2.at<double>(key2, 0);

	double output = 0;
	for (int i = 0; i < src1.rows; i++) {
	
		output += abs(src1.at<double>((key1 + i) % 36, 0) - src2.at<double>((key2 + i) % 36, 0));

	}
	return output;

}


int search_key(cv::Mat src) {
	int key = 0;
	double Max = 0;
	for (int i = 0; i < src.rows; i++) {
		if (src.at<double>(i, 0) > Max) {
			Max = src.at<double>(i, 0);
			key = i;
		}
	}


	return key;
}






