#include "estimateTransmission.h"

void estimateTransmission(Mat& img, Mat& t, Vec3b A, int maskSize)
{
    t.create(img.size(), CV_64F);
    
    const double AL_B = A.val[0], AL_G = A.val[1], AL_R = A.val[2];
    double *const T_DATA = (double *) t.data;
    
    const int BORDER = maskSize/2;
    
    Mat imgBorder(img.rows+BORDER*2, img.cols+BORDER*2, img.type());
    copyMakeBorder(img, imgBorder, BORDER, BORDER, BORDER, BORDER, BORDER_REFLECT_101);
    
    const int ROWS = imgBorder.rows, COLS = imgBorder.cols,
    IMG_STRIDE1 = imgBorder.step1(), IMG_STRIDE2 = imgBorder.step1(1),
    T_STRIDE = t.step1();
    
    for (int borderY = BORDER ; borderY < ROWS-BORDER; borderY++) {
        for (int borderX = BORDER ; borderX < COLS-BORDER; borderX++) {
            
            double min = numeric_limits<double>::max(), pxl_b, pxl_g, pxl_r;
            for (int shiftY = -maskSize/2 ; shiftY <= maskSize/2 ; shiftY++) {
                for (int shiftX = -maskSize/2 ; shiftX <= maskSize/2 ; shiftX++) {
                    pxl_b = (double) imgBorder.data[(borderY+shiftY)*IMG_STRIDE1+(borderX+shiftX)*IMG_STRIDE2+0];
                    pxl_g = (double) imgBorder.data[(borderY+shiftY)*IMG_STRIDE1+(borderX+shiftX)*IMG_STRIDE2+1];
                    pxl_r = (double) imgBorder.data[(borderY+shiftY)*IMG_STRIDE1+(borderX+shiftX)*IMG_STRIDE2+2];
                    
                    // for BGR channel
                    min = (min > pxl_b / AL_B) ? (pxl_b / AL_B) : min;
                    min = (min > pxl_g / AL_G) ? (pxl_g / AL_G) : min;
                    min = (min > pxl_r / AL_R) ? (pxl_r / AL_R) : min;
                }
            }
            T_DATA[(borderY-BORDER)*T_STRIDE+(borderX-BORDER)] = 1.0 - 0.95*min;
        }
    }
}
