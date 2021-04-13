#include "homography.h"

Mat homography(const vector<Point> &pnts1, const vector<Point> &pnts2) {
    if (pnts1.size() < 4 || pnts2.size() < 4) exit(-1);
    int x[4], y[4];
    int xp[4], yp[4];

    for (int i = 0; i < 4; ++i) {
        x[i] = pnts1[i].x, y[i] = pnts1[i].y;
        xp[i] = pnts2[i].x, yp[i] = pnts2[i].y;
    }

    Mat H =
        (Mat_<float>(8, 9) << -x[0], -y[0], -1, 0, 0, 0, x[0] * xp[0], y[0] * xp[0], xp[0], 0, 0, 0,
         -x[0], -y[0], -1, x[0] * yp[0], y[0] * yp[0], yp[0], -x[1], -y[1], -1, 0, 0, 0,
         x[1] * xp[1], y[1] * xp[1], xp[1], 0, 0, 0, -x[1], -y[1], -1, x[1] * yp[1], y[1] * yp[1],
         yp[1], -x[2], -y[2], -1, 0, 0, 0, x[2] * xp[2], y[2] * xp[2], xp[2], 0, 0, 0, -x[2], -y[2],
         -1, x[2] * yp[2], y[2] * yp[2], yp[2], -x[3], -y[3], -1, 0, 0, 0, x[3] * xp[3],
         y[3] * xp[3], xp[3], 0, 0, 0, -x[3], -y[3], -1, x[3] * yp[3], y[3] * yp[3], yp[3]);

    Mat U, S, VT;
    SVDecomp(H, U, S, VT, SVD::FULL_UV);
    transpose(VT, VT);

    Mat col_vec = VT.col(8);
    col_vec = col_vec / col_vec.at<float>(8, 0);
    Mat transform(Size(3, 3), CV_32FC1);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) transform.at<float>(i, j) = col_vec.at<float>(i * 3 + j, 0);

    return transform;
}