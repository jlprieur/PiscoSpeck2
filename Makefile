##################################################################
# Makefile for AndorSpeck2 program 
#
# JLP
# Version 07-01-2015
##################################################################

# To check it, type "echo %OS%" on Windows command terminal
ifeq ($(OS),Windows_NT)
JLP_SYSTEM=Windows
else
JLP_SYSTEM=Linux
endif

##############################################################
# Linux CENTOS6:
##############################################################
ifeq ($(JLP_SYSTEM),Linux)

CPP = c++

JLPLIB_DIR = $(JLPSRC)/jlplib
CFITSIO_INCL_DIR = $(JLPLIB_DIR)/cfitsio/incl

CXFLAGS = -Wall `wx-config --cppflags` $(MY_INCL) -DLINUX

mylib = $(JLPLIB)/jlp
MATHLIB = $(JLPLIB)/jlp/jlp_numeric.a $(JLPLIB)/math/libfftw3.a

PATHSEP1 = /

#WX_LIB = -L/usr/local/lib -pthread   -lwx_baseu-2.9 -lwx_gtk2u_core-2.9
WX_LIB := `wx-config --libs base,core`

else

##############################################################
# Windows operating system:
##############################################################

CPP = c++
JLPSRC = c:/jlp_src
JLPLIB_DIR = c:/jlp_src/jlplib
CFITSIO_INCL_DIR = $(JLPLIB_DIR)/cfitsio/incl

# C++ flags to use with wxWidgets code 
WX_INCLUDE_DIR = c:/wxWidgets-3.0.2/include

# Windows: I add USE_XPM (USE_XPM is needed to have the icon)
CXFLAGS = -Wall -DUSE_XPM -I$(WX_INCLUDE_DIR) $(MY_INCL) -DWIN32

# To avoid console apparition:
NO_CONSOLE = -mwindows 

# Need two back slashes for Linux compatibility:
PATHSEP1 = \\
# Not true: hence:
PATHSEP1 = /

EXEC = c:/EXEC

mylib = c:/EXEC/MyLib
MATHLIB = $(mylib)$(PATHSEP)jlp_numeric.a $(mylib)$(PATHSEP)libfftw3.a
WX_LIB_DIR = c:/wxWidgets-3.0.2/lib/gcc_dll

WX_LIB = -L$(WX_LIB_DIR) -lwxbase30u -lwxmsw30u_core

endif

#Remove extra blank before $(PATHSEP1):
PATHSEP=$(strip $(PATHSEP1))

EXEC_DIR = $(EXEC)$(PATHSEP)

MY_INCL=-I. -I$(JLPLIB_DIR)$(PATHSEP)jlp_splot  \
	-I$(JLPLIB_DIR)$(PATHSEP)jlp_fits \
	-I$(JLPLIB_DIR)$(PATHSEP)jlp_wxplot \
	-I$(JLPLIB_DIR)$(PATHSEP)jlp_numeric \
	-I$(CFITSIO_INCL_DIR)

FITSLIB=$(mylib)$(PATHSEP)jlp_fits.a $(mylib)$(PATHSEP)libcfitsio.a 
MY_LIB= $(mylib)$(PATHSEP)jlp_wxplot.a $(mylib)$(PATHSEP)jlp_splot.a  \
	$(MATHLIB) $(FITSLIB) -lm

FSP2_OBJ = asp2_main.o asp2_main_utils.o asp2_language.o \
	jlp_language_dlg.o \
	gdp_frame_video.o gdp_frame_logbook.o asp2_menu_rwfits.o \
	asp2_3Dfits.o asp2_fits_utils.o \
	jlp_rw_fits0.o jlp_fits0_cvb.o \
	asp2_frame_menu.o jlp_decode.o jlp_decode_access.o \
	jlp_decode_process_cube.o jlp_decode_process2.o \
	jlp_decode_cube_save.o jlp_decode_process_cube_utils.o \
	asp2_covermas.o asp2_photon_corr.o jlp_time0.o \
	asp2_inv_bispec2.o asp2_clean_deconv.o \
	jlp_andor_display_mutex.o jlp_display2_mutex.o \
	asp2_rw_config_files.o asp2_display_panel.o asp2_process.o \
	jlp_speckprocess_panel.o jlp_speckprocess_panel_onclick.o \
	jlp_speckprocess_panel_update.o \
	asp2_andor_thread.o asp2_andor_thread_display.o \
	asp2_decode_thread.o asp2_3dfits_thread.o  

FSP2_SRC = $(FSP2_OBJ)(.o:.cpp)
 
FSP2_DEP = AndorSpeck2.h asp2_frame.h asp2_frame_id.h  \
	jlp_fits0_computer.h jlp_fits0_fmt.h \
	asp2_3Dfits.h asp2_fits_utils.h \
	asp2_covermas.h jlp_decode.h asp2_defs.h \
	asp2_photon_corr.h jlp_time0.h \
	asp2_typedef.h jlp_andor_display_mutex.h jlp_display2_mutex.h \
	asp2_rw_config_files.h jlp_speckprocess_panel.h \
	asp2_andor_thread.h asp2_decode_thread.h asp2_3dfits_thread.h \
	asp2_display_panel.h

ANDOR_OBJ = jlp_andor_cam1.o jlp_andor_cam1_set.o \
	jlp_andor_cam1_acqui.o jlp_andor_cam1_acqui_conti.o jlp_andor_utils.o \
	jlp_andor_panel.o jlp_andor_panel_update.o \
	jlp_andor_panel_onclick1.o jlp_andor_panel_onclick2.o \
	jlp_andor_display.o jlp_Atmcd32d_linux.o

ANDOR_SRC = $(ANDOR_OBJ)(.o:.cpp)

ANDOR_DEP = jlp_andor_cam1.h jlp_andor_panel.h jlp_Atmcd32d_linux.h

.SUFFIXES:
.SUFFIXES: .o .cpp .exe $(SUFFIXES) 

.cpp.o:
	$(CPP) -c -g $(CXFLAGS) $*.cpp

.o.exe:
	$(CPP) -o $(EXEC_DIR)$*.exe $*.o $(FSP2_OBJ) $(ANDOR_OBJ) \
	$(MY_LIB) $(WX_LIB)

.cpp.exe:
	$(CPP) -c -g $(CXFLAGS) $*.cpp
	$(CPP) -o $(EXEC_DIR)$*.exe $*.o $(FSP2_OBJ) $(ANDOR_OBJ) \
	$(MY_LIB) $(WX_LIB)

### Targets: ###

all: $(FSP2_OBJ) $(ANDOR_OBJ) AndorSpeck2.exe

Debug: $(FSP2_OBJ) $(ANDOR_OBJ) AndorSpeck2.exe

AndorSpeck2.o: AndorSpeck2.cpp $(FSP2_DEP)

asp2_main.o: asp2_main.cpp $(FSP2_DEP)

asp2_main_utils.o: asp2_main_utils.cpp $(FSP2_DEP)

gdp_frame_logbook.o: gdp_frame_logbook.cpp  $(FSP2_DEP)

gdp_frame_video.o: gdp_frame_video.cpp  $(FSP2_DEP)

asp2_frame_menu.o: asp2_frame_menu.cpp  $(FSP2_DEP)

asp2_display_panel.o: asp2_display_panel.cpp  $(FSP2_DEP)

asp2_process.o: asp2_process.cpp  $(FSP2_DEP)

asp2_menu_rwfits.o: asp2_menu_rwfits.cpp  $(FSP2_DEP)

asp2_3Dfits.o: asp2_3Dfits.cpp  $(FSP2_DEP)

jlp_fits0_cvb.o: jlp_fits0_cvb.cpp  $(FSP2_DEP)

jlp_rw_fits0.o: jlp_rw_fits0.cpp  $(FSP2_DEP)

asp2_fits_utils.o: asp2_fits_utils.cpp  $(FSP2_DEP)

jlp_decode.o: jlp_decode.cpp  $(FSP2_DEP)

jlp_decode_process_cube.o: jlp_decode_process_cube.cpp  $(FSP2_DEP)

jlp_decode_process_cube_utils.o: jlp_decode_process_cube_utils.cpp  $(FSP2_DEP)

jlp_decode_process2.o: jlp_decode_process2.cpp  $(FSP2_DEP)

jlp_decode_access.o: jlp_decode_access.cpp  $(FSP2_DEP)

asp2_covermas.o: asp2_covermas.cpp  $(FSP2_DEP)

asp2_photon_corr.o: asp2_photon_corr.cpp  $(FSP2_DEP)

asp2_inv_bispec2.o : asp2_inv_bispec2.cpp $(FSP2_DEP)

asp2_clean_deconv.o : asp2_clean_deconv.cpp $(FSP2_DEP)

jlp_display1_mutex.o : jlp_display1_mutex.cpp $(FSP2_DEP)

jlp_display2_mutex.o : jlp_display2_mutex.cpp $(FSP2_DEP)

asp2_rw_config_files.o : asp2_rw_config_files.cpp $(FSP2_DEP)

jlp_speckprocess_panel.o : jlp_speckprocess_panel.cpp $(FSP2_DEP)

jlp_speckprocess_panel_onclick.o : jlp_speckprocess_panel_onclick.cpp $(FSP2_DEP)

jlp_speckprocess_panel_update.o : jlp_speckprocess_panel_update.cpp $(FSP2_DEP)

jlp_andor_panel.o : jlp_andor_panel.cpp $(ANDOR_DEP)

jlp_andor_panel_update.o : jlp_andor_panel_update.cpp $(ANDOR_DEP)

jlp_andor_panel_onclick1.o : jlp_andor_panel_onclick1.cpp $(ANDOR_DEP)

jlp_andor_panel_onclick2.o : jlp_andor_panel_onclick2.cpp $(ANDOR_DEP)

jlp_andor_cam1.o : jlp_andor_cam1.cpp $(ANDOR_DEP)

jlp_andor_cam2.o : jlp_andor_cam2.cpp $(ANDOR_DEP)

jlp_andor_utils.o : jlp_andor_utils.cpp $(ANDOR_DEP)

jlp_Atmcd32d_linux.o : jlp_Atmcd32d_linux.cpp 

jlp_time0.o: jlp_time0.cpp 

########################################################
# Linux: 
ifeq ($(JLP_SYSTEM),Linux)

AndorSpeck2.exe: AndorSpeck2.cpp  $(FSP2_DEP)

else
########################################################
# Windows:
AndorSpeck2_rc.o: AndorSpeck2.rc
	windres --use-temp-file --define USE_XPM -I$(WX_INCLUDE_DIR) AndorSpeck2.rc \
	-o AndorSpeck2_rc.o --define __WXMSW__ --define NOPCH

AndorSpeck2.exe: $(FSP2_OBJ) AndorSpeck2.cpp AndorSpeck2_rc.o
	$(CPP) $(CXFLAGS) $(NO_CONSOLE) -o $(EXEC_DIR)AndorSpeck2.exe \
	AndorSpeck2.cpp AndorSpeck2_rc.o $(FSP2_OBJ) $(ANDOR_OBJ) $(MY_LIB) \
	$(WX_LIB) atmcd32m.lib -lm 
# Missing DLL's:
# libgcc_s_dw2-1.dll, libstdc++-6.dll 
# I also tried adding the linker options: -static-libgcc -static-libstdc++
# but it did not work.
########################################################
endif

clear: clean

clean: 
	rm -f AndorSpeck2_rc.o AndorSpeck2.o $(FSP2_OBJ) $(ANDOR_OBJ) 
