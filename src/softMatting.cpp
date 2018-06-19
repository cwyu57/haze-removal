#include "softMatting.h"

void softMatting(const Mat &src, const Mat &transmission, Mat &op_transmission, int k_size)
{
    const double EPS = 0.2; //pow(10.0, -1);
    const double LAMBDA = pow(10.0, -4);
    const size_t ROWS = src.rows, COLS = src.cols;
    const int K_CENTER = (k_size - 1) / 2;
    
    SparseMatrix<double> sm(ROWS*COLS, ROWS*COLS);
    vector<Triplet<double> > tripletList;
    VectorXd T(ROWS*COLS);
    VectorXd T_OPTIMAL(ROWS*COLS);
    
    Mat local_win, cov_mat, inv_cov_mat;
    Scalar mu;
    
    cov_mat.create(Size(k_size, k_size), CV_64FC1);
    inv_cov_mat.create(Size(k_size, k_size), CV_64FC1);
    
    uchar *local_data;
    double *cov_data, *inv_cov_data;
    const size_t LOCAL_STRIDE1 = src.step1(0), LOCAL_STRIDE2 = src.step1(1),
    COV_STRIDE = cov_mat.step1(), INV_COV_STRIDE = inv_cov_mat.step1();
    
    // get laplacian matrix
    for (int i = K_CENTER; i < ROWS - K_CENTER; i++) {
        for (int j = K_CENTER; j < COLS - K_CENTER; j++) {
            
            local_win = src.colRange(j-K_CENTER, j+K_CENTER+1).rowRange(i-K_CENTER, i+K_CENTER+1);
            local_data = local_win.data;
            
            mu = mean(local_win);
            cov_mat.setTo(0);
            
            cov_data = (double *) cov_mat.data;
            for (size_t a = 0; a < local_win.rows; a++) {
                for (size_t b = 0; b < local_win.cols; b++) {
                    cov_data[0*COV_STRIDE+0] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+0]-mu[0]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+0]-mu[0]);
                    cov_data[0*COV_STRIDE+1] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+0]-mu[0]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+1]-mu[1]);
                    cov_data[0*COV_STRIDE+2] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+0]-mu[0]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+2]-mu[2]);
                    cov_data[1*COV_STRIDE+1] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+1]-mu[1]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+1]-mu[1]);
                    cov_data[1*COV_STRIDE+2] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+1]-mu[1]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+2]-mu[2]);
                    cov_data[2*COV_STRIDE+2] += (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+2]-mu[2]) * (local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+2]-mu[2]);
                }
            }
            
            cov_data[0*COV_STRIDE+0] = (cov_data[0*COV_STRIDE+0]+EPS) / (k_size * k_size);
            cov_data[0*COV_STRIDE+1] /= k_size * k_size;
            cov_data[0*COV_STRIDE+2] /= k_size * k_size;
            cov_data[1*COV_STRIDE+0] =  cov_data[0*COV_STRIDE+1];
            cov_data[1*COV_STRIDE+1] = (cov_data[1*COV_STRIDE+1]+EPS) / (k_size * k_size);
            cov_data[1*COV_STRIDE+2] /= k_size * k_size;
            cov_data[2*COV_STRIDE+0] =  cov_data[0*COV_STRIDE+2];
            cov_data[2*COV_STRIDE+1] =  cov_data[1*COV_STRIDE+2];
            cov_data[2*COV_STRIDE+2] = (cov_data[2*COV_STRIDE+2]+EPS) / (k_size * k_size);
            
            inv_cov_mat = cov_mat.inv(DECOMP_SVD);
            inv_cov_data = (double *) inv_cov_mat.data;
            
            for (size_t a = 0; a < local_win.rows; a++) {
                for (size_t b = 0; b < local_win.cols; b++) {
                    int index_i = (i - K_CENTER + a) * src.cols + j - K_CENTER + b;
                    double diff_i[3] = {local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+0]-mu[0],
                        local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+1]-mu[1],
                        local_data[a*LOCAL_STRIDE1+b*LOCAL_STRIDE2+2]-mu[2]};
                    
                    for (size_t c = 0; c < local_win.rows; c++) {
                        for (size_t d = 0; d < local_win.cols; d++) {
                            int index_j = (i - K_CENTER + c) * src.cols + j - K_CENTER + d;
                            double diff_j[3] = {local_data[c*LOCAL_STRIDE1+d*LOCAL_STRIDE2+0]-mu[0],
                                local_data[c*LOCAL_STRIDE1+d*LOCAL_STRIDE2+1]-mu[1],
                                local_data[c*LOCAL_STRIDE1+d*LOCAL_STRIDE2+2]-mu[2]};
                            
                            double tmp[3] = {inv_cov_data[0*INV_COV_STRIDE+0] * diff_j[0]+
                                inv_cov_data[0*INV_COV_STRIDE+1] * diff_j[1]+
                                inv_cov_data[0*INV_COV_STRIDE+2] * diff_j[2],
                                inv_cov_data[1*INV_COV_STRIDE+0] * diff_j[0]+
                                inv_cov_data[1*INV_COV_STRIDE+1] * diff_j[1]+
                                inv_cov_data[1*INV_COV_STRIDE+2] * diff_j[2],
                                inv_cov_data[2*INV_COV_STRIDE+0] * diff_j[0]+
                                inv_cov_data[2*INV_COV_STRIDE+1] * diff_j[1]+
                                inv_cov_data[2*INV_COV_STRIDE+2] * diff_j[2]};
                            
                            double tmp_value = diff_i[0] * tmp[0] + diff_i[1] * tmp[1] + diff_i[2] * tmp[2];
                            
                            if (index_i == index_j) {
                                Triplet<double> tmp(index_i, index_j, 1.0 - (1 + tmp_value) / (k_size * k_size));
                                tripletList.push_back(tmp);
                            }
                            else {
                                Triplet<double> tmp(index_i, index_j, 0.0 - (1 + tmp_value) / (k_size * k_size));
                                tripletList.push_back(tmp);
                            }
                        }
                    }
                }
            }
        }
    }
    
    sm.setFromTriplets(tripletList.begin(), tripletList.end());
    
    for (int i = 0 ; i < ROWS*COLS ; ++i)
        sm.coeffRef(i, i) += LAMBDA;
    
    size_t t_index = 0;
    double *const TRANS_DATA = (double *) transmission.data;
    const size_t TRANS_STRIDE = transmission.step1();
    for (size_t y = 0 ; y < ROWS ; ++y) {
        for (size_t x = 0 ; x < COLS ; ++x) {
            T(t_index) = LAMBDA*TRANS_DATA[y*TRANS_STRIDE+x];
            t_index++;
        }
    }
    
    // solve sparse linear systems for Ax = b
    cout << "Size = " << sm.rows() << " x " << sm.cols() << endl;
    cout << "(L+LAMBDA*U) non zeros = " << sm.nonZeros() << endl;
    cout << "begin solve" << endl;
    
    ConjugateGradient<SparseMatrix<double> > cg;
    cg.setTolerance(EPS);
    cg.compute(sm);
    T_OPTIMAL = cg.solve(T);
    
    cout << "#iterations:     " << cg.iterations() << std::endl;
    cout << "estimated error: " << cg.error()      << std::endl;
    
    op_transmission.create(transmission.size(), transmission.type());
    double *const OP_TRANS_DATA = (double *) op_transmission.data;
    const size_t OP_TRANS_STRIDE = op_transmission.step1();
    for (size_t i = 0 ; i < ROWS*COLS ; ++i)
        OP_TRANS_DATA[(i/COLS)*OP_TRANS_STRIDE+(i%COLS)] = T_OPTIMAL(i);
}
