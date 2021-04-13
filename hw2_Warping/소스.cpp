#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
/*Mat pseudo_inverse_H 3*3
,int x_src,int y_src 3*1
,int x_dst,int y_dst 3*1


*/



Mat warping(Mat pseudo_inverse_H, int x_dst, int y_dst) {
	
	
	// pseudo_inverse_H * [x,y,1]^T
	
	float x_src = pseudo_inverse_H.at<float>(0, 0) * x_dst
		+ pseudo_inverse_H.at<float>(0, 1) * y_dst
		+ pseudo_inverse_H.at<float>(0, 2);
	float y_src = pseudo_inverse_H.at<float>(1, 0) * x_dst
		+ pseudo_inverse_H.at<float>(1, 1) * y_dst
		+ pseudo_inverse_H.at<float>(1, 2);

	
	Mat result(2, 1, CV_32F, { x_src,y_src });
	return result;
}


Mat Mapping(Mat src,Mat dst,Mat dst_xy) {
	//dst_xy  = 2x4 mat [{x1,y1}, {x2,y2} {x3,y3} {x4,y4}] 
	// 1-2/ 2-3 /3-4 /4-1
	
	Mat output = dst.clone();
	
	
	
	/*
	interpolation
	
	
	*/








}

bool isInside(Mat xy,int x,int y){

	bool count = true;
	for (int i = 0; i < 4; i++) {
		
		if (i != 3) {
			float data[] = { xy.at<float>(i,0),xy.at<float>(i,1) ,xy.at<float>(i + 1,0) ,xy.at<float>(i + 1,1) ,(float)x,(float)y };
			Mat x( 2 , 3, CV_32F, data);
			line_position(x, i);
		}

		else {
		
			float data[] = { xy.at<float>(i,0),xy.at<float>(i,1) ,xy.at<float>(0,0) ,xy.at<float>(0,1) ,(float)x,(float)y };
			Mat x(2, 3, CV_32F, data);
			line_position(x, i);
		
		}
	}
}




bool line_position(Mat xy,int i){
	
	
	/*
	*  i is determine
	1-2 : 0
	2-3 : 1
	3-4 : 2
	4-1 : 3
	* 
	x1,y1
	x2,y2
	xk,yk << is this inside ?
	x1,y1 + determine i

	
	*/

	Mat AB(2, 1, CV_32F, { xy.at<int>(1, 0) - xy.at<int>(0, 0),xy.at<int>(1,1) - xy.at<int>(0,1) });
	Mat AK(2, 1, CV_32F, { xy.at<int>(2, 0) - xy.at<int>(0, 0),xy.at<int>(2,1) - xy.at<int>(0,1) });
	

	Mat AD;
	if (i == 0) { // right is inside
		Mat AD(2, 1, CV_32F, { xy.at<int>(0, 0) + 1,xy.at<int>(0,1) });	
	}
	else if (i == 1) { // up is inside
		Mat AD(2, 1, CV_32F, { xy.at<int>(0, 0) ,xy.at<int>(0,1)+1 });

	}
	else if (i == 2) { // left is inside
		Mat AD(2, 1, CV_32F, { xy.at<int>(0, 0) -1,xy.at<int>(0,1) });

	}
	else if (i == 3) { // down is inside
		Mat AD(2, 1, CV_32F, { xy.at<int>(0, 0) ,xy.at<int>(0,1)-1 });

	}
	else {
		printf("i = null");
		return false;
	}
	/*
	(AD cross AK) dot (AD cross AD)
	
	*/

	float calc = (AB.at<float>(0, 0) * AK.at<float>(1, 1) - AB.at<float>(0, 1) - AK.at<float>(1, 0))
		*(AB.at<float>(0, 0) * AD.at<float>(1, 1) - AB.at<float>(0, 1) - AD.at<float>(1, 0));

	if (calc >= 0) {
		return true;
	}
	else
		return false;


}