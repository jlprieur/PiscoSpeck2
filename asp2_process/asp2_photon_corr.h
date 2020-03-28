/********************************************************************
* photon_corr.h
*
* Set of routines used for image restoration
* (contained in "photon_corr.cpp", "inv_bispec2.cpp", "jlp_median.cpp
*  and "clean_deconv.cpp")
*
* JLP
* Version 14/10/2008
*******************************************************************/
#ifndef _photon_corr_h   /* Sentry */
#define _photon_corr_h

/* Contained in photon_corr.cpp */
int photon_corr_auto1(double *bispp, double *modsq, double *phot_modsq,
                          int nx, int ny, int bisp_dim, double *xphot,
                          int nbeta, int ngamma, char *err_msg);
int compute_nphotons(double *modsq, double *phot_modsq, double *a1,
                     int nx, int ny, int idim, double rmin, double rmax);

// Contained in vcrb_2008/inv_bispec2.cpp:
int inv_bispec2(double *re_out, double *im_out, double *bisp1, double *modsq1,
                int ir, int ngamma, int nbeta, int nclos_max, int nx1, int ny1);

// Contained in vcrb_2008/clean_deconv.cpp:
int clean_deconvolution(double *in_map, double *cleaned_map,
                        double *modsq_of_unresolved, int nx, int ny,
                        double omega, double noise, int ir,
                        int *n_iterations);

#endif
