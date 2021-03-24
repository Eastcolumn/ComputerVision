#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types_c.h"

using namespace cv;

void drawline(Mat input, double points[8][2]);

void drawline(Mat input, double points[8][2]) { // input part = 3x3 Mat

	line(input, Point(points[0][0], points[0][1]), Point(points[4][0]+600, points[4][1]), Scalar::all(255), 1, 8, 0);
	line(input, Point(points[1][0], points[1][1]), Point(points[5][0] + 600, points[5][1]), Scalar::all(255), 1, 8, 0);
	line(input, Point(points[2][0], points[2][1]), Point(points[6][0] + 600, points[6][1]), Scalar::all(255), 1, 8, 0);
	line(input, Point(points[3][0], points[3][1]), Point(points[7][0] + 600, points[7][1]), Scalar::all(255), 1, 8, 0);

}
