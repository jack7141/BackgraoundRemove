#include "opencv2/opencv.hpp"
#include <time.h>

using namespace cv;
using namespace std;
int findBiggestContour(vector<vector<Point>>contour)
{
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i=0;i<contour.size();i++) {
        if (contour[i].size() > sizeOfBiggestContour){

            sizeOfBiggestContour=contour[i].size();
            indexOfBiggestContour=i;

        }
    }
    return indexOfBiggestContour;
}
int main()
{
    VideoCapture video(1);
    vector<vector<Point> > contours;
    vector<Vec4i> hierachy;
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );


    while (1)
    {
        Mat frame;
        Mat hsv,hsv2;
        video >> frame;
        cvtColor(frame, hsv, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat mask,mask2;
        inRange(hsv, Scalar(0, 130, 0), Scalar(179, 255, 255), mask);

        Mat mask_inv,mask_inv2;
        bitwise_not(mask,mask_inv,noArray());

        Mat Test,Test2;
        bitwise_and(frame,frame,Test,mask_inv);

        Mat crane;
        Test.copyTo(crane);
        cvtColor(crane, hsv2, COLOR_BGR2HSV);


        inRange(hsv2, Scalar(71, 35, 179), Scalar(179, 255, 255), mask2);
        dilate( mask2, mask2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( mask2, mask2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        erode(mask2, mask2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(mask2, mask2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        findContours( mask2, contours,hierachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

        int BiggestContours=findBiggestContour(contours);

         vector<vector<Point> > hull( contours.size() );
        convexHull( Mat(contours[BiggestContours]), hull[BiggestContours], false);
        drawContours( frame, hull ,BiggestContours,Scalar(0,0,255),4,LINE_8 );

        bitwise_and(crane,crane,Test2,mask2);
        dilate( Test2, Test2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( Test2, Test2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        erode(Test2, Test2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(Test2, Test2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


        imshow("Test3", Test2);
        imshow("Test", crane);
        imshow("Original", frame);

        if ( waitKey(1) == 27)break;
    }

}
