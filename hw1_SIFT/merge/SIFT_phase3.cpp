#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types_c.h"

using namespace cv;

void drawline(Mat input, double points[8][2]);
void histogram(String name, std::vector<cv::Mat> sobel_blured);
int search_key(cv::Mat src);

void drawline(Mat input, double points[8][2]) { // input part = 3x3 Mat

    line(input, Point(points[0][0], points[0][1]), Point(points[4][0] + 600, points[4][1]), Scalar::all(255), 1, 8, 0);
    line(input, Point(points[1][0], points[1][1]), Point(points[5][0] + 600, points[5][1]), Scalar::all(255), 1, 8, 0);
    line(input, Point(points[2][0], points[2][1]), Point(points[6][0] + 600, points[6][1]), Scalar::all(255), 1, 8, 0);
    line(input, Point(points[3][0], points[3][1]), Point(points[7][0] + 600, points[7][1]), Scalar::all(255), 1, 8, 0);
}

void histogram(String name, std::vector<cv::Mat> sobel_blured) {
    int key;
    int count[4][11] = { 0, };
    Mat result1 = Mat::zeros(800, 400, CV_8UC3); // 세로, 가로 각각 400, 400 사이즈의 검정 이미지를 만든다. 
    Mat result2 = Mat::zeros(800, 400, CV_8UC3); // 세로, 가로 각각 400, 400 사이즈의 검정 이미지를 만든다. 
    Mat result3 = Mat::zeros(800, 400, CV_8UC3); // 세로, 가로 각각 400, 400 사이즈의 검정 이미지를 만든다. 
    Mat result4 = Mat::zeros(800, 400, CV_8UC3); // 세로, 가로 각각 400, 400 사이즈의 검정 이미지를 만든다. 


    for (int i = 0; i < 4; i++) {
        key = search_key(sobel_blured[i]);
        sobel_blured[i] = sobel_blured[i] / sobel_blured[i].at<double>(key, 0);
    }

        for (int j = 0; j < 36; j++) {
            line(result1, Point(10 + 10 * j, 400), Point(10 + 10 * j, 400 - (int)(400 * sobel_blured[0].at<double>(j, 0))), Scalar::all(255), 10, 8, 0);
            line(result2, Point(10 + 10 * j, 400), Point(10 + 10 * j, 400 - (int)(400 * sobel_blured[1].at<double>(j, 0))), Scalar::all(255), 10, 8, 0);
            line(result3, Point(10 + 10 * j, 400), Point(10 + 10 * j, 400 - (int)(400 * sobel_blured[2].at<double>(j, 0))), Scalar::all(255), 10, 8, 0);
            line(result4, Point(10 + 10 * j, 400), Point(10 + 10 * j, 400 - (int)(400 * sobel_blured[3].at<double>(j, 0))), Scalar::all(255), 10, 8, 0);
        }
   

       // line(result1, Point(10 + i * 20, 400), Point(10 + i * 20, 10 + 400 - count[0][i] * 20), Scalar::all(255), 10, 8, 0);



    hconcat(result1, result2, result1);
    hconcat(result1, result3, result1);
    hconcat(result1, result4, result1);


    cv::namedWindow(name + "result1", 1);
    cv::imshow(name + "result1", result1);


    cv::waitKey(0);

}