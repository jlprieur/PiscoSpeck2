/******************************************************************************
* Set of routines to read elementary frames from 2D FITS files
*
* JLP Version
* Version 27/08/2015
******************************************************************************/
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <wx/filedlg.h>  // wxFileDialog
#include <wx/arrstr.h>  // wxArrayString

#include "jlp_time0.h"
#include "jlp_rw_fits0.h"

#include "asp2_2Dfits.h"
#include "asp2_fits_utils.h"

/******************************************************************************
* Open a series of 2D FITS files
*
* INPUT:
*  generic_name
*
* OUTPUT:
*  **FileName: array of filenames (without the directory)
*  *FileDirectory: directory
*  nx, ny, nz_total, err_mess
*  fp_FITS_cube: pointer to the FITS cube file
******************************************************************************/
int open_input_2DFitsFiles(char *GenericName, char *FileDirectory,
                           wxString **FileName, int *nx, int *ny, int *nz_total,
                           char *errmess)
{
int status = 0, nz_maxi = 20000;
register int i, k;
char filename[128], comments[80];
wxFileDialog *m_dialog;
wxArrayString found_filenames;
AWORD *awd_image;

found_filenames.Alloc(nz_maxi);

// Go twice on this loop: firstly to determine the number of files,
// and secondly to load the filenames:
*nz_total = 0;

// wxFileDialog(parent, message, default dir, default filename, wildcard, flag) :
  m_dialog = new wxFileDialog(NULL, _T("Select image FITS files"), 
                            wxString(FileDirectory), _T(""),
                            _T("FITS files (*.fits;*.fit)|*.fits;*.fit"),
                            wxFD_MULTIPLE);
  if(m_dialog->ShowModal() != wxID_OK) {
    sprintf(errmess, "No FITS files found in >%s< !\n", FileDirectory);
    return(-1);
  } else {
    m_dialog->GetFilenames(found_filenames);
    if(found_filenames.IsEmpty()) {
     sprintf(errmess, "No FITS files found in >%s< !\n", FileDirectory);
     return(-1);
    }
    *nz_total = found_filenames.GetCount();
  }

// Transfer filenames to array:
*FileName = new wxString[*nz_total];
for(i = 0; i < *nz_total; i++) (*FileName)[i] = found_filenames.Item(i); 

#ifdef DEBUG
sprintf(errmess, "nfiles=%d 0=>%s< \n 1=>%s< \n 2=>%s<\n", *nz_total,
        (*FileName)[0].c_str(), (*FileName)[1].c_str(), (*FileName)[2].c_str());
wxMessageBox(wxString(errmess), wxT("open_input_2DFitsFiles"),
             wxICON_INFORMATION | wxOK );
#endif

// Open first file to read nx, ny:
  strcpy(filename, (*FileName[0]).c_str());
  status = readimag_aword_fits(filename, &awd_image, nx, ny, comments,
                               errmess);
  if(status) {
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
  }
  delete[] awd_image;

#ifdef DEBUG
sprintf(errmess, "file=%s nz_total=%d nx=%d ny=%d\n", filename,
        *nz_total, *nx, *ny);
wxMessageBox(wxString(errmess), wxT("open_input_2DFitsFiles"),
                   wxICON_INFORMATION | wxOK );
#endif

if(*nz_total < 50) {
  sprintf(errmess, "Error too few files: nz_total=%d < 50 (nx=%d ny=%d)\n",
          *nz_total, *nx, *ny);
  status = 1;
  }

return(status);
}
/******************************************************************************
* Read BYTE data from FITS 2D files
*
* INPUT:
*  FileName: string array of filenames
*  nz_cube: number of files to be read from the 2D FITS file set
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_byte_cube_from_2DFitsFiles(wxString *FileName, BYTE *image,
                                    int nx, int ny, int nz_cube,
                                    int *current_file_nz,
                                    int total_file_nz, int *all_read)
{
int status, nx0, ny0;
register int i, k;
AWORD *awd_image;
char errmess[256], filename[128], comments[80];

if((total_file_nz - *current_file_nz) < nz_cube) {
 *all_read = 1;
 return(10);
 }

 *all_read = 0;

for(k = 0; k < nz_cube; k++) {
  strcpy(filename, FileName[k].c_str());
  status = readimag_aword_fits(filename, &awd_image, &nx0, &ny0, comments,
                               errmess);
    if(status) {
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] awd_image;
      return(-1);
    }
// Check size:
    if(nx0 != nx || ny0 != ny) {
      sprintf(errmess, "Incompatible size/cube: nx=%d ny=%d (image: nx=%d ny=%d)!",
              nx, ny, nx0, ny0);
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] awd_image;
      return(-1);
    }

  for (i = 0; i < nx * ny; i++) {
    image[i + k * nx * ny] = (BYTE)awd_image[i];
    }

}

  delete[] awd_image;

*current_file_nz += nz_cube;

return(0);
}
/******************************************************************************
* Read AWORD data from FITS 2D files
*
* INPUT:
*  FileName: string array of filenames
*  nz_cube: number of files to be read from the 2D FITS file set
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_aword_cube_from_2DFitsFiles(wxString *FileName, AWORD *image,
                                     int nx, int ny, int nz_cube,
                                     int *current_file_nz,
                                     int total_file_nz, int *all_read)
{
int status, nx0, ny0;
register int i, k;
float *flt_image;
char errmess[256], filename[128], comments[80];

if((total_file_nz - *current_file_nz) < nz_cube) {
 *all_read = 1;
 return(10);
 }

 *all_read = 0;

for(k = 0; k < nz_cube; k++) {
  strcpy(filename, FileName[k].c_str());
  status = readimag_float_fits(filename, &flt_image, &nx0, &ny0, comments,
                               errmess);
    if(status) {
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] flt_image;
      return(-1);
    }
// Check size:
    if(nx0 != nx || ny0 != ny) {
      sprintf(errmess, "Incompatible size/cube: nx=%d ny=%d (image: nx=%d ny=%d)!",
              nx, ny, nx0, ny0);
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] flt_image;
      return(-1);
    }

  for (i = 0; i < nx * ny; i++) {
    image[i + k * nx * ny] = (AWORD)flt_image[i];
    }

}

  delete[] flt_image;

*current_file_nz += nz_cube;

return(0);
}
/******************************************************************************
* Read float data from FITS 2D files
*
* INPUT:
*  FileName: string array of filenames
*  nz_cube: number of files to be read from the 2D FITS file set
*
* OUTPUT:
*  image: 3D data to be written to output file
*
* INPUT/OUTPUT:
*  current_file_nz
******************************************************************************/
int read_float_cube_from_2DFITSFiles(wxString *FileName, float *image,
                                     int nx, int ny, int nz_cube,
                                     int *current_file_nz,
                                     int total_file_nz, int *all_read)
{
int status, nx0, ny0;
register int i, k;
float *flt_image;
char errmess[256], filename[128], comments[80];

if((total_file_nz - *current_file_nz) < nz_cube) {
 *all_read = 1;
 return(10);
 }

 *all_read = 0;

for(k = 0; k < nz_cube; k++) {
  strcpy(filename, FileName[k].c_str());
  status = readimag_float_fits(filename, &flt_image, &nx0, &ny0, comments,
                               errmess);
    if(status) {
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] flt_image;
      return(-1);
    }
// Check size:
    if(nx0 != nx || ny0 != ny) {
      sprintf(errmess, "Incompatible size/cube: nx=%d ny=%d (image: nx=%d ny=%d)!",
              nx, ny, nx0, ny0);
      wxMessageBox(wxString(errmess), wxT("read_byte_cube_from_2DFitsFiles"),
                   wxICON_ERROR);
      delete[] flt_image;
      return(-1);
    }

  for (i = 0; i < nx * ny; i++) {
    image[i + k * nx * ny] = flt_image[i];
    }

}

delete[] flt_image;

*current_file_nz += nz_cube;

return(0);
}
