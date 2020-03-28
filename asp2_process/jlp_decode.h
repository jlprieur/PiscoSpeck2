/****************************************************************************
* asp2_decod.h
* JLP_Decode Class
*
* To compute autocorrelation, power spectrum and bispectrum of elementary frames
*
* ProcessingMode = 0 : no processing (only numerisation)
* ProcessingMode = 1 : Only integration
* ProcessingMode = 2 : autoc - crossc, FFT modulus, long integration
* ProcessingMode = 3 : autoc - crossc, FFT modulus, long integration, quadrant
* ProcessingMode = 4 : Bispectrum, FFT modulus, long integration
* ProcessingMode = 5 : Bispectrum, autoc, quadrant
* ProcessingMode = 6 : Spectroscopy
* ProcessingMode = 7 : SCIDAR Log
* ProcessingMode = 8 : SCIDAR Lin
* ProcessingMode = 9 : Statistics
*
* JLP
* Version 06-09-2015
****************************************************************************/

#ifndef jlp_decode_h_
#define jlp_decode_h_

#include "jlp_numeric.h"           // FFTW_COMPLEX
#include "asp2_typedef.h"          // MY_SELECT
#include "asp2_rw_config_files.h"  // Copy_PSET()
#include "asp2_defs.h"             // enum ChildIndex
#include "jlp_fitsio.h"            //
#include "asp2_3D_infits.h"           // inFITS_3DFile
#include "asp2_3D_outfits.h"          // outFITS_3DFile
#include "asp2_2dfits_utils.h"       // JLP_D_WRITEIMAG
#include "jlp_rw_fits0.h"          // writeimag_double_fits...
#include "jlp_camera_utils.h"      // JLP_CAMERA_SETTINGS, ...
#include "jlp_camera1.h"           // JLP_Camera1, ...

#ifndef PI
#define PI 3.14159
#endif
#ifndef ABS
#define ABS(a) ((a) >= 0 ? (a) : (-(a)))
#endif

//---------------------------------------------------------------------------

// KK= 6 (100 msec) for Merate
// KK= 3 (40 msec) for Nice
// For autoc-crossc: DATA_KK is the maximum difference between frames
// to compute the cross-correlation
#define DATA_KK 6

// To compute the cross-correlation (wind speed):
#define SCIDAR_CROSSC

#ifdef SCIDAR_CROSSC
// For crossc SCIDAR: difference of 20 msec
// #define DATA_KK_SCIDAR 2
// For crossc SCIDAR: difference of 40 msec
// JLP 2007: should be less or equal to DATA_KK !! (size constraint on data_old)
#define DATA_KK_SCIDAR 3
#endif

class JLP_Decode {

public:
    JLP_Decode(PROCESS_SETTINGS Pset0, JLP_CAMERA_SETTINGS CamSet0,
               DESCRIPTORS descrip0, const int nx0, const int ny0,
               wxString *str_messg, const int n_messg, const int ilang);
    ~JLP_Decode();
    int EraseProcessCube(const int confirm);
    int DC_LoadAndorCube(AWORD *AndorCube2, int nx2, int ny2, int nz2, int icube);
    int ReadDbleImage(double *dble_image2, const int nx2, const int ny2,
                      const int k_decode_index);
    int ReadOffsetFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadFlatFieldFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadUnresAutocFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadUnresModsqFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadPhotModsqFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadBispectrumFrame(double *dble_image2, const int nx2, const int ny2);
    int ReadRestoredImageFrame(double *dble_image2, const int nx2, const int ny2);

    int UpdateShortExp_v(AWORD *short_exposure2, int nx2, int ny2);
    int DC_LoadNewSettings(PROCESS_SETTINGS Pset, int nx, int ny);

// jlp_decode_cube_save.cpp
    int DC_Open_outFITS_3DFile();
    int SaveToCubeOfGoodFrames(AWORD *CubeImage, int nx1, int ny1,
                               int good_frame);
    int SaveCubeToFITSFile(AWORD* CubeOfGoodFrames, int nx, int ny,
                           int nz_CubeOfGoodFrames);
    int OutputFilesProcessCube(char *generic_name, char* directory,
                               char* comments);
    int Close_outFITS_3DFile_and_Save(char *generic_name, char *directory,
                               char *comments, DESCRIPTORS descrip,
                               bool expand_files);

// Load camera settings to Decod1 (used for writing FITS header):
    void DC_LoadAndorSettings(JLP_CAMERA_SETTINGS CamSet0) {
        Copy_CameraSettings(CamSet0, &CamSet1);
        }

