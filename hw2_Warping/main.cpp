#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

struct Image {
    Mat img;
    string name;
    vector<Point> pnts;

    Image(string source, string _name) {
        img = imread(source, IMREAD_COLOR);
        name = _name;
        namedWindow(name);
    }

    bool empty() { return img.empty(); }
}

void onMouse(int event, int x, int y, int flags, void *userdata) {
    auto im = (Image *)userdata;
    auto &pnts = im->pnts;
    if (event == EVENT_LBUTTONDOWN && pnts.size() < 4) {
        pnts.push_back(Point(x, y));
        rectangle(im->img, Rect(x - 10, y - 10, 20, 20), Scalar(255, 0, 0), 5);
        imshow(im->name, im->img);
    }
}

int main(void) {
    Image img1("src.jpg", "src");
    Image img2("dst.jpg", "dst");

    if (img1.empty() || img2.empty()) return -1;

    setMouseCallback("src", onMouse, &img1);
    setMouseCallback("dst", onMouse, &img2);
    imshow("src", img1.img);
    imshow("dst", img2.img);

    Mat H = homography(img1.pnts, img2.pnts);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) cout << H[i][j] << " ";
        cout << '\n';
    }

    waitKey(0);
    destroyAllWindows();

    return 0;
}