#ifndef DARK_CHANNEL
#define DARK_CHANNEL
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
void darkChannel(Mat& img, Mat& dark, int maskSize);
#endif
