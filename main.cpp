#include <iostream>
#include <vector>
#include <dirent.h> //遍历系统指定目录下文件要包含的头文件
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int dodo(String imgpath){
    //    String imgpath = "/home/baymin/daily-work/wahaha/娃哈哈/20190826151428189.bmp";
    Mat srcImage = imread(imgpath, 1);  // Mat tmpImage = srcImage;
//    resize(srcImage, srcImage, Size(250, 125));
    imshow("srcImage", srcImage);

    // 判断图像是彩色图像还是灰度图像
    // 如果图像是彩色图像，需要将图像转换为灰度图像
    if (srcImage.channels()==3)
    {
        cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);
    }
    medianBlur(srcImage, srcImage, 5);
    adaptiveThreshold(srcImage, srcImage, 255, 0, 0, 9, 15);; //阈值化为二值图片

    imshow("xxxxxxxx", srcImage);
    moveWindow("xxxxxxxx",1000,100);
    Mat grayImg;

    medianBlur(srcImage, grayImg, 7); //中值滤波 看情况

//    imshow("grayImgmedianBlur", grayImg);

    Mat structure_element = getStructuringElement(MORPH_RECT, Size(8,30)); //设置膨胀/腐蚀的核为矩形，大小为8*30
//    dilate(grayImg, grayImg, structure_element); //膨胀
//    erode(dst, dst, structure_element); //先膨胀后腐蚀
//    namedWindow("dilate img", CV_WINDOW_AUTOSIZE);
//    imshow("dilate img", grayImg);
    getStructuringElement(MORPH_RECT, Size(4,4));
    erode(srcImage, grayImg, structure_element); //腐蚀
//    namedWindow("erode img", CV_WINDOW_AUTOSIZE);
    imshow("erode img", grayImg);



    int *res;
    cvCreateStructuringElementEx(3, 31, 1, 1, CV_SHAPE_ELLIPSE, res);


    float cannyThr = 60, FACTOR = 2.5;
    Mat cannyPic;
    Canny(grayImg, cannyPic, cannyThr, cannyThr*FACTOR); //Canny边缘检测
    imshow("cannyPic", cannyPic);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = NULL;//轮廓    //储存轮廓
    vector<Vec4i> hierarchy;
    vector<Point> listPoint(4);
//    findContours(cannyPic, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);    //获取轮廓

    IplImage imgTmp = cannyPic;//为了方便程序运行，这里采用老版的图像数据类型
    IplImage  *src = cvCloneImage(&imgTmp);//图像数据的深拷贝
    int count = cvFindContours(src, storage, &contours, \
		sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);//找出各个连通域
    CvBox2D box;//外接矩形
    CvBox2D maxLenBox;//最长长条的外接矩形

    Mat linePic = Mat::zeros(cannyPic.rows, cannyPic.cols, CV_8UC3);
    for (; contours != 0; contours = contours->h_next)//找到最长的长条
    {
        double area = cvContourArea(contours);
        if(area>13 && area <800){
            box = cvMinAreaRect2(contours, 0);//调用cvMinAreaRect2()函数返回外接矩形给box
            CvPoint2D32f rect_pts0[4];
            cvBoxPoints(box, rect_pts0);
            listPoint.push_back(rect_pts0[0]);
            listPoint.push_back(rect_pts0[1]);
            listPoint.push_back(rect_pts0[2]);
            listPoint.push_back(rect_pts0[3]);
            std::cout << "big area:" << area << std::endl;
        }
        std::cout << area << std::endl;

//        CvPoint2D32f* point2D32f;
//        cvBoxPoints(box, point2D32f);
//        listPoint.insert(point2D32f[0]);
//        listPoint.Add(point2D32f[1]);
//        listPoint.Add(point2D32f[2]);
//        listPoint.Add(point2D32f[3]);
        std::cout << area << std::endl;
    }
    RotatedRect rRect;
    rRect = minAreaRect(listPoint);
    Point2f vertices[4];
    rRect.points(vertices);

//    int x = (int)(rRect.center.x - Math.Min(box.size.width, box.size.height) / 2f) + iX;
//    int y = (int)(rRect.center.y - Math.Max(box.size.width, box.size.height) / 2) + iY;

    for (int i = 0; i < 4; i++)
        line(srcImage, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 2);

