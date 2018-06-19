#ifndef SOFT_MATTING
#define SOFT_MATTING
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
using namespace std;
using namespace cv;
using namespace Eigen;
void softMatting(const Mat &src, const Mat &transmission, Mat &op_transmission, int k_size);
#endif
