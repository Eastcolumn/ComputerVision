#include "homography.h"
#include "WWW.h"
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
};



void onMouse(int event, int x, int y, int flags, void *userdata) {
    auto im = (Image *)userdata;
    auto &pnts = im->pnts;
    if (event == EVENT_LBUTTONDOWN && pnts.size() < 4) {
        pnts.push_back(Point(x, y));
        rectangle(im->img, Rect(x - 10, y - 10, 20, 20), Scalar(255, 0, 0), 5);
        imshow(im->name, im->img);
    }
}

void backword(Image& src, Image& dst, Mat& IH) {

    // ÁÂÇ¥ 4ºÎºÐ

    Point2d src_P0(src.pnts[0].x, src.pnts[0].y);
    Point2d src_P1(src.pnts[1].x, src.pnts[1].y);
    Point2d src_P2(src.pnts[2].x, src.pnts[2].y);
    Point2d src_P3(src.pnts[3].x, src.pnts[3].y);


    Point2d dst_P0(dst.pnts[0].x, dst.pnts[0].y);
    Point2d dst_P1(dst.pnts[1].x, dst.pnts[1].y);
    Point2d dst_P2(dst.pnts[2].x, dst.pnts[2].y);
    Point2d dst_P3(dst.pnts[3].x, dst.pnts[3].y);


    // x' = [ h11 h12 h13 / h31 h32 h33 ] x y 1
    int min_x = 600; int max_x = 0;
    int min_y = 600; int max_y = 0;

    for (int i = 0; i < 4; i++) {

        if (dst.pnts[i].x > max_x) {
            max_x = dst.pnts[i].x;
        }
        if (dst.pnts[i].x < min_x) {
            min_x = dst.pnts[i].x;
        }
        if (dst.pnts[i].y > max_y) {
            max_y = dst.pnts[i].y;
        }
        if (dst.pnts[i].y < min_y) {
            min_y = dst.pnts[i].y;
        }
    }



    for (int c = min_x; c < max_x; c++) {
        for (int r = min_y; r < max_y; r++) {
            // c = x , r = y 
            // 00 10 20
            // 01 11 21
            // 02 21 22

            float X_src = IH.at<float>(0, 0) * c + IH.at<float>(0, 1) * r + IH.at<float>(0, 2);
            X_src = X_src / (IH.at<float>(2, 0) * c + IH.at<float>(2, 1) * r + IH.at<float>(2, 2));

            float Y_src = IH.at<float>(1, 0) * c + IH.at<float>(1, 1) * r + IH.at<float>(1, 2);
            Y_src = Y_src / (IH.at<float>(2, 0) * c + IH.at<float>(2, 1) * r + IH.at<float>(2, 2));

            // X , Y == X_src / Y_src
            if (X_src < 0)
                X_src = 0;
            if (Y_src < 0)
                Y_src = 0;
            dst.img.at<Vec3b>(r, c) = src.img.at<Vec3b>((int)Y_src, (int)X_src);


        }

    }







    // interpolation





    //




}


int main(void) {
    Image img1("src.jpg", "src");
    Image img2("dst.jpg", "dst");

    if (img1.empty() || img2.empty()) return -1;

    setMouseCallback("src", onMouse, &img1);
    setMouseCallback("dst", onMouse, &img2);
    imshow("src", img1.img);
    imshow("dst", img2.img);

    waitKey(0);
    destroyAllWindows();

    Mat H = homography(img1.pnts, img2.pnts);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) cout << H.at<float>(i, j) << " ";
        cout << "\n";
    }


    backword(img1, img2, H);
    imshow("dst", img2.img);

    waitKey(0);
    destroyAllWindows();
    return 0;
}



