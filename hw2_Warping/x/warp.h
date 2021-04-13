#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "homography.h"

using namespace cv;
using namespace std;




void warp_2(const Mat& src1, Mat& src2, const Mat& H);
bool triangle(Point2d p1, Point2d p2, Point2d p3, Point2d p);
bool inside(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int x, int y);
bool isInside(Mat xy, int x, int y);
Point2d warping(Mat pseudo_inverse_H, int x_dst, int y_dst);
//Mat Mapping(Mat H, Image img1, Image img2);
bool line_position(Mat xy, int i);
