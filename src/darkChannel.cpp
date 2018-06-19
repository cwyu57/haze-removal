#include "darkChannel.h"

void darkChannel(Mat& img, Mat& dark, int maskSize) {
    dark.create(img.size(), CV_8U);
    
    const int BORDER = maskSize/2;
    
    Mat imgBorder(img.rows+BORDER*2, img.cols+BORDER*2, img.type());
    copyMakeBorder(img, imgBorder, BORDER, BORDER, BORDER, BORDER, BORDER_REFLECT_101);
    
    const int IMGB_ROWS = imgBorder.rows, IMGB_COLS = imgBorder.cols;
    const int IMGB_STRIDE1 = imgBorder.step1(), IMGB_STRIDE2 = imgBorder.step1(1),
    DARK_STRIDE1 = dark.step1(), DARK_STRIDE2 = dark.step1(1);
    
    for (int borderY = BORDER ; borderY < IMGB_ROWS-BORDER ; borderY++) {
        for (int borderX = BORDER ; borderX < IMGB_COLS-BORDER ; borderX++) {
            
            uchar min = numeric_limits<uchar>::max(), pxl_b, pxl_g, pxl_r;
            for (int shiftY = -maskSize/2 ; shiftY <= maskSize/2 ; shiftY++) {
                for (int shiftX = -maskSize/2 ; shiftX <= maskSize/2 ; shiftX++) {
                    pxl_b = imgBorder.data[(borderY+shiftY)*IMGB_STRIDE1+(borderX+shiftX)*IMGB_STRIDE2+0];
                    pxl_g = imgBorder.data[(borderY+shiftY)*IMGB_STRIDE1+(borderX+shiftX)*IMGB_STRIDE2+1];
                    pxl_r = imgBorder.data[(borderY+shiftY)*IMGB_STRIDE1+(borderX+shiftX)*IMGB_STRIDE2+2];
                    
                    // for BGR channel
                    min = (min > pxl_b) ? pxl_b : min;
                    min = (min > pxl_g) ? pxl_g : min;
                    min = (min > pxl_r) ? pxl_r : min;
                }
            }
            dark.data[(borderY-BORDER)*DARK_STRIDE1+(borderX-BORDER)*DARK_STRIDE2] = min;
        }
    }
}
