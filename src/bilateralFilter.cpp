#include "bilateralFilter.h"

void myBilateralFilter(Mat& src, Mat& dst, double sigmaSpatial, double sigmaRange, int maskSize)
{
    dst.create(src.size(), src.type());
    
    // allocate memory
    double **gaussianCoefSpatial = new double* [maskSize];
    double **gaussianCoefRange = new double* [maskSize];
    for (int i = 0 ; i < maskSize ; i++) {
        gaussianCoefSpatial[i] = new double [maskSize];
        gaussianCoefRange[i]   = new double [maskSize];
    }
    
    // generate gaussian spatial coefficient
    for (int i = 0 ; i < maskSize ; i++)
        for (int j = 0 ; j < maskSize ; j++)
            gaussianCoefSpatial[i][j] = exp(-(pow((double)(i-maskSize/2), 2.0)+pow((double)(j-maskSize/2), 2.0)) / (2.0*pow(sigmaSpatial, 2.0)));
    
    const int BORDER = maskSize/2;
    
    Mat srcBorder(src.rows+BORDER*2, src.cols+BORDER*2, src.type());
    copyMakeBorder(src, srcBorder, BORDER, BORDER, BORDER, BORDER, BORDER_REFLECT_101);
    
    const int SRCB_ROWS = srcBorder.rows, SRCB_COLS = srcBorder.cols;
    const int SRCB_STRIDE1 = srcBorder.step1(), SRCB_STRIDE2 = srcBorder.step1(1);
    const int DST_STRIDE1 = dst.step1(), DST_STRIDE2 = dst.step1(1);
    double *const SRCB_DATA = (double *) srcBorder.data, *const DST_DATA  = (double *) dst.data;
    
    for (int borderY = BORDER ; borderY < SRCB_ROWS-BORDER ; borderY++) {
        for (int borderX = BORDER ; borderX < SRCB_COLS-BORDER ; borderX++) {
            
            double W = 0.0, val = 0.0;
            for (int shiftY = -maskSize/2 ; shiftY <= maskSize/2 ; shiftY++) {
                for (int shiftX = -maskSize/2 ; shiftX <= maskSize/2 ; shiftX++) {
                    
                    // generate gaussian range coefficient
                    double tmp = SRCB_DATA[(borderY+shiftY)*SRCB_STRIDE1+(borderX+shiftX)*SRCB_STRIDE2] - SRCB_DATA[borderY*SRCB_STRIDE1+borderX*SRCB_STRIDE2];
                    gaussianCoefRange[shiftY+maskSize/2][shiftX+maskSize/2] = exp(-pow(tmp, 2.0) / (2.0*pow(sigmaRange, 2.0)));
                    
                    // calculate normalize factor
                    W += gaussianCoefSpatial[shiftY+maskSize/2][shiftX+maskSize/2] * gaussianCoefRange[shiftY+maskSize/2][shiftX+maskSize/2];
                    
                    // filtering
                    val += gaussianCoefSpatial[shiftY+maskSize/2][shiftX+maskSize/2] * gaussianCoefRange[shiftY+maskSize/2][shiftX+maskSize/2] * SRCB_DATA[(borderY+shiftY)*SRCB_STRIDE1+(borderX+shiftX)*SRCB_STRIDE2];
                }
            }
            val /= W;
            DST_DATA[(borderY-BORDER)*DST_STRIDE1+(borderX-BORDER)*DST_STRIDE2] = val;
        }
    }
    
    // free memory
    for (int i = 0 ; i < maskSize ; i++) {
        delete[] gaussianCoefSpatial[i];
        delete[] gaussianCoefRange[i];
    }
    delete[] gaussianCoefSpatial;
    delete[] gaussianCoefRange;
}