//    Rect brect = rRect.boundingRect();
//    rectangle(srcImage, brect, Scalar(255,0,0), 2);
    imshow("final", srcImage);

//    vector<vector<Point>> polyContours(contours.size());
//    Mat linePic = Mat::zeros(cannyPic.rows, cannyPic.cols, CV_8UC3);
//    int maxArea = 0;
//    int area = 0;
//    for (int index = 0; index < contours.size(); index++){
//        if (contourArea(contours[index]) > contourArea(contours[maxArea])){
//            maxArea = index;
//            area=contourArea(contours[index]);
//            approxPolyDP(contours[index], polyContours[index], 10, true);
//        }
//        cv::drawContours(linePic, contours, index, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8/*, hierarchy*/);
//    }
//    std::cout << area << std::endl;
//    Mat polyPic = Mat::zeros(srcImage.size(), CV_8UC3);
//    drawContours(polyPic, polyContours, maxArea, Scalar(0,0,255/*rand() & 255, rand() & 255, rand() & 255*/), 2);
//
//
//
//    for (; contour != 0; contour = contour->h_next)//找到最长的长条
//    {
//        box = cvMinAreaRect2(contour, 0);//调用cvMinAreaRect2()函数返回外接矩形给box
//    }



//    vector<vector<Point>> polyContours(contours.size());
//    int maxArea = 0;
//    for (int index = 0; index < contours.size(); index++){
//        if (contourArea(contours[index]) > contourArea(contours[maxArea]))
//            maxArea = index;
//        approxPolyDP(contours[index], polyContours[index], 10, true);
//    }
//    Mat polyPic = Mat::zeros(srcImage.size(), CV_8UC3);
//    drawContours(polyPic, polyContours, maxArea, Scalar(0,0,255/*rand() & 255, rand() & 255, rand() & 255*/), 2);
//
////    Mat polyPic = Mat::zeros(cannyPic.rows, cannyPic.cols, CV_8UC3);
////
////    drawContours(polyPic, contours, maxArea, Scalar(rand() & 255, rand() & 255, rand() & 255), 1, 8/*, hierarchy*/);

//    imshow("polyPic", polyPic);
    waitKey(0);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    const char *basePath = "../test/";

    DIR* dir = opendir(basePath);//打开指定目录
    dirent* p = NULL;//定义遍历指针
    while((p = readdir(dir)) != NULL)//开始逐个遍历
    {
        //这里需要注意，linux平台下一个目录中有"."和".."隐藏文件，需要过滤掉
        if(p->d_name[0] != '.')//d_name是一个char数组，存放当前遍历到的文件名
        {
            string name = basePath + string(p->d_name);
            dodo(name);
        }
    }
    closedir(dir);//关闭指定目录



    return 0;
}


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int mainddaa( int argc, char** argv )
{
    /// 加载源图像
    src = imread("/home/baymin/daily-work/wahaha/娃哈哈/aa.png", 1 );

    /// 转成灰度并模糊化降噪
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );

    Mat grayImg;

    medianBlur(src_gray, grayImg, 7); //中值滤波 看情况

//    imshow("grayImgmedianBlur", grayImg);

    Mat structure_element = getStructuringElement(MORPH_RECT, Size(6,15)); //设置膨胀/腐蚀的核为矩形，大小为3*3
    dilate(grayImg, grayImg, structure_element); //膨胀
//    erode(dst, dst, structure_element); //先膨胀后腐蚀
//    namedWindow("dilate img", CV_WINDOW_AUTOSIZE);
    imshow("dilate img", grayImg);
    getStructuringElement(MORPH_RECT, Size(4,4));
    erode(grayImg, grayImg, structure_element); //腐蚀
//    namedWindow("erode img", CV_WINDOW_AUTOSIZE);
    imshow("erode img", grayImg);
    Mat binPic;

    threshold(grayImg, binPic, 60, 255, THRESH_BINARY); //阈值化为二值图片
    imshow("binPic95", binPic);

    src_gray =grayImg;

    /// 创建窗体
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );

    createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );

    waitKey(0);
    return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// 用Canny算子检测边缘
    Canny( src_gray, canny_output, thresh, thresh*2);
    /// 寻找轮廓
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// 绘出轮廓
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }

    /// 在窗体中显示结果
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}