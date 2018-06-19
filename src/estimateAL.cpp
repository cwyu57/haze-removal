#include "estimateAL.h"

void estimateAtmosphericLight(Mat& img, Mat& darkChannel, Vec3b& A)
{
    vector<Vec2i> container[256];
    for (int y = 0 ; y < darkChannel.rows ; y++) {
        for (int x = 0 ; x < darkChannel.cols ; x++) {
            Vec2i tmp(y, x);
            container[(int)darkChannel.at<uchar>(y, x)].push_back(tmp);
        }
    }
    
    int cnt = 0;
    const int LOOPMAXCNT = (int) ((double) (img.rows*img.cols) * 0.001);
    const int STRIDE1 = (int) img.step1(), STRIDE2 = (int) img.step1(1);
    double intensityMax = numeric_limits<double>::min();
    for (int i = 255 ; i >= 0 ; i--) {
        for (int j = 0 ; j < container[i].size() ; j++) {
            int y = container[i][j].val[0], x = container[i][j].val[1];
            double b = img.data[y*STRIDE1+x*STRIDE2+0],
            g = img.data[y*STRIDE1+x*STRIDE2+1],
            r = img.data[y*STRIDE1+x*STRIDE2+2],
            intensity = 0.299*r + 0.587*g + 0.114*b;
            
            if (intensity > intensityMax) {
                intensityMax = intensity;
                A.val[0] = b;
                A.val[1] = g;
                A.val[2] = r;
            }
            cnt++;
            if (cnt >= LOOPMAXCNT)
                return;
        }
    }
}
