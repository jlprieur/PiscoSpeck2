/****************************************************************************
* Set of routines contained in covermas.cpp and mainly called by decode.cpp
*
* JLP
* Version 06/08/2010
*****************************************************************************/
#ifndef __asp2_covermas
#define __asp2_covermas

#include "jlp_numeric.h"  // FFTW_COMPLEX

#ifndef SQUARE
#define SQUARE(a) ((a)*(a))
#endif
int compute_uv_coverage(char* fmask_name, int ir, int* nbeta,
                        int* ngamma, int max_nclosure, char *err_message);
int compute_uv_coverage_1D(char* fmask_name, int ir, int* nbeta,
                           int* ngamma, int max_nclosure,
                           char *err_message);
int COVERA_MASK(double* mask, int* nx_mask, int* ny_mask, int* ir,
                int* max_nclosure, int* nbeta, int* ngamma, char *err_message);
int jlp_normalize_fft_1D(double* bispp, double* modsq, double* snrm,
                         int* nx, int* ny, int* nbeta, int* ngamma);
int jlp_normalize_fft(double* bispp, double* modsq, double* snrm,
                      int* nx, int* ny, int* nbeta, int* ngamma);
int photon_corr(double* bispp, double* modsq, double* snrm,
                int* nx, int* ny, double* xphot, int* nbeta, int* ngamma,
                int* photon_correction);
int rearrange_mask(double* bispp, int* ngamma, int* ydim1);
int photon_corr_mask(double* bispp, double* modsq, int* nx, int* ny,
                     int* bisp_dim, double* xphot, int* nbeta, int* ngamma,
                     double* fraction);
int photon_corr_1D(double* bispp, double* modsq, int* nx, int* ny,
                   int* bisp_dim, int* nbeta, int* ngamma);
int bisp2D_to_2D_image(double* out_image, double* modsq,
                       int nx, int ny, double* bisp, int nx_bisp,
                       int nbeta, int ngamma, int ShouldNormalize, int iopt,
                       char *err_message);
int bisp1D_to_2D_image(double* out_image, int* nx_out, int* ny_out,
                       double* modsq, int* nx_modsq, double* bisp, int* nx_bisp,
                       int* nbeta, int* ngamma, int* line_to_output,int* iopt,
                       char *err_message);
int COVER_NGT(int* ngt_val, int* index);
int COVER_KLM(int* klm_val, int* klm_index, int* ng_index);
int COVER_NBCOUV(int* nb_val, int* index1, int* index2, int* ir_max);
int COVER_IXY(int* ixy1_val, int* ixy2_val, int* index);
int COVER_IXY_1D(int* ixy1_val, int* index);
int bispec3(double* re, double* im, double* modsq, double* snrm,
            int nx, int ny, double* bispp, int ir, int nbeta, int ngamma);
int bispec3_fftw(FFTW_COMPLEX *data, double* modsq, double* snrm,
            int nx, int ny, double* bispp, int ir, int nbeta, int ngamma,
            char *err_message);
int bispec3_fourn(double* data, double* modsq, double* snrm,
            int nx, int ny, double* bispp, int ir, int nbeta, int ngamma);
int bispec_1D_Y(double* re, double* im, double* modsq, double* snrm,
              int nx, int ny, double* bispp, int ir, int nbeta, int ngamma);
int COVERA_MASK_1D(double* mask, int* nx_mask, int* ir, int* max_nclosure,
                   int* nbeta, int* ngamma, char *err_message);
int cover_ngt1(int* ng2, int nb);
int cover_klm1(int* k, int kk, int ng);
int cover_klm0(int* k, int kk, int ng);
int output_bisp(double* bispp, double* bisp1, int ngamma);
int output_bisp_1D(double* bispp, double* bisp1, int ngamma, int nx);
int prepare_output_bisp(double* bispp, double* modsq, double* snrm,
                        int nx, int ny, double xframes,
                        int nbeta, int ngamma);
int prepare_output_bisp_1D(double* bispp, double* modsq, double* snrm,
                           int nx, int ny, double xframes,
                           int nbeta, int ngamma);
#endif
