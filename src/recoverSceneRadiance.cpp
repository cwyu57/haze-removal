#include "recoverSceneRadiance.h"

inline int myRound(double val)
{
    return (val >= 0) ? (int)(val + 0.5) : (int)(val - 0.5);
}
inline int myClip(int val, int min, int max)
{
    return (val < min) ? min : ( (val > max) ? max : val);
}
void recoverSceneRadiance(Mat& img, Mat& dehaze, Mat& t, Vec3b A)
{
    dehaze.create(img.size(), img.type());
    
    const double T0 = 0.1;
    const uchar AL_B = A.val[0], AL_G = A.val[1], AL_R = A.val[2];
    const int ROWS = dehaze.rows, COLS = dehaze.cols, STRIDE1 = img.step1(), STRIDE2 = img.step1(1);
    
    double maxT_T0;
    uchar pxl_b, pxl_g, pxl_r;
    
    for (int y = 0 ; y < ROWS ; y++) {
        for (int x = 0 ; x < COLS ; x++) {
            pxl_b   = img.data[y*STRIDE1+x*STRIDE2+0];
            pxl_g   = img.data[y*STRIDE1+x*STRIDE2+1];
            pxl_r   = img.data[y*STRIDE1+x*STRIDE2+2];
            maxT_T0 = max(t.at<double>(y, x), T0);
            
            // for BGR channel
            dehaze.data[y*STRIDE1+x*STRIDE2+0] = (uchar) myClip(myRound((double)(pxl_b-AL_B) / maxT_T0 + AL_B), 0, 255);
            dehaze.data[y*STRIDE1+x*STRIDE2+1] = (uchar) myClip(myRound((double)(pxl_g-AL_G) / maxT_T0 + AL_G), 0, 255);
            dehaze.data[y*STRIDE1+x*STRIDE2+2] = (uchar) myClip(myRound((double)(pxl_r-AL_R) / maxT_T0 + AL_R), 0, 255);
        }
    }
}
