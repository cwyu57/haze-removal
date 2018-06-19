#ifndef ESTIMATE_AL
#define ESTIMATE_AL
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
void estimateAtmosphericLight(Mat& img, Mat& darkChannel, Vec3b& A);
#endif
