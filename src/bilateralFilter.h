#ifndef BILATERAL_FILTER
#define BILATERAL_FILTER
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
void myBilateralFilter(Mat& src, Mat& dst, double sigmaSpatial, double sigmaRange, int maskSize);
#endif
