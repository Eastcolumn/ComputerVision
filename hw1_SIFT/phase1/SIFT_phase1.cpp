#include <algorithm>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

// N, sigmaX and Y, Kernel size
#define w 100
#define sigma 5
#define k 7

struct ImageData {
    cv::Mat origin;  // origin matrix of the images
    cv::Scalar color;
    std::string windowName;
    std::vector<cv::Mat> selected;  // The corners that you choose
    std::vector<cv::Mat> blured;    // Blured matrices of corner

    ImageData(cv::Mat _o, char *name, cv::Scalar _c) {
        origin = _o;
        color = _c;
        selected = std::vector<cv::Mat>();
        windowName = std::string(name);
    }

    void insertSubMat(int &x, int &y) {
        cv::Size s = origin.size();
        x = std::max(0, x - w / 2);
        y = std::max(0, y - w / 2);
        if (x + w >= s.width) x -= x + w - s.width;
        if (y + w >= s.height) y -= y + w - s.height;
        selected.push_back(origin(cv::Rect(x, y, w, w)));
    }

    // Gaussian filtering
    void gaussianBlur() {
        blured = std::vector<cv::Mat>(selected.size());
        /*
            use cv::GaussianBlur
            k: kernel size
            sigma: standard distribution of x and y
        */
        for (int i = 0; i < selected.size(); ++i)
            cv::GaussianBlur(selected[i], blured[i], cv::Size(k, k), sigma, sigma);
    }
};

void callback(int event, int x, int y, int flags, void *userdata) {
    auto image = (ImageData *)userdata;
    int size = (int)image->selected.size();
    if (event == cv::EVENT_LBUTTONDOWN && size < 4) {
        image->insertSubMat(x, y);
        cv::rectangle(image->origin, cv::Rect(x, y, w, w), cv::Scalar(0, 0, 255), 5);
        cv::putText(image->origin, std::string(1, size + '0'), cv::Point(x, y - 20),
                    cv::FONT_HERSHEY_COMPLEX, 5, image->color, 3);
        cv::imshow(image->windowName, image->origin);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Invalid input parameters - Usage: " << argv[0] << " [source] [input]\n";
        return -1;
    }
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

    // add mouse event to add a corner data and draw rectangle
    cv::namedWindow(source.windowName);
    cv::setMouseCallback(source.windowName, callback, &source);
    cv::imshow(source.windowName, source.origin);

    cv::namedWindow(input.windowName);
    cv::setMouseCallback(input.windowName, callback, &input);
    cv::imshow(input.windowName, input.origin);

    // hold
    cv::waitKey(0);
    cv::destroyAllWindows();

    // GaussianBlur
    source.gaussianBlur();
    input.gaussianBlur();
    auto blured1 = source.blured;
    auto blured2 = input.blured;

    return 0;
}