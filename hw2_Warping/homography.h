#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

Mat homography(const vector<Point> &pnts1, const vector<Point> &pnts2);