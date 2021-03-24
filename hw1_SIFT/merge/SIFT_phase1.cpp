#include <algorithm>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace std;
#define w 100
#define sigma 5
#define k 7

void drawline(cv::Mat input, double points[8][2]);
void sobel_part(cv::Mat input_part, double& gradient_m, double& gradient_dir);
cv::Mat sobel(cv::Mat input);
double similarlity(cv::Mat src1, cv::Mat src2);
double points[8][2] = { 0, };
double points_result[8][2] = { 0, };
void histogram(cv::String name, std::vector<cv::Mat> sobel_blured);


struct ImageData {
    cv::Mat origin;
    cv::Scalar color;
    std::vector<cv::Mat> selected;
    std::vector<cv::Mat> blured;
    std::string windowName;

    ImageData(cv::Mat _o, char* name, cv::Scalar _c) {
        origin = _o;
        color = _c;
        selected = std::vector<cv::Mat>();
        windowName = std::string(name);
    }

    void insertSubMat(int& x, int& y) {
        cv::Size s = origin.size();
        x = std::max(0, x - w / 2);
        y = std::max(0, y - w / 2);
        if (x + w >= s.width) x -= x + w - s.width;
        if (y + w >= s.height) y -= y + w - s.height;
        selected.push_back(origin(cv::Rect(x, y, w, w)));
    }

    void gaussianBlur() {
        blured = std::vector<cv::Mat>(selected.size());
        for (int i = 0; i < selected.size(); ++i)
            cv::GaussianBlur(selected[i], blured[i], cv::Size(k, k), sigma, sigma);
    }
};

void callback_first(int event, int x, int y, int flags, void* userdata) {
    auto image = (ImageData*)userdata;
    int size = (int)image->selected.size();
    if (event == cv::EVENT_LBUTTONDOWN && size < 4) {
        image->insertSubMat(x, y);
        points[size][0] = x;
        points[size][1] = y;
        cv::rectangle(image->origin, cv::Rect(x, y, w, w), cv::Scalar(0, 0, 255), 5);
        cv::putText(image->origin, std::string(1, size + '0'), cv::Point(x, y - 20),
            cv::FONT_HERSHEY_COMPLEX, 5, image->color, 3);
        cv::imshow(image->windowName, image->origin);
    }
}
void callback_second(int event, int x, int y, int flags, void* userdata) {
    auto image = (ImageData*)userdata;
    int size = (int)image->selected.size();
    if (event == cv::EVENT_LBUTTONDOWN && size < 4) {
        image->insertSubMat(x, y);
        points[size + 4][0] = x;
        points[size + 4][1] = y;
        cv::rectangle(image->origin, cv::Rect(x, y, w, w), cv::Scalar(0, 0, 255), 5);
        cv::putText(image->origin, std::string(1, size + '0'), cv::Point(x, y - 20),
            cv::FONT_HERSHEY_COMPLEX, 5, image->color, 3);
        cv::imshow(image->windowName, image->origin);
    }
}
int main(int argc, char* argv[]) {
    // read image

    ImageData source = ImageData(cv::imread(argv[1]), argv[1], cv::Scalar(0, 255, 255));
    ImageData input = ImageData(cv::imread(argv[2]), argv[2], cv::Scalar(255, 255, 0));

    if (source.origin.empty()) {
        std::cout << "Error loading the image: " << argv[1] << '\n';
        return -1;
    }
    if (input.origin.empty()) {
        std::cout << "Error loading the image: " << argv[2] << '\n';
        return -1;
    }



    // add mouse event to add a corner data
    cv::namedWindow(source.windowName);
    cv::resize(source.origin, source.origin, cv::Size(600, 600));
    cv::setMouseCallback(source.windowName, callback_first, &source);
    cv::imshow(source.windowName, source.origin);

    cv::namedWindow(input.windowName);
    cv::resize(input.origin, input.origin, cv::Size(600, 600));
    cv::setMouseCallback(input.windowName, callback_second, &input);
    cv::imshow(input.windowName, input.origin);




    // hold
    cv::waitKey(0);
    cv::destroyAllWindows();


    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            points_result[i][j] = points[i][j];
        }
    }


    // GaussianBlur
    source.gaussianBlur();
    input.gaussianBlur();

    auto blured1 = source.blured;
    auto blured2 = input.blured;


    // sobel + gradient

    // sobel_blured[i] => Mat [degree 0~10 , degree 10~20 ... degree 350~ 360]  -> [36]array instance
    // sobel_blured => vector(Mat(cols,rows),<double>) cols = 36 rows = 1


    std::vector<cv::Mat> sobel_blured;
    std::vector<cv::Mat> sobel_blured2;


    for (int i = 0; i < 4; i++) {
        sobel_blured.push_back(sobel(blured1[i]));
        sobel_blured2.push_back(sobel(blured2[i]));
    }




    // calc similarlity
    int similar_mat[4] = { 0, 0, 0, 0 };
    double similar_rate[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                similar_rate[i] = similarlity(sobel_blured[i], sobel_blured2[j]);
            }
            else {
                if (similar_rate[i] > similarlity(sobel_blured[i], sobel_blured2[j])) {
                    similar_mat[i] = j;
                    similar_rate[i] = similarlity(sobel_blured[i], sobel_blured2[j]);
                }
            }
        }

    }
    for (int i = 0; i < 4; i++) {

        points_result[i + 4][0] = points[similar_mat[i] + 4][0];
        points_result[i + 4][1] = points[similar_mat[i] + 4][1];

    }

    histogram("1st", sobel_blured);
    histogram("2nd", sobel_blured2);


    cv::Mat result;
    hconcat(source.origin, input.origin, result);


    drawline(result, points_result);

    cv::imshow("hi", result);
    cv::waitKey(0);


    // i, left/right, x/y 


    return 0;
}