    int DC_ImageFromBispectrum(double *xphot);

// asp2_Decode_access.cpp
    int DC_LoadProcessingResults(char* generic_name, bool bispectrum_was_computed);
    int DC_LoadLongInt(char *filename);
    int DC_LoadModsq(char *filename);
    int DC_LoadAutoc(char *filename);
    int DC_LoadQuadrant(char *filename);
    int DC_LoadBispectrum(char *filename);
    int DC_LoadOffsetFrame(char *filename);
    int DC_LoadFlatFieldFrame(char *filename);
    int DC_LoadPhotModsqFrame(char *filename);
    int DC_LoadUnresolvedModsqFrame(char *filename);
    int DC_LoadUnresolvedAutocFrame(char *filename);
    int ClipFlatField();

// In "jlp_decode_process_cube.cpp":
    int ProcessCube(int iCube);
    int ProcessCube_OnlyNumeri(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCube_OnlyInteg(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeBisp(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeBispAutoc(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeStatistics(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeSpectro(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeScidarLog(AWORD* CubeImage, int nx, int ny, int nz_cube);
    int ProcessCubeScidarLin(AWORD* CubeImage, int nx, int ny, int nz_cube);

// In "jlp_decode_process_cube_utils.cpp"
    int rotate_spectro_ima(double *in_1, double *out_2, int nx1, int ny1,
                       int nx2, int ny2);
    int update_cross_spectrum(FFTW_COMPLEX *data, double *data_old_Re[DATA_KK],
                                  double *data_old_Im[DATA_KK],
                                  double *CrossSpec_Re, double *CrossSpec_Im,
                                  int nx, int ny, int kk, int k_data_old);
    int update_modsq(FFTW_COMPLEX *data, double* modsq, int nx, int ny);
    int cross_spectrum_fourn(double *data, double* data_old[DATA_KK],
                                 double* CrossSpec_Re, double* CrossSpec_Im,
                                 double* modsq, int nx, int ny, int kk,
                                 int k_data_old);
    int rotate_data(FFTW_COMPLEX *data, double *data_old_Re[DATA_KK],
                        double *data_old_Im[DATA_KK],
                        int nx, int ny, int kk, int *k_data_old);
    int rotate_data_fourn(double *data, double* data_old[DATA_KK],
                              int nx, int ny, int kk, int *k_data_old);
    int ComputeStatistics();
    int RotateImage(AWORD *ima, int nx, int ny, int RotationMode);
    int MirrorImage(AWORD *ima, int nx, int ny, int MirrorMode);
    int OffsetAndFlatFieldCorrection(AWORD *ima, double *dble_ima,
                                     int nx, int ny);
    int seuillage_photon(AWORD* ima, int nx, int ny);
    int fwhm_by_modsq(FFTW_COMPLEX *data, int nx, int ny, float *fwhm);
    int jlp_fit_kolmogorof(double *ff, double *log_g, int npts,
                       float *aa, float *bb);

    int fwhm_aris(FFTW_COMPLEX *data, int nx, int ny, float *fwhm);
    int SelectFrame_with_fwhm(FFTW_COMPLEX *data, int nx, int ny, int *good_frame);
    int SelectFrame_with_MaxVal(double *frame, int nx, int ny, int *good_frame);
    int PreProcessFrame(AWORD* CubeImage, FFTW_COMPLEX *cplx_frame,
                        double *dble_frame, int iz, int *good_frame);
    int Check_if_frame_is_good(AWORD *im1, int nx1, int ny1);
    int BuildUpDirectVector(double *frame, int nx, int ny);

    int BuildUpLuckyImage(double *frame, FFTW_COMPLEX *inFFT, int nx, int ny,
                         const int iframe);

// Accessors:
    void Get_nframes(int *ngood_frames0, int *nbad_frames0,
                     int *nz0_CubeOfGoodFrames){
      *ngood_frames0 = ngood_frames;
      *nbad_frames0 = nbad_frames;
      *nz0_CubeOfGoodFrames = nz1_CubeOfGoodFrames;
      };
    void GetNxNyNz_cube(int *nx0, int *ny0, int *nz0_cube) {
        *nx0 = nx1; *ny0 = ny1; *nz0_cube = nz1_cube;
       }
    int GetProcessingMode() {return Pset1.ProcessingMode;}
    void GetInformation(double& PhotonsPerFrame1, int& ngood_frames1,
                        int& nbad_frames1, bool& Files_already_saved1) {
        PhotonsPerFrame1 = PhotonsPerFrame;
        ngood_frames1 = ngood_frames;
        nbad_frames1 = nbad_frames;
        Files_already_saved1 = Files_already_saved;
       }
    int GetMessageForStatusBar(wxString &message2);
    void Get_PSET(PROCESS_SETTINGS& Pset) {
        Copy_PSET(Pset1, Pset);
       }

protected:

    int DC_ExtractDirAndGeneric(char *filename, char *generic_name,
                                char *directory);
    int DC_AutocFromModsq(double *out_autoc, int nx, int ny);
    int DC_BispectrumSlice(double *output, int nx, int ny);
    int DC_LoadBispectrum(float *image1, int nx1, int ny1);
    int SetupProcessCube();
    int FreeMemory();

private:

int nbeta, ngamma, nx1, ny1, nz1_cube, nz1_CubeOfGoodFrames;
double lucky_x0, lucky_y0;  // center of first Lucky image
bool Files_already_saved, DecodeInitialized, CubeSavedToFile;
PROCESS_SETTINGS Pset1;
DESCRIPTORS descrip1;

// Messages in 5 languages:
int iLang;
wxString *m_messg;

JLP_CAMERA_SETTINGS CamSet1;
bool input_processed_frames;

// bool OnlyVisuMode;
JLP_3D_outFITSFile *outFITS_3DFile;
char out3DFITS_fname[256];
int ngood_frames, nbad_frames;
double PhotonsPerFrame, SigmaUnresolvedModsq;

AWORD *CurrentImage, *(CubeImage[2]), *CubeOfGoodFrames, *FFTImage;
FFTW_COMPLEX *cplx_data_sq, *cplx_data;
double *dble_data, *data_old_Re[DATA_KK], *data_old_Im[DATA_KK];
double *CrossSpec_Re, *CrossSpec_Im, *Quadrant_Re, *Quadrant_Im;
double *bispp, *snrm, *real_part, *imag_part, *work1, *ShortExp_v;
double *autocc, *autocc_centered_v, *quadr_centered_v, *crossc;
double *modsq, *modsq_centered_v, *long_int, *OffsetFrame;
double *UnresolvedModsqFrame, *UnresolvedAutocFrame, *RestoredImageFrame;
double *autocc_flattened, *FlatFieldFrame, *PhotModsqFrame;
double *BispectrumFrame;
double *scidar_lf, *scidar_func;
double *direct_vector, *lucky_image;

};

// Defined in "decode2.cpp" and accessible to all modules:
int SubtractProfile(double *ima0, const int nx0, const int ny0,
                    double *pro0, const int np0);
int ComputeCenterOfImage(double *frame, int nx, int ny, double &x0, double &y0);
int ShiftImage(double *inframe, FFTW_COMPLEX *inFFT, double *outframe,
               const int nx, const int ny, const double x0, const double y0);
int ComputeThresholdForDVA(double *inframe, const int nx, const int ny,
                           double &DVA_threshold);
int ComputeHistogram(double *inframe, const int nx, const int ny,
                     double *histo_nb, double *histo_val, const int nbins);
int Copy_JLP_Decode(JLP_Decode *jlp_decode0, JLP_Decode *jlp_decode1);

// Defined in jlp_decode_filter.cpp and accessible to all modules:

  int MedianProfileFilter(double *modsq_centered, double *autoc,
                          double *UnresolvedModsqFrame,
                          double SigmaUnresolvedModsq,
                          const int nx2, const int ny2);
  int HighContrastFilter(double *modsq_centered, double *autoc,
                         double *UnresolvedModsqFrame,
                         double SigmaUnresolvedModsq,
                         const int nx2, const int ny2);
  int UnsharpMaskingFilter(double *in_image1, double *out_image2,
                           const int nx2, const int ny2, const int iwidth);
  int ComputeMedianProfile(double *ima0, const int nx0, const int ny0,
                           double *pro0, const int np0);


#endif
