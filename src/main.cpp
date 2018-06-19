#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>
#include <list>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "bilateralFilter.h"
#include "darkChannel.h"
#include "estimateAL.h"
#include "estimateTransmission.h"
#include "recoverSceneRadiance.h"
#include "softMatting.h"
#include <unistd.h>
using namespace std;
using namespace cv;

int main()
{
    Mat img, dark, t, dehaze, t_softMatting, temp;
    Vec3b A;

    //string imgPath = "../../../../images/tiananmen1.bmp";
    string imgPath = "../../../../images/forest1.jpg";

    double secs;

    img = imread(imgPath, CV_LOAD_IMAGE_COLOR);
    resize(img, img, Size(600, 450));
    cout << "Width = " << img.cols << " Height = " << img.rows << endl << endl;

    secs = (double) getTickCount();
    darkChannel(img, dark, 15);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "darkChannel = " << secs << "s" << endl << endl;;

    secs = (double) getTickCount();
    estimateAtmosphericLight(img, dark, A);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "estimateAL = " << secs << "s" << endl << endl;;

    secs = (double) getTickCount();
    estimateTransmission(img, t, A, 15);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "estimateTransmission = " << secs << "s" << endl << endl;;

    secs = (double) getTickCount();
    softMatting(img, t, t_softMatting, 3);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "softMatting = " << secs << "s" << endl << endl;;

    secs = (double) getTickCount();
    myBilateralFilter(t_softMatting, t_softMatting, 0.5, 0.5, 15);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "myBilateralFilter = " << secs << "s" << endl << endl;;

    secs = (double) getTickCount();
    recoverSceneRadiance(img, dehaze, t_softMatting, A);
    secs = ((double) getTickCount() - secs) / getTickFrequency();
    cout << "recoverSceneRadiance = " << secs << "s" << endl << endl;

    imshow("Original Image", img);
    imshow("Dark Channel Image", dark);
    imshow("Transmission Map", t);
    imshow("Transmission Map Soft Matting", t_softMatting);
    imshow("Dehaze Image", dehaze);
    waitKey(0);

    temp = Mat(t.size(), CV_8U);
    t_softMatting.convertTo(temp, CV_8U, 255.0);
    imwrite("../../../../results/dark.bmp", dark);
    imwrite("../../../../results/t.bmp", dark);
    imwrite("../../../../results/t_softMatting.bmp", temp);
    imwrite("../../../../results/dehaze.bmp", dehaze);
    
    return 0;
}
