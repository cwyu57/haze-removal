#ifndef RECOVER_SCENE_RADIANCE
#define RECOVER_SCENE_RADIANCE
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
void recoverSceneRadiance(Mat& img, Mat& dehaze, Mat& t, Vec3b A);
#endif
