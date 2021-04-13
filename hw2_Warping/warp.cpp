#include "warp.h"

/*Mat pseudo_inverse_H 3*3
,int x_src,int y_src 3*1
,int x_dst,int y_dst 3*1


*/


void warp_2(const Mat& src1, Mat& src2, const Mat& H) {

	Mat H_inv = H.inv();

	for (int h = 0; h < src2.rows; h++) {
		for (int w = 0; w < src2.cols; w++) {

			Point3f coor(w, h, 1);
			Mat tmp_coor = (H_inv * Mat(coor));
			tmp_coor /= tmp_coor.at<float>(2.0);

			Point trans_coor((cvRound(tmp_coor.at<float>(0, 0))), (cvRound(tmp_coor.at<float>(1, 0))));



			float tx = (cvRound(trans_coor.x));
			float ty = (cvRound(trans_coor.y));
			float a = trans_coor.x - tx;
			float b = trans_coor.y - ty;

			if (tx >= 0 && tx < src2.cols && ty >= 0 && ty < src2.rows) {
				for (int i = 0; i < 3; i++) {
					src2.at<Vec3b>(h, w)[i] = cvRound(((1.0 - a) * (1.0 - b) * src1.at<Vec3b>(ty, tx)[i])
						+ ((a * (1.0 - b)) * src1.at<Vec3b>(ty, tx + 1)[i])
						+ ((a * b) * src1.at<Vec3b>(ty + 1, tx + 1)[i])
						+ (((1.0 - a) * b) * src1.at<Vec3b>(ty + 1, tx))[i]);


				}

			}
		}

	}








}
Point2d warping(Mat pseudo_inverse_H, int x_dst, int y_dst) {
	
	
	// pseudo_inverse_H * [x,y,1]^T
	
	float x_src = pseudo_inverse_H.at<float>(0, 0) * x_dst
		+ pseudo_inverse_H.at<float>(0, 1) * y_dst
		+ pseudo_inverse_H.at<float>(0, 2);
	float y_src = pseudo_inverse_H.at<float>(1, 0) * x_dst
		+ pseudo_inverse_H.at<float>(1, 1) * y_dst
		+ pseudo_inverse_H.at<float>(1, 2);

	
	Point2d result( x_src,y_src );
	return result;
}

