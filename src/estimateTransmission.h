#ifndef ESTIMATE_TRANSMISSION
#define ESTIMATE_TRANSMISSION
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
void estimateTransmission(Mat& img, Mat& t, Vec3b A, int maskSize);
#endif