/*
Mat Mapping(Mat H, Image img1, Image img2){
	//dst_xy  = 2x4 mat [{x1,y1}, {x2,y2} {x3,y3} {x4,y4}] 
	// 1-2/ 2-3 /3-4 /4-1
	
	Mat output = img2.img.clone();

	for (int x = 1; x < img2.img.cols-1; x++) {
		for (int y =1; y < img2.img.rows-1; y++) {
			
			//if (isInside(dst_xy,x,y) == true) {
				if(inside(img2.pnts[0].x, img2.pnts[0].y,
					img2.pnts[1].x, img2.pnts[1].y,
					img2.pnts[2].x, img2.pnts[2].y,
					img2.pnts[3].x, img2.pnts[3].y,
					x,y)) {
				Point2d xy_src = warping(H, y, x);
				//output.at<Vec3b>(x, y) = img1.img.at<Vec3b>((int)xy_src.x, (int)xy_src.y);
				output.at<Vec3b>(y, x) = cv::Vec3b { 0,0,0 };
				}
				else {
				
					 printf(" %d  %d ", x, y);
				}
		
		}
	}
	





	return output;



}

bool isInside(Mat xy,int x,int y){

	bool count = true;
	for (int i = 0; i < 4; i++) {
					   
		if (i != 3) {
			float data[] = { xy.at<float>(0,i),xy.at<float>(1,i) ,xy.at<float>(0,i + 1) ,xy.at<float>(1 ,1 + i) ,(float)x,(float)y };
			Mat matx( 3 ,2, CV_32F, data);
			//print(matx);
			count = line_position(matx, i);
		}

		else {
		
			float data[] = { xy.at<float>(0,i),xy.at<float>(1,i) ,xy.at<float>(0,0) ,xy.at<float>(1,0) ,(float)x,(float)y };
			Mat matx(3,2, CV_32F, data);
			count = line_position(matx, i);
		
		}

		if (count == false) {
			return false;

		}

	}
	return count;
}




bool line_position(Mat xy,int i){
	
	
	
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

	
	
	//cv::print(xy);
	float data[2];

	if (i == 0) { // right is inside
		data[0] = xy.at<float>(0, 0)+1;
		data[1] =  xy.at<float>(1, 0);
	}
	else if (i == 1) { // up is inside
		data[0] = xy.at<float>(0, 0);
		data[1] = xy.at<float>(1, 0)+1;

	}
	else if (i == 2) { // left is inside
		data[0] = xy.at<float>(0, 0)+1;
		data[1] = xy.at<float>(1, 0);

	}
	else if (i == 3) { // down is inside
		data[0] = xy.at<float>(0, 0);
		data[1] = xy.at<float>(1, 0)-1;
		
	}
	else {
		printf("i = null");

		return false;
	}
	//cout << "½ÃÀÛ\n" << endl;

	//cout << xy.at<float>(1, 0)<< "  " << xy.at<float>(0, 0) << "\n"<<endl;
	//cout << xy.at<float>(1, 0)<<"  " << xy.at<float>(1, 1) << "\n" <<endl;
	//cout << xy.at<float>(2, 0) << "  " << xy.at<float>(2, 1) << "\n" << endl;
	//cout << xy.at<float>(1, 0) - xy.at<float>(0, 0) << xy.at<float>(1, 1) - xy.at<float>(0, 1) << endl;

	float data_AB[] = { xy.at<float>(1, 0) - xy.at<float>(0, 0),xy.at<float>(1,1) - xy.at<float>(0,1) };
	float data_AK[] = { xy.at<float>(2, 0) - xy.at<float>(0, 0),xy.at<float>(2,1) - xy.at<float>(0,1) };
	float data_AD[] = { data[0] - xy.at<float>(0, 0), data[1] - xy.at<float>(0, 1) };

	
	Mat AB(1, 2, CV_32F, data_AB);
	Mat AK(1, 2, CV_32F, data_AK);
	Mat AD(1, 2, CV_32F, data_AD);
	
	//print(AB);


	//print(AK);


	//print(AD);
	
	
	(AD cross AK) dot (AD cross AD)
	
	
	//print(AB);
	//printf("%d",AB.at<float>(0, 0));
	//printf("%d", AB.at<float>(0, 1));
	//print(AD);
	float calc = (AB.at<float>(0, 0) * AK.at<float>(0,1) - AB.at<float>(0, 1) * AK.at<float>(0, 0))
		*(AB.at<float>(0, 0) * AD.at<float>(0, 1) - AB.at<float>(0, 1) * AD.at<float>(0, 0));

	if (calc >= 0) {
		return true;
	}
	else
		return false;


}


bool inside(int x1,int x2,int x3,int x4,int y1,int y2,int y3,int y4,int x,int y) {
	Point2d p1(x1, y1);
	Point2d p2(x2, y2);
	Point2d p3(x3, y3);
	Point2d p4(x4, y4);
	Point2d p(x, y); //You are checking if this point lies in the triangle.

	if (triangle(p1, p2, p3, p)) {

		return true;

	}
	if (triangle(p1, p2, p4, p)) {

		return true;

	}
	if (triangle(p1, p4, p3, p)) {

		return true;

	}
	if (triangle(p4, p2, p3, p)) {

		return true;

	}
	
		return false;
	

}



bool triangle(Point2d p1, Point2d p2,Point2d p3, Point2d p) {

	float alpha = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) /
		((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
	float beta = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) /
		((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
	float gamma = 1.0f - alpha - beta;

	if ((gamma > 0) && (alpha > 0)) {


		if (beta > 0) {
			return true;


		}
		
	}
		return false;
	
}
*/