//                                                         
//   XCLIBWNT_LABVIEW.H  External     27-Apr-2017    
//   Copyright (C)  2007-2017   EPIX, Inc.  All rights reserved.
//                                                          
//   XCLIB Declarations for LabView 32-Bit                  
//                                                          

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_PIXCIopen(const char*driverparms,const char*formatname,const char*formatfile);
int __stdcall pxd_PIXCIopen(char* driverparms, char* formatname, char* formatfile);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_PIXCIclose(void);
int __stdcall pxd_PIXCIclose();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_mesgFault(int unitmap);
int __stdcall pxd_mesgFault(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,char*)pxd_mesgErrorCode(int err);
char* __stdcall pxd_mesgErrorCode(int err);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_mesgFaultText(int unitmap,char buf[],size_t bufsize);
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxd_mesgFaultText(int unitmap, char buf[], int bufsize);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_mesgFaultText(int unitmap,char*buf,size_t bufsize);
// Duplicate declaration

// _cDcl(__declspec(dllexport),__stdcall,pxdstate_s*)pxe_XCLIBinstantiate(void);
void* __stdcall pxe_XCLIBinstantiate();

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_XCLIBuninstantiate(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void __stdcall pxe_XCLIBuninstantiate(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_PIXCIopen(pxdstate_s*,const char*driverparms,const char*formatname,const char*formatfile);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_PIXCIopen(void* , char* driverparms, char* formatname, char* formatfile);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_PIXCIclose(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_PIXCIclose(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_mesgFault(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_mesgFault(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_mesgFaultText(pxdstate_s*,int unitmap,char buf[],size_t bufsize);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxe_mesgFaultText(void* , int unitmap, char buf[], int bufsize);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_mesgFaultText(pxdstate_s*,int unitmap,char*buf,size_t bufsize);
// Duplicate declaration

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_infoModel(int unitmap);
int __stdcall pxd_infoModel(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_infoSubmodel(int unitmap);
int __stdcall pxd_infoSubmodel(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_infoUnits(void);
int __stdcall pxd_infoUnits();

// _cDcl(__declspec(dllexport),__stdcall,char*)pxd_infoDriverId(void);
char* __stdcall pxd_infoDriverId();

// _cDcl(__declspec(dllexport),__stdcall,char*)pxd_infoLibraryId(void);
char* __stdcall pxd_infoLibraryId();

// _cDcl(__declspec(dllexport),__stdcall,ulong)pxd_infoMemsize(int unitmap);
long __stdcall pxd_infoMemsize(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_infoSysTicksUnits(uint32 ticku[2]);
// The `ticku' parameter has been declared expecting a dimensioned int array w. 2 elements to be passed
int __stdcall pxd_infoSysTicksUnits(unsigned int ticku[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_infoSysTicks(uint32 ticks[2]);
// The `ticks' parameter has been declared expecting a dimensioned int array w. 2 elements to be passed
int __stdcall pxd_infoSysTicks(unsigned int ticks[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_infoModel(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_infoModel(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_infoSubmodel(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_infoSubmodel(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_infoUnits(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_infoUnits(void* );

// _cDcl(__declspec(dllexport),__stdcall,char*)pxe_infoDriverId(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
char* __stdcall pxe_infoDriverId(void* );

// _cDcl(__declspec(dllexport),__stdcall,char*)pxe_infoLibraryId(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
char* __stdcall pxe_infoLibraryId(void* );

// _cDcl(__declspec(dllexport),__stdcall,ulong)pxe_infoMemsize(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
long __stdcall pxe_infoMemsize(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_infoSysTicksUnits(pxdstate_s*,uint32 ticku[2]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `ticku' parameter has been declared expecting a dimensioned int array to be passed
int __stdcall pxe_infoSysTicksUnits(void* , unsigned int ticku[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_infoSysTicks(pxdstate_s*,uint32 ticks[2]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `ticks' parameter has been declared expecting a dimensioned int array to be passed
int __stdcall pxe_infoSysTicks(void* , unsigned int ticks[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageXdim(void);
int __stdcall pxd_imageXdim();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageYdim(void);
int __stdcall pxd_imageYdim();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageCdim(void);
int __stdcall pxd_imageCdim();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageBdim(void);
int __stdcall pxd_imageBdim();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageZdim(void);
int __stdcall pxd_imageZdim();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageIdim(void);
int __stdcall pxd_imageIdim();

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_imageAspectRatio(void);
double __stdcall pxd_imageAspectRatio();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageXdims(int unitmap);
int __stdcall pxd_imageXdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageYdims(int unitmap);
int __stdcall pxd_imageYdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageCdims(int unitmap);
int __stdcall pxd_imageCdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageBdims(int unitmap);
int __stdcall pxd_imageBdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageZdims(int unitmap);
int __stdcall pxd_imageZdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_imageIdims(int unitmap);
int __stdcall pxd_imageIdims(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_imageAspectRatios(int unitmap);
double __stdcall pxd_imageAspectRatios(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageXdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageXdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageYdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageYdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageCdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageCdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageBdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageBdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageZdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageZdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageIdim(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageIdim(void* );

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_imageAspectRatio(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_imageAspectRatio(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageXdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageXdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageYdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageYdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageCdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageCdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageBdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageBdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageZdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageZdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_imageIdims(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_imageIdims(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_imageAspectRatios(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_imageAspectRatios(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_readuchar(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,uchar*membuf,size_t cnt,char*colorspace);
// The `membuf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxd_readuchar(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned char* membuf, 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_writeuchar(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,uchar*membuf,size_t cnt,char*colorspace);
// The `membuf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxd_writeuchar(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned char* membuf, 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_readushort(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,ushort*membuf,size_t cnt,char*colorspace);
// The `membuf' parameter has been declared expecting a dimensioned short array to be passed
int __stdcall pxd_readushort(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned short membuf[], 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_writeushort(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,ushort*membuf,size_t cnt,char*colorspace);
// The `membuf' parameter has been declared expecting a dimensioned short array to be passed
int __stdcall pxd_writeushort(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned short membuf[], 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage_s*)pxd_defineImage(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
void* __stdcall pxd_defineImage(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage3_s*)pxd_defineImage3(int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
void* __stdcall pxd_defineImage3(int unitmap, long startbuf, long endbuf, int ulx, int uly, int lrx, 
int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage_s*)pxd_definePximage(int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
void* __stdcall pxd_definePximage(int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage3_s*)pxd_definePximage3(int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
void* __stdcall pxd_definePximage3(int unitmap, long startbuf, long endbuf, int ulx, int uly, int lrx, 
int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_definePximageFree(pximage_s*);
void __stdcall pxd_definePximageFree(void* );

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_definePximage3Free(pximage3_s*);
void __stdcall pxd_definePximage3Free(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_readuchar(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,uchar*membuf,size_t cnt,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `membuf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxe_readuchar(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned char* membuf, 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_writeuchar(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,uchar*membuf,size_t cnt,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `membuf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxe_writeuchar(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned char* membuf, 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_readushort(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,ushort*membuf,size_t cnt,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `membuf' parameter has been declared expecting a dimensioned short array to be passed
int __stdcall pxe_readushort(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned short membuf[], 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_writeushort(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,ushort*membuf,size_t cnt,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `membuf' parameter has been declared expecting a dimensioned short array to be passed
int __stdcall pxe_writeushort(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, unsigned short membuf[], 
int cnt, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage_s*)pxe_defineImage(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_defineImage(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, 
char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage3_s*)pxe_defineImage3(pxdstate_s*,int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_defineImage3(void* , int unitmap, long startbuf, long endbuf, int ulx, int uly, int lrx, 
int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage_s*)pxe_definePximage(pxdstate_s*,int unitmap,pxbuffer_t framebuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_definePximage(void* , int unitmap, long framebuf, int ulx, int uly, int lrx, int lry, 
char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,pximage3_s*)pxe_definePximage3(pxdstate_s*,int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,char*colorspace);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_definePximage3(void* , int unitmap, long startbuf, long endbuf, int ulx, int uly, 
int lrx, int lry, char* colorspace);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_definePximageFree(pxdstate_s*,pximage_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void __stdcall pxe_definePximageFree(void* , void* );

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_definePximage3Free(pxdstate_s*,pximage3_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void __stdcall pxe_definePximage3Free(void* , void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_doSnap(int unitmap,pxbuffer_t buffer,ulong timeout);
int __stdcall pxd_doSnap(int unitmap, long buffer, long timeout);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goSnap(int unitmap,pxbuffer_t buffer);
int __stdcall pxd_goSnap(int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goSnapPair(int unitmap,pxbuffer_t buffer1,pxbuffer_t buffer2);
int __stdcall pxd_goSnapPair(int unitmap, long buffer1, long buffer2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goLive(int unitmap,pxbuffer_t buffer);
int __stdcall pxd_goLive(int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goUnLive(int unitmap);
int __stdcall pxd_goUnLive(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goAbortLive(int unitmap);
int __stdcall pxd_goAbortLive(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goLivePair(int unitmap,pxbuffer_t buffer1,pxbuffer_t buffer2);
int __stdcall pxd_goLivePair(int unitmap, long buffer1, long buffer2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goLiveSeq(int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxbuffer_t incbuf,pxbuffer_t numbuf,int period);
int __stdcall pxd_goLiveSeq(int unitmap, long startbuf, long endbuf, long incbuf, long numbuf, int period);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goLiveTrig(int unitmap,pxbuffer_t buffer,uint gpin10mask,uint gpout20value,uint gpout20mask,uint gpout20when,
// uint gpin30wait,uint gpin30mask,uint gpout40value,uint gpout40mask,uint option50,uint field50,
// uint gpout50value,uint gpout50mask,uint delay60,uint gpout60value,uint gpout60mask,uint delay70,
// uint field70,uint capture70,uint gpin80mask,uint gpout80value,uint gpout80mask);
int __stdcall pxd_goLiveTrig(int unitmap, long buffer, int gpin10mask, int gpout20value, int gpout20mask, 
int gpout20when, int gpin30wait, int gpin30mask, int gpout40value, int gpout40mask, int option50, 
int field50, int gpout50value, int gpout50mask, int delay60, int gpout60value, int gpout60mask, int delay70, 
int field70, int capture70, int gpin80mask, int gpout80value, int gpout80mask);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goLiveSeqTrig(int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxbuffer_t incbuf,pxbuffer_t numbuf,int period,
// uint rsvd1,uint rsvd2,uint trig20wait,uint trig20slct,pxvbtime_t trig20delay,uint rsvd3,uint rsvd4,
// uint rsvd5,uint rsvd6,pxvbtime_t rsvd7,uint rsvd8,uint rsvd9,uint trig40wait,uint trig40slct,
// pxvbtime_t trig40delay,uint rsvd10,uint rsvd11,uint rsvd12,uint rsvd13,uint rsvd14,uint rsvd15);
int __stdcall pxd_goLiveSeqTrig(int unitmap, long startbuf, long endbuf, long incbuf, long numbuf, 
int period, int rsvd1, int rsvd2, int trig20wait, int trig20slct, long trig20delay, int rsvd3, int rsvd4, 
int rsvd5, int rsvd6, long rsvd7, int rsvd8, int rsvd9, int trig40wait, int trig40slct, long trig40delay, 
int rsvd10, int rsvd11, int rsvd12, int rsvd13, int rsvd14, int rsvd15);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_goneLive(int unitmap,int rsvd);
int __stdcall pxd_goneLive(int unitmap, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_videoFieldsPerFrame(void);
int __stdcall pxd_videoFieldsPerFrame();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_videoFieldsPerFrames(int unitmap);
int __stdcall pxd_videoFieldsPerFrames(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxd_videoFieldCount(int unitmap);
long __stdcall pxd_videoFieldCount(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxbuffer_t)pxd_capturedBuffer(int unitmap);
long __stdcall pxd_capturedBuffer(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxd_capturedSysTicks(int unitmap);
int __stdcall pxd_capturedSysTicks(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxd_capturedSysTicksHi(int unitmap);
int __stdcall pxd_capturedSysTicksHi(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxd_capturedFieldCount(int unitmap);
long __stdcall pxd_capturedFieldCount(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxd_buffersFieldCount(int unitmap,pxbuffer_t buffer);
long __stdcall pxd_buffersFieldCount(int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxd_buffersSysTicks(int unitmap,pxbuffer_t buffer);
int __stdcall pxd_buffersSysTicks(int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_buffersSysTicks2(int unitmap,pxbuffer_t buffer,uint32 ticks[2]);
// The `ticks' parameter has been declared expecting a dimensioned int array w. 2 elements to be passed
int __stdcall pxd_buffersSysTicks2(int unitmap, long buffer, unsigned int ticks[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_buffersGPIn(int unitmap,pxbuffer_t buffer);
int __stdcall pxd_buffersGPIn(int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_doSnap(pxdstate_s*,int unitmap,pxbuffer_t buffer,ulong timeout);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_doSnap(void* , int unitmap, long buffer, long timeout);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goSnap(pxdstate_s*,int unitmap,pxbuffer_t buffer);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goSnap(void* , int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goSnapPair(pxdstate_s*,int unitmap,pxbuffer_t buffer1,pxbuffer_t buffer2);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goSnapPair(void* , int unitmap, long buffer1, long buffer2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goLive(pxdstate_s*,int unitmap,pxbuffer_t buffer);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goLive(void* , int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goUnLive(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goUnLive(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goAbortLive(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goAbortLive(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goLivePair(pxdstate_s*,int unitmap,pxbuffer_t buffer1,pxbuffer_t buffer2);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goLivePair(void* , int unitmap, long buffer1, long buffer2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goLiveSeq(pxdstate_s*,int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxbuffer_t incbuf,pxbuffer_t numbuf,int period);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goLiveSeq(void* , int unitmap, long startbuf, long endbuf, long incbuf, long numbuf, int period);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goLiveTrig(pxdstate_s*,int unitmap,pxbuffer_t buffer,uint gpin10mask,uint gpout20value,uint gpout20mask,uint gpout20when,
// uint gpin30wait,uint gpin30mask,uint gpout40value,uint gpout40mask,uint option50,uint field50,
// uint gpout50value,uint gpout50mask,uint delay60,uint gpout60value,uint gpout60mask,uint delay70,
// uint field70,uint capture70,uint gpin80mask,uint gpout80value,uint gpout80mask);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goLiveTrig(void* , int unitmap, long buffer, int gpin10mask, int gpout20value, int gpout20mask, 
int gpout20when, int gpin30wait, int gpin30mask, int gpout40value, int gpout40mask, int option50, 
int field50, int gpout50value, int gpout50mask, int delay60, int gpout60value, int gpout60mask, int delay70, 
int field70, int capture70, int gpin80mask, int gpout80value, int gpout80mask);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goLiveSeqTrig(pxdstate_s*,int unitmap,pxbuffer_t startbuf,pxbuffer_t endbuf,pxbuffer_t incbuf,pxbuffer_t numbuf,int period,
// uint rsvd1,uint rsvd2,uint trig20wait,uint trig20slct,pxvbtime_t trig20delay,uint rsvd3,uint rsvd4,
// uint rsvd5,uint rsvd6,pxvbtime_t rsvd7,uint rsvd8,uint rsvd9,uint trig40wait,uint trig40slct,
// pxvbtime_t trig40delay,uint rsvd10,uint rsvd11,uint rsvd12,uint rsvd13,uint rsvd14,uint rsvd15);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goLiveSeqTrig(void* , int unitmap, long startbuf, long endbuf, long incbuf, long numbuf, 
int period, int rsvd1, int rsvd2, int trig20wait, int trig20slct, long trig20delay, int rsvd3, int rsvd4, 
int rsvd5, int rsvd6, long rsvd7, int rsvd8, int rsvd9, int trig40wait, int trig40slct, long trig40delay, 
int rsvd10, int rsvd11, int rsvd12, int rsvd13, int rsvd14, int rsvd15);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_goneLive(pxdstate_s*,int unitmap,int rsvd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_goneLive(void* , int unitmap, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_videoFieldsPerFrame(pxdstate_s*);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_videoFieldsPerFrame(void* );

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_videoFieldsPerFrames(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_videoFieldsPerFrames(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxe_videoFieldCount(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
long __stdcall pxe_videoFieldCount(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxbuffer_t)pxe_capturedBuffer(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
long __stdcall pxe_capturedBuffer(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxe_capturedSysTicks(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_capturedSysTicks(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxe_capturedSysTicksHi(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_capturedSysTicksHi(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxe_capturedFieldCount(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
long __stdcall pxe_capturedFieldCount(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,pxvbtime_t)pxe_buffersFieldCount(pxdstate_s*,int unitmap,pxbuffer_t buffer);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
long __stdcall pxe_buffersFieldCount(void* , int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,uint32)pxe_buffersSysTicks(pxdstate_s*,int unitmap,pxbuffer_t buffer);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_buffersSysTicks(void* , int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_buffersSysTicks2(pxdstate_s*,int unitmap,pxbuffer_t buffer,uint32 ticks[2]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `ticks' parameter has been declared expecting a dimensioned int array w. 2 elements to be passed
int __stdcall pxe_buffersSysTicks2(void* , int unitmap, long buffer, unsigned int ticks[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_buffersGPIn(pxdstate_s*,int unitmap,pxbuffer_t buffer);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_buffersGPIn(void* , int unitmap, long buffer);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setVidMux(int unitmap,int inmux);
int __stdcall pxd_setVidMux(int unitmap, int inmux);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getVidMux(int unitmap);
int __stdcall pxd_getVidMux(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setContrastBrightness(int unitmap,double contrast,double brightness);
int __stdcall pxd_setContrastBrightness(int unitmap, double contrast, double brightness);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setHueSaturation(int unitmap,double hue,double Ugain,double Vgain);
int __stdcall pxd_setHueSaturation(int unitmap, double hue, double Ugain, double Vgain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getContrast(int unitmap);
double __stdcall pxd_getContrast(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getBrightness(int unitmap);
double __stdcall pxd_getBrightness(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getHue(int unitmap);
double __stdcall pxd_getHue(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getUGain(int unitmap);
double __stdcall pxd_getUGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getVGain(int unitmap);
double __stdcall pxd_getVGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setVidMux(pxdstate_s*,int unitmap,int inmux);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setVidMux(void* , int unitmap, int inmux);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getVidMux(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getVidMux(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setContrastBrightness(pxdstate_s*,int unitmap,double contrast,double brightness);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setContrastBrightness(void* , int unitmap, double contrast, double brightness);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setHueSaturation(pxdstate_s*,int unitmap,double hue,double Ugain,double Vgain);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setHueSaturation(void* , int unitmap, double hue, double Ugain, double Vgain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getContrast(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getContrast(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getBrightness(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getBrightness(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getHue(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getHue(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getUGain(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getUGain(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getVGain(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getVGain(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setAdcGainOffset(int unitmap,int rsvd,double gainA,double offsetA,double gainB,double offsetB);
int __stdcall pxd_setAdcGainOffset(int unitmap, int rsvd, double gainA, double offsetA, double gainB, double offsetB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setAdcColorGainOffsets(int unitmap,int rsvd,double gainsA[4],double gainsB[4],double offsetsA[4],double offsetsB[4]);
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxd_setAdcColorGainOffsets(int unitmap, int rsvd, double gainsA[], double gainsB[], 
double offsetsA[], double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getAdcGainA(int unitmap);
double __stdcall pxd_getAdcGainA(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getAdcGainB(int unitmap);
double __stdcall pxd_getAdcGainB(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getAdcGainsA(int unitmap,double gainsA[4]);
// The `gainsA' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_getAdcGainsA(int unitmap, double gainsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getAdcGainsB(int unitmap,double gainsB[4]);
// The `gainsB' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_getAdcGainsB(int unitmap, double gainsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getAdcOffsetsA(int unitmap,double offsetsA[4]);
// The `offsetsA' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_getAdcOffsetsA(int unitmap, double offsetsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getAdcOffsetsB(int unitmap,double offsetsB[4]);
// The `offsetsB' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_getAdcOffsetsB(int unitmap, double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getAdcOffsetA(int unitmap);
double __stdcall pxd_getAdcOffsetA(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_getAdcOffsetB(int unitmap);
double __stdcall pxd_getAdcOffsetB(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setAdcGainOffset(pxdstate_s*,int unitmap,int rsvd,double gainA,double offsetA,double gainB,double offsetB);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setAdcGainOffset(void* , int unitmap, int rsvd, double gainA, double offsetA, double gainB, 
double offsetB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setAdcColorGainOffsets(pxdstate_s*,int unitmap,int rsvd,double gainsA[4],double gainsB[4],double offsetsA[4],double offsetsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_setAdcColorGainOffsets(void* , int unitmap, int rsvd, double gainsA[], double gainsB[], 
double offsetsA[], double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getAdcGainA(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getAdcGainA(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getAdcGainB(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getAdcGainB(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getAdcGainsA(pxdstate_s*,int unitmap,double gainsA[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_getAdcGainsA(void* , int unitmap, double gainsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getAdcGainsB(pxdstate_s*,int unitmap,double gainsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_getAdcGainsB(void* , int unitmap, double gainsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getAdcOffsetsA(pxdstate_s*,int unitmap,double offsetsA[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_getAdcOffsetsA(void* , int unitmap, double offsetsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getAdcOffsetsB(pxdstate_s*,int unitmap,double offsetsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_getAdcOffsetsB(void* , int unitmap, double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getAdcOffsetA(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getAdcOffsetA(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_getAdcOffsetB(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_getAdcOffsetB(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setExsyncPrin(int unitmap,uint exsync,uint prin);
int __stdcall pxd_setExsyncPrin(int unitmap, int exsync, int prin);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxd_getExsync(int unitmap);
int __stdcall pxd_getExsync(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxd_getPrin(int unitmap);
int __stdcall pxd_getPrin(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setExsyncPrincMode(int unitmap,uint exsyncbits,uint princbits);
int __stdcall pxd_setExsyncPrincMode(int unitmap, int exsyncbits, int princbits);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxd_getExsyncMode(int unitmap);
int __stdcall pxd_getExsyncMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxd_getPrincMode(int unitmap);
int __stdcall pxd_getPrincMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setExsyncPrin(pxdstate_s*,int unitmap,uint exsync,uint prin);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setExsyncPrin(void* , int unitmap, int exsync, int prin);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxe_getExsync(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getExsync(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxe_getPrin(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getPrin(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setExsyncPrincMode(pxdstate_s*,int unitmap,uint exsyncbits,uint princbits);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setExsyncPrincMode(void* , int unitmap, int exsyncbits, int princbits);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxe_getExsyncMode(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getExsyncMode(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,uint)pxe_getPrincMode(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getPrincMode(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getGPIn(int unitmap,int data);
int __stdcall pxd_getGPIn(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setGPIn(int unitmap,int data);
int __stdcall pxd_setGPIn(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setGPOut(int unitmap,int data);
int __stdcall pxd_setGPOut(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getGPOut(int unitmap,int data);
int __stdcall pxd_getGPOut(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getGPTrigger(int unitmap,int which);
int __stdcall pxd_getGPTrigger(int unitmap, int which);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setCameraLinkCCOut(int unitmap,int data);
int __stdcall pxd_setCameraLinkCCOut(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_getCameraLinkCCOut(int unitmap,int data);
int __stdcall pxd_getCameraLinkCCOut(int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getGPIn(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getGPIn(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setGPIn(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setGPIn(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setGPOut(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setGPOut(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getGPOut(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getGPOut(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getGPTrigger(pxdstate_s*,int unitmap,int which);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getGPTrigger(void* , int unitmap, int which);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setCameraLinkCCOut(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_setCameraLinkCCOut(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_getCameraLinkCCOut(pxdstate_s*,int unitmap,int data);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_getCameraLinkCCOut(void* , int unitmap, int data);

// _cDcl(__declspec(dllexport),__stdcall,HGLOBAL)pxd_renderDIBCreate(int unitmap,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int mode,int options);
void* __stdcall pxd_renderDIBCreate(int unitmap, long buf, int ulx, int uly, int lrx, int lry, int mode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderDIBFree(HGLOBAL hDIB);
// The `hDIB' parameter has been declared expecting a Windows global handle to be passed
int __stdcall pxd_renderDIBFree(void* hDIB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderStretchDIBits(int unitmap,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int options,
// HDC hDC,uint nX,uint nY,uint nWidth,uint nHeight,int winoptions);
// The `hDC' parameter has been declared expecting a Windows handle to a device context (hDC) to be passed
int __stdcall pxd_renderStretchDIBits(int unitmap, long buf, int ulx, int uly, int lrx, int lry, int options, 
void* hDC, int nX, int nY, int nWidth, int nHeight, int winoptions);

// _cDcl(__declspec(dllexport),__stdcall,HGLOBAL)pxe_renderDIBCreate(pxdstate_s*,int unitmap,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int mode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_renderDIBCreate(void* , int unitmap, long buf, int ulx, int uly, int lrx, int lry, int mode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderDIBFree(pxdstate_s*,HGLOBAL hDIB);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hDIB' parameter has been declared expecting a Windows global handle to be passed
int __stdcall pxe_renderDIBFree(void* , void* hDIB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderStretchDIBits(pxdstate_s*,int unitmap,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int options,
// HDC hDC,uint nX,uint nY,uint nWidth,uint nHeight,int winoptions);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hDC' parameter has been declared expecting a Windows handle to a device context (hDC) to be passed
int __stdcall pxe_renderStretchDIBits(void* , int unitmap, long buf, int ulx, int uly, int lrx, int lry, 
int options, void* hDC, int nX, int nY, int nWidth, int nHeight, int winoptions);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_saveBmp(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
int __stdcall pxd_saveBmp(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int savemode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_loadBmp(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int loadmode,int options);
int __stdcall pxd_loadBmp(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int loadmode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_saveTga(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
int __stdcall pxd_saveTga(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int savemode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_savePcx(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
int __stdcall pxd_savePcx(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int savemode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_saveTiff(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
int __stdcall pxd_saveTiff(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int savemode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_loadTiff(int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int loadmode,int options);
int __stdcall pxd_loadTiff(int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, int loadmode, 
int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_saveRawBuffers(int unitmap,const char*pathname,pxbuffer_t startbuf,pxbuffer_t endbuf,void*filehandle,pxbuffer_t fileoffset,uint32 alignment,int options);
int __stdcall pxd_saveRawBuffers(int unitmap, char* pathname, long startbuf, long endbuf, void* filehandle, 
long fileoffset, int alignment, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_loadRawBuffers(int unitmap,const char*pathname,pxbuffer_t startbuf,pxbuffer_t endbuf,void*filehandle,pxbuffer_t fileoffset,uint32 alignment,int options);
int __stdcall pxd_loadRawBuffers(int unitmap, char* pathname, long startbuf, long endbuf, void* filehandle, 
long fileoffset, int alignment, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_saveBmp(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_saveBmp(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int savemode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_loadBmp(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int loadmode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_loadBmp(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int loadmode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_saveTga(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_saveTga(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int savemode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_savePcx(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_savePcx(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int savemode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_saveTiff(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int savemode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_saveTiff(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int savemode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_loadTiff(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t buf,pxcoord_t ulx,pxcoord_t uly,pxcoord_t lrx,pxcoord_t lry,int loadmode,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_loadTiff(void* , int unitmap, char* pathname, long buf, int ulx, int uly, int lrx, int lry, 
int loadmode, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_saveRawBuffers(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t startbuf,pxbuffer_t endbuf,void*filehandle,pxbuffer_t fileoffset,uint32 alignment,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_saveRawBuffers(void* , int unitmap, char* pathname, long startbuf, long endbuf, void* filehandle, 
long fileoffset, int alignment, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_loadRawBuffers(pxdstate_s*,int unitmap,const char*pathname,pxbuffer_t startbuf,pxbuffer_t endbuf,void*filehandle,pxbuffer_t fileoffset,uint32 alignment,int options);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_loadRawBuffers(void* , int unitmap, char* pathname, long startbuf, long endbuf, void* filehandle, 
long fileoffset, int alignment, int options);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderDirectVideoUnLive(int unitmap,HWND hWnd);
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxd_renderDirectVideoUnLive(int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderDirectVideoLive(int unitmap,HWND hWnd,uint nX,uint nY,
// uint nWidth,uint nHeight,COLORREF ClrKey1,COLORREF ClrKey2);
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxd_renderDirectVideoLive(int unitmap, void* hWnd, int nX, int nY, int nWidth, int nHeight, 
long ClrKey1, long ClrKey2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderDirectVideoDone(int unitmap,HWND hWnd);
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxd_renderDirectVideoDone(int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_renderDirectVideoInit(int unitmap,HWND hWnd);
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxd_renderDirectVideoInit(int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderDirectVideoUnLive(pxdstate_s*,int unitmap,HWND hWnd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxe_renderDirectVideoUnLive(void* , int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderDirectVideoLive(pxdstate_s*,int unitmap,HWND hWnd,uint nX,uint nY,
// uint nWidth,uint nHeight,COLORREF ClrKey1,COLORREF ClrKey2);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxe_renderDirectVideoLive(void* , int unitmap, void* hWnd, int nX, int nY, int nWidth, int nHeight, 
long ClrKey1, long ClrKey2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderDirectVideoDone(pxdstate_s*,int unitmap,HWND hWnd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxe_renderDirectVideoDone(void* , int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_renderDirectVideoInit(pxdstate_s*,int unitmap,HWND hWnd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hWnd' parameter has been declared expecting a Windows handle to a window (hWnd) to be passed
int __stdcall pxe_renderDirectVideoInit(void* , int unitmap, void* hWnd);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventFieldCreate(int unitmap);
void* __stdcall pxd_eventFieldCreate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventFieldCreate2(int unitmap,int type);
void* __stdcall pxd_eventFieldCreate2(int unitmap, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_eventFieldClose(int unitmap,HANDLE hEvent);
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxd_eventFieldClose(int unitmap, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventCapturedFieldCreate(int unitmap);
void* __stdcall pxd_eventCapturedFieldCreate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventCapturedFieldCreate2(int unitmap,int type);
void* __stdcall pxd_eventCapturedFieldCreate2(int unitmap, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_eventCapturedFieldClose(int unitmap,HANDLE hEvent);
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxd_eventCapturedFieldClose(int unitmap, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventGPTriggerCreate(int unitmap,int which,int rsvd);
void* __stdcall pxd_eventGPTriggerCreate(int unitmap, int which, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventGPTriggerCreate2(int unitmap,int which,int rsvd,int type);
void* __stdcall pxd_eventGPTriggerCreate2(int unitmap, int which, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_eventGPTriggerClose(int unitmap,int which,int rsvd,HANDLE hEvent);
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxd_eventGPTriggerClose(int unitmap, int which, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventFaultCreate(int unitmap,int rsvd);
void* __stdcall pxd_eventFaultCreate(int unitmap, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventFaultCreate2(int unitmap,int rsvd,int type);
void* __stdcall pxd_eventFaultCreate2(int unitmap, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_eventFaultClose(int unitmap,int rsvd,HANDLE hEvent);
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxd_eventFaultClose(int unitmap, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxd_eventSerialCreate(int unitmap,int which,int rsvd,int type);
void* __stdcall pxd_eventSerialCreate(int unitmap, int which, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxd_eventSerialClose(int unitmap,int which,int rsvd,HANDLE hEvent);
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxd_eventSerialClose(int unitmap, int which, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventFieldCreate(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventFieldCreate(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventFieldCreate2(pxdstate_s*,int unitmap,int type);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventFieldCreate2(void* , int unitmap, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_eventFieldClose(pxdstate_s*,int unitmap,HANDLE hEvent);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxe_eventFieldClose(void* , int unitmap, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventCapturedFieldCreate(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventCapturedFieldCreate(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventCapturedFieldCreate2(pxdstate_s*,int unitmap,int type);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventCapturedFieldCreate2(void* , int unitmap, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_eventCapturedFieldClose(pxdstate_s*,int unitmap,HANDLE hEvent);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxe_eventCapturedFieldClose(void* , int unitmap, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventGPTriggerCreate(pxdstate_s*,int unitmap,int which,int rsvd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventGPTriggerCreate(void* , int unitmap, int which, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventGPTriggerCreate2(pxdstate_s*,int unitmap,int which,int rsvd,int type);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventGPTriggerCreate2(void* , int unitmap, int which, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_eventGPTriggerClose(pxdstate_s*,int unitmap,int which,int rsvd,HANDLE hEvent);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxe_eventGPTriggerClose(void* , int unitmap, int which, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventFaultCreate(pxdstate_s*,int unitmap,int rsvd);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventFaultCreate(void* , int unitmap, int rsvd);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventFaultCreate2(pxdstate_s*,int unitmap,int rsvd,int type);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventFaultCreate2(void* , int unitmap, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_eventFaultClose(pxdstate_s*,int unitmap,int rsvd,HANDLE hEvent);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxe_eventFaultClose(void* , int unitmap, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,HANDLE)pxe_eventSerialCreate(pxdstate_s*,int unitmap,int which,int rsvd,int type);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
void* __stdcall pxe_eventSerialCreate(void* , int unitmap, int which, int rsvd, int type);

// _cDcl(__declspec(dllexport),__stdcall,void)pxe_eventSerialClose(pxdstate_s*,int unitmap,int which,int rsvd,HANDLE hEvent);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `hEvent' parameter has been declared expecting a Windows handle to be passed
void __stdcall pxe_eventSerialClose(void* , int unitmap, int which, int rsvd, void* hEvent);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setImageDarkBalance(int unitmap,uint referenceRGB[3],uint targetRGB[3],double gamma);
// The `referenceRGB' parameter has been declared expecting a dimensioned int array to be passed
// The `targetRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
int __stdcall pxd_setImageDarkBalance(int unitmap, unsigned int referenceRGB[], unsigned int targetRGB[], double gamma);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_setImageBrightBalance(int unitmap,uint referenceRGB[3],uint targetRGB[3],double gamma);
// The `referenceRGB' parameter has been declared expecting a dimensioned int array to be passed
// The `targetRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
int __stdcall pxd_setImageBrightBalance(int unitmap, unsigned int referenceRGB[], unsigned int targetRGB[], 
double gamma);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setImageDarkBalance(pxdstate_s*,int unitmap,uint referenceRGB[3],uint targetRGB[3],double gamma);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `referenceRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
// The `targetRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
int __stdcall pxe_setImageDarkBalance(void* , int unitmap, unsigned int referenceRGB[], unsigned int targetRGB[], 
double gamma);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_setImageBrightBalance(pxdstate_s*,int unitmap,uint referenceRGB[3],uint targetRGB[3],double gamma);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `referenceRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
// The `targetRGB' parameter has been declared expecting a dimensioned int array w. 3 elements to be passed
int __stdcall pxe_setImageBrightBalance(void* , int unitmap, unsigned int referenceRGB[], unsigned int targetRGB[], 
double gamma);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_serialConfigure(int unitmap,int rsvd0,double baud,int bits,int parity,int stopbits,int rsvd1,int rsvd2,int rsvd3);
int __stdcall pxd_serialConfigure(int unitmap, int rsvd0, double baud, int bits, int parity, int stopbits, 
int rsvd1, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_serialRead(int unitmap,int rsvd0,uchar*buf,int cnt);
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxd_serialRead(int unitmap, int rsvd0, unsigned char* buf, int cnt);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_serialWrite(int unitmap,int rsvd0,uchar*buf,int cnt);
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxd_serialWrite(int unitmap, int rsvd0, unsigned char* buf, int cnt);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_serialRead(int unitmap,int rsvd0,char buf[],int cnt);
// Duplicate declaration

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_serialWrite(int unitmap,int rsvd0,char buf[],int cnt);
// Duplicate declaration

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_serialConfigure(pxdstate_s*,int unitmap,int rsvd0,double baud,int bits,int parity,int stopbits,int rsvd1,int rsvd2,int rsvd3);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_serialConfigure(void* , int unitmap, int rsvd0, double baud, int bits, int parity, int stopbits, 
int rsvd1, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_serialRead(pxdstate_s*,int unitmap,int rsvd0,uchar*buf,int cnt);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxe_serialRead(void* , int unitmap, int rsvd0, unsigned char* buf, int cnt);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_serialWrite(pxdstate_s*,int unitmap,int rsvd0,uchar*buf,int cnt);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `buf' parameter has been declared expecting a dimensioned unsigned char array to be passed
int __stdcall pxe_serialWrite(void* , int unitmap, int rsvd0, unsigned char* buf, int cnt);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_serialRead(pxdstate_s*,int unitmap,int rsvd0,char buf[],int cnt);
// Duplicate declaration

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_serialWrite(pxdstate_s*,int unitmap,int rsvd0,char buf[],int cnt);
// Duplicate declaration

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clSerialInit();
int __cdecl clSerialInit();

// _cDcl(__declspec(dllexport) ,__cdecl,void)clSerialClose();
void __cdecl clSerialClose();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clSerialRead();
int __cdecl clSerialRead();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clSerialWrite();
int __cdecl clSerialWrite();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetNumBytesAvail();
int __cdecl clGetNumBytesAvail();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clFlushPort();
int __cdecl clFlushPort();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetErrorText();
int __cdecl clGetErrorText();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetNumSerialPorts();
int __cdecl clGetNumSerialPorts();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetSerialPortIdentifier();
int __cdecl clGetSerialPortIdentifier();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetManufacturerInfo();
int __cdecl clGetManufacturerInfo();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clGetSupportedBaudRates();
int __cdecl clGetSupportedBaudRates();

// _cDcl(__declspec(dllexport) ,__cdecl,sint32)clSetBaudRate();
int __cdecl clSetBaudRate();

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_setExposureAndGain(int unitmap,int rsvd,double exposure,double redgain,double grngain,double blugain);
int __stdcall pxd_SV2112_setExposureAndGain(int unitmap, int rsvd, double exposure, double redgain, 
double grngain, double blugain);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_setResolutionAndTiming(int unitmap,int rsvd,int decimation,int aoileft,int aoitop,int aoiwidth,int aoiheight,
// int scandirection,double pixelClkFreq,double rsvd2,int rsvd3);
int __stdcall pxd_SV2112_setResolutionAndTiming(int unitmap, int rsvd, int decimation, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int scandirection, double pixelClkFreq, double rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledvideomode,int controlledtrigger);
int __stdcall pxd_SV2112_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledvideomode, 
int controlledtrigger);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_setCtrlExposureAndRate(int unitmap,int rsvd,double exposure,double framerate);
int __stdcall pxd_SV2112_setCtrlExposureAndRate(int unitmap, int rsvd, double exposure, double framerate);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getExposure(int unitmap);
double __stdcall pxd_SV2112_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getGain(int unitmap,int color);
double __stdcall pxd_SV2112_getGain(int unitmap, int color);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getDecimation(int unitmap);
int __stdcall pxd_SV2112_getDecimation(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getAoiTop(int unitmap);
int __stdcall pxd_SV2112_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getAoiLeft(int unitmap);
int __stdcall pxd_SV2112_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getPixelClock(int unitmap);
double __stdcall pxd_SV2112_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getScanDirection(int unitmap);
int __stdcall pxd_SV2112_getScanDirection(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getVideoMode(int unitmap);
int __stdcall pxd_SV2112_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV2112_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV2112_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV2112_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV2112_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getCtrlExposure(int unitmap);
double __stdcall pxd_SV2112_getCtrlExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV2112_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV2112_getMinMaxCtrlExposure(int unitmap,double exposure);
double __stdcall pxd_SV2112_getMinMaxCtrlExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_setExposureGainOffset(int unitmap,int rsvd,double exposure,double gain,double offset,double rsvd2,double rsvd3);
int __stdcall pxd_SV1310_setExposureGainOffset(int unitmap, int rsvd, double exposure, double gain, 
double offset, double rsvd2, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_setColorGain(int unitmap,int rsvd,double greenR,double red,double blue,double greenB);
int __stdcall pxd_SV1310_setColorGain(int unitmap, int rsvd, double greenR, double red, double blue, double greenB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_setResolutionAndTiming(int unitmap,int rsvd,int subsample,int aoileft,int aoitop,int aoiwidth,int aoiheight,
// int readoutdirection,double pixelClkFreq,double framePeriod,double rsvd2);
int __stdcall pxd_SV1310_setResolutionAndTiming(int unitmap, int rsvd, int subsample, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int readoutdirection, double pixelClkFreq, double framePeriod, 
double rsvd2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int strobemode,int rsvd2,int rsvd3);
int __stdcall pxd_SV1310_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledmode, 
int controlledtrigger, int strobemode, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_setCtrlRate(int unitmap,int rsvd,double rsvd2,double framerate,double rsvd3);
int __stdcall pxd_SV1310_setCtrlRate(int unitmap, int rsvd, double rsvd2, double framerate, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getExposure(int unitmap);
double __stdcall pxd_SV1310_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getGain(int unitmap);
double __stdcall pxd_SV1310_getGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getOffset(int unitmap);
double __stdcall pxd_SV1310_getOffset(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getSubsample(int unitmap);
int __stdcall pxd_SV1310_getSubsample(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getAoiTop(int unitmap);
int __stdcall pxd_SV1310_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getAoiLeft(int unitmap);
int __stdcall pxd_SV1310_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getReadoutDirection(int unitmap);
int __stdcall pxd_SV1310_getReadoutDirection(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getPixelClock(int unitmap);
double __stdcall pxd_SV1310_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getFramePeriod(int unitmap);
double __stdcall pxd_SV1310_getFramePeriod(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV1310_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getVideoMode(int unitmap);
int __stdcall pxd_SV1310_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV1310_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV1310_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1310_getStrobeMode(int unitmap);
int __stdcall pxd_SV1310_getStrobeMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getColorGain(int unitmap,int color);
double __stdcall pxd_SV1310_getColorGain(int unitmap, int color);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV1310_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1310_getMinMaxFramePeriod(int unitmap,double frameperiod);
double __stdcall pxd_SV1310_getMinMaxFramePeriod(int unitmap, double frameperiod);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_setExposureGainOffset(int unitmap,int rsvd,double exposure,
// double gain,double offset,double rsvd2,double rsvd3);
int __stdcall pxd_SV1281_setExposureGainOffset(int unitmap, int rsvd, double exposure, double gain, 
double offset, double rsvd2, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_setResolutionAndTiming(int unitmap,int rsvd,int rsvd1,
// int aoileft,int aoitop,int aoiwidth,int aoiheight,
// int rsvd4,double pixelClkFreq,double rsvd2,double rsvd3);
int __stdcall pxd_SV1281_setResolutionAndTiming(int unitmap, int rsvd, int rsvd1, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int rsvd4, double pixelClkFreq, double rsvd2, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledvideomode,int controlledtrigger,int rsvd1,int rsvd2,int rsvd3);
int __stdcall pxd_SV1281_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledvideomode, 
int controlledtrigger, int rsvd1, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_setCtrlRate(int unitmap,int rsvd,double rsvd2,double framerate,double rsvd3);
int __stdcall pxd_SV1281_setCtrlRate(int unitmap, int rsvd, double rsvd2, double framerate, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getExposure(int unitmap);
double __stdcall pxd_SV1281_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getGain(int unitmap);
double __stdcall pxd_SV1281_getGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getOffset(int unitmap);
double __stdcall pxd_SV1281_getOffset(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_getAoiTop(int unitmap);
int __stdcall pxd_SV1281_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_getAoiLeft(int unitmap);
int __stdcall pxd_SV1281_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getPixelClock(int unitmap);
double __stdcall pxd_SV1281_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_getVideoMode(int unitmap);
int __stdcall pxd_SV1281_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV1281_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV1281_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV1281_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV1281_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV1281_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV1281_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_setExposureAndGain(int unitmap,int rsvd,double exposure,double redgain,double grnrgain,double bluegain,double grnbgain);
int __stdcall pxd_SV9M001_setExposureAndGain(int unitmap, int rsvd, double exposure, double redgain, 
double grnrgain, double bluegain, double grnbgain);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_setResolutionAndTiming(int unitmap,int rsvd,int subsample,int aoileft,int aoitop,int aoiwidth,int aoiheight,int scandirection,double pixelClkFreq,double framePeriod,double rsvd2);
int __stdcall pxd_SV9M001_setResolutionAndTiming(int unitmap, int rsvd, int subsample, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int scandirection, double pixelClkFreq, double framePeriod, double rsvd2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int rsvd4,int rsvd2,int rsvd3);
int __stdcall pxd_SV9M001_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledmode, 
int controlledtrigger, int rsvd4, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_setCtrlRate(int unitmap,int rsvd,double rsvd2,double framerate,double rsvd3);
int __stdcall pxd_SV9M001_setCtrlRate(int unitmap, int rsvd, double rsvd2, double framerate, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_setExposureAndDigitalGain(int unitmap,int rsvd,double exposure,double digitalgain,double rsvd2,double rsvd3,double rsvd4);
int __stdcall pxd_SV9M001_setExposureAndDigitalGain(int unitmap, int rsvd, double exposure, 
double digitalgain, double rsvd2, double rsvd3, double rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getExposure(int unitmap);
double __stdcall pxd_SV9M001_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getAoiTop(int unitmap);
int __stdcall pxd_SV9M001_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getAoiLeft(int unitmap);
int __stdcall pxd_SV9M001_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getGain(int unitmap,int color);
double __stdcall pxd_SV9M001_getGain(int unitmap, int color);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getPixelClock(int unitmap);
double __stdcall pxd_SV9M001_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getFramePeriod(int unitmap);
double __stdcall pxd_SV9M001_getFramePeriod(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getVideoMode(int unitmap);
int __stdcall pxd_SV9M001_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV9M001_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV9M001_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV9M001_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getSubsample(int unitmap);
int __stdcall pxd_SV9M001_getSubsample(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV9M001_getScanDirection(int unitmap);
int __stdcall pxd_SV9M001_getScanDirection(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getDigitalGain(int unitmap);
double __stdcall pxd_SV9M001_getDigitalGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV9M001_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV9M001_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_setExposureAndGain(int unitmap,int rsvd1,double exposure,double redgain,double grnrgain,double bluegain,double grnbgain,int gainrange);
int __stdcall pxd_SV642_setExposureAndGain(int unitmap, int rsvd1, double exposure, double redgain, 
double grnrgain, double bluegain, double grnbgain, int gainrange);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_setResolutionAndTiming(int unitmap,int rsvd,int rsvd2,int aoileft,int aoitop,int aoiwidth,int aoiheight,int rsvd3,double pixelClkFreq,double framePeriod,double rsvd4);
int __stdcall pxd_SV642_setResolutionAndTiming(int unitmap, int rsvd, int rsvd2, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int rsvd3, double pixelClkFreq, double framePeriod, double rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int rsvd4,int rsvd2,int rsvd3);
int __stdcall pxd_SV642_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledmode, 
int controlledtrigger, int rsvd4, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_setCtrlRate(int unitmap,int rsvd,double rsvd2,double framerate,double rsvd3);
int __stdcall pxd_SV642_setCtrlRate(int unitmap, int rsvd, double rsvd2, double framerate, double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getExposure(int unitmap);
double __stdcall pxd_SV642_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getAoiTop(int unitmap);
int __stdcall pxd_SV642_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getAoiLeft(int unitmap);
int __stdcall pxd_SV642_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getGain(int unitmap,int color);
double __stdcall pxd_SV642_getGain(int unitmap, int color);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getPixelClock(int unitmap);
double __stdcall pxd_SV642_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getFramePeriod(int unitmap);
double __stdcall pxd_SV642_getFramePeriod(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getVideoMode(int unitmap);
int __stdcall pxd_SV642_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV642_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV642_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV642_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV642_getGainRange(int unitmap);
int __stdcall pxd_SV642_getGainRange(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV642_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV642_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_setExposureAndGain(int unitmap,int rsvd1,double exposure,double gain);
int __stdcall pxd_SV643_setExposureAndGain(int unitmap, int rsvd1, double exposure, double gain);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_setExposureAndGainOffset(int unitmap,int rsvd1,double exposure,double gain,double offset,double rsvd2);
int __stdcall pxd_SV643_setExposureAndGainOffset(int unitmap, int rsvd1, double exposure, double gain, 
double offset, double rsvd2);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_setResolutionAndTiming(int unitmap,int rsvd,int subsample,int aoileft,int aoitop,int aoiwidth,int aoiheight,int rsvd3,double pixelClkFreq,double framePeriod,double rsvd4);
int __stdcall pxd_SV643_setResolutionAndTiming(int unitmap, int rsvd, int subsample, int aoileft, 
int aoitop, int aoiwidth, int aoiheight, int rsvd3, double pixelClkFreq, double framePeriod, double rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_setVideoAndTriggerMode(int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int rsvd4,int rsvd2,int rsvd3);
int __stdcall pxd_SV643_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, int controlledmode, 
int controlledtrigger, int rsvd4, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_setCtrlExposureAndRate(int unitmap,int rsvd,double exposure,double framerate,double rsvd3);
int __stdcall pxd_SV643_setCtrlExposureAndRate(int unitmap, int rsvd, double exposure, double framerate, 
double rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getExposure(int unitmap);
double __stdcall pxd_SV643_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getAoiTop(int unitmap);
int __stdcall pxd_SV643_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getAoiLeft(int unitmap);
int __stdcall pxd_SV643_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getGain(int unitmap);
double __stdcall pxd_SV643_getGain(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getOffset(int unitmap);
double __stdcall pxd_SV643_getOffset(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getPixelClock(int unitmap);
double __stdcall pxd_SV643_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getFramePeriod(int unitmap);
double __stdcall pxd_SV643_getFramePeriod(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getVideoMode(int unitmap);
int __stdcall pxd_SV643_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SV643_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getCtrlExposure(int unitmap);
double __stdcall pxd_SV643_getCtrlExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SV643_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SV643_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SV643_getSubsample(int unitmap);
int __stdcall pxd_SV643_getSubsample(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SV643_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SV643_getMinMaxCtrlExposure(int unitmap,double exposure);
double __stdcall pxd_SV643_getMinMaxCtrlExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setExposureGainOffset(
// int unitmap,int rsvd,double exposure,double gainA,double offsetA,double gainB,double offsetB);
int __stdcall pxd_SILICONVIDEO_setExposureGainOffset(int unitmap, int rsvd, double exposure, 
double gainA, double offsetA, double gainB, double offsetB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setExposureColorGainOffsets(
// int unitmap,int rsvd,double exposure,double gainsA[4],double gainsB[4],double offsetsA[4],double offsetsB[4]);
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxd_SILICONVIDEO_setExposureColorGainOffsets(int unitmap, int rsvd, double exposure, 
double gainsA[], double gainsB[], double offsetsA[], double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setExposure(int unitmap,int rsvd,double exposure);
int __stdcall pxd_SILICONVIDEO_setExposure(int unitmap, int rsvd, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setVideoAndTriggerMode(
// int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int rsvd1,int rsvd2,int rsvd3,int rsvd4);
int __stdcall pxd_SILICONVIDEO_setVideoAndTriggerMode(int unitmap, int rsvd, int videomode, 
int controlledmode, int controlledtrigger, int rsvd1, int rsvd2, int rsvd3, int rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setResolutionAndTiming(
// int unitmap,int rsvd1,int subsample,int aoileft,int aoitop,int aoiwidth,int aoiheight,int scandirection,
// int bitdepth,int rsvd3,int rsvd4,double pixelClkFreq,double framePeriod,double rsvd5,double rsvd6,double rsvd7);
int __stdcall pxd_SILICONVIDEO_setResolutionAndTiming(int unitmap, int rsvd1, int subsample, 
int aoileft, int aoitop, int aoiwidth, int aoiheight, int scandirection, int bitdepth, int rsvd3, int rsvd4, 
double pixelClkFreq, double framePeriod, double rsvd5, double rsvd6, double rsvd7);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setCtrlRates(
// int unitmap,int rsvd,double exposure,double framerate,double rsvd2,double rsvd3,double rsvd4);
int __stdcall pxd_SILICONVIDEO_setCtrlRates(int unitmap, int rsvd, double exposure, double framerate, 
double rsvd2, double rsvd3, double rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_setAxC(int unitmap,int rsvd,
// int agcA,int agcB,int rsvd2,int rsvd3,int aec,int rsvd4,int rsvd5,int rsvd6,int rsvd7);
int __stdcall pxd_SILICONVIDEO_setAxC(int unitmap, int rsvd, int agcA, int agcB, int rsvd2, int rsvd3, 
int aec, int rsvd4, int rsvd5, int rsvd6, int rsvd7);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getCameraType(int unitmap);
int __stdcall pxd_SILICONVIDEO_getCameraType(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getExposure(int unitmap);
double __stdcall pxd_SILICONVIDEO_getExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAoiTop(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAoiTop(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAoiLeft(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAoiLeft(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAoiWidth(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAoiWidth(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAoiHeight(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAoiHeight(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getPixelClock(int unitmap);
double __stdcall pxd_SILICONVIDEO_getPixelClock(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getFramePeriod(int unitmap);
double __stdcall pxd_SILICONVIDEO_getFramePeriod(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getSubsample(int unitmap);
int __stdcall pxd_SILICONVIDEO_getSubsample(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getScanDirection(int unitmap);
int __stdcall pxd_SILICONVIDEO_getScanDirection(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getVideoMode(int unitmap);
int __stdcall pxd_SILICONVIDEO_getVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getCtrlExposure(int unitmap);
double __stdcall pxd_SILICONVIDEO_getCtrlExposure(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getCtrlFrameRate(int unitmap);
double __stdcall pxd_SILICONVIDEO_getCtrlFrameRate(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getCtrlVideoMode(int unitmap);
int __stdcall pxd_SILICONVIDEO_getCtrlVideoMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getCtrlTriggerMode(int unitmap);
int __stdcall pxd_SILICONVIDEO_getCtrlTriggerMode(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getGainA(int unitmap);
double __stdcall pxd_SILICONVIDEO_getGainA(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getGainB(int unitmap);
double __stdcall pxd_SILICONVIDEO_getGainB(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getGainsA(int unitmap,double gainsA[4]);
// The `gainsA' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_SILICONVIDEO_getGainsA(int unitmap, double gainsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getGainsB(int unitmap,double gainsB[4]);
// The `gainsB' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_SILICONVIDEO_getGainsB(int unitmap, double gainsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getOffsetsA(int unitmap,double offsetsA[4]);
// The `offsetsA' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_SILICONVIDEO_getOffsetsA(int unitmap, double offsetsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getOffsetsB(int unitmap,double offsetsB[4]);
// The `offsetsB' parameter has been declared expecting a dimensioned double array to be passed
int __stdcall pxd_SILICONVIDEO_getOffsetsB(int unitmap, double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getOffsetA(int unitmap);
double __stdcall pxd_SILICONVIDEO_getOffsetA(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getOffsetB(int unitmap);
double __stdcall pxd_SILICONVIDEO_getOffsetB(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxExposure(int unitmap,double exposure);
double __stdcall pxd_SILICONVIDEO_getMinMaxExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxFramePeriod(int unitmap,double framePeriod);
double __stdcall pxd_SILICONVIDEO_getMinMaxFramePeriod(int unitmap, double framePeriod);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxCtrlFrameRate(int unitmap,double frameRate);
double __stdcall pxd_SILICONVIDEO_getMinMaxCtrlFrameRate(int unitmap, double frameRate);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxCtrlExposure(int unitmap,double exposure);
double __stdcall pxd_SILICONVIDEO_getMinMaxCtrlExposure(int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxPixelClock(int unitmap,double pixelClkFreq);
double __stdcall pxd_SILICONVIDEO_getMinMaxPixelClock(int unitmap, double pixelClkFreq);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxGainA(int unitmap,double gain);
double __stdcall pxd_SILICONVIDEO_getMinMaxGainA(int unitmap, double gain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxGainB(int unitmap,double gain);
double __stdcall pxd_SILICONVIDEO_getMinMaxGainB(int unitmap, double gain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxOffsetA(int unitmap,double offset);
double __stdcall pxd_SILICONVIDEO_getMinMaxOffsetA(int unitmap, double offset);

// _cDcl(__declspec(dllexport),__stdcall,double)pxd_SILICONVIDEO_getMinMaxOffsetB(int unitmap,double offset);
double __stdcall pxd_SILICONVIDEO_getMinMaxOffsetB(int unitmap, double offset);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxAoiWidth(int unitmap,int width);
int __stdcall pxd_SILICONVIDEO_getMinMaxAoiWidth(int unitmap, int width);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxAoiHeight(int unitmap,int height);
int __stdcall pxd_SILICONVIDEO_getMinMaxAoiHeight(int unitmap, int height);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAgcA(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAgcA(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAgcB(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAgcB(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getAec(int unitmap);
int __stdcall pxd_SILICONVIDEO_getAec(int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxAgcA(int unitmap,int agc);
int __stdcall pxd_SILICONVIDEO_getMinMaxAgcA(int unitmap, int agc);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxAgcB(int unitmap,int agc);
int __stdcall pxd_SILICONVIDEO_getMinMaxAgcB(int unitmap, int agc);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxAec(int unitmap,int aec);
int __stdcall pxd_SILICONVIDEO_getMinMaxAec(int unitmap, int aec);

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_SILICONVIDEO_getMinMaxResolution(int unitmap,int subsamplep[],int aoileftp[],int aoitopp[],int aoiwidthp[],int aoiheightp[],int scandirectionp[],int bitdepthp[],int rsvd3[],int rsvd4[]);
// The `subsamplep' parameter has been declared expecting a dimensioned int array to be passed
// The `aoileftp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoitopp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoiwidthp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoiheightp' parameter has been declared expecting a dimensioned int array to be passed
// The `scandirectionp' parameter has been declared expecting a dimensioned int array to be passed
// The `bitdepthp' parameter has been declared expecting a dimensioned int array to be passed
// The `rsvd3' parameter has been declared expecting a dimensioned int array to be passed
// The `rsvd4' parameter has been declared expecting a dimensioned int array to be passed
int __stdcall pxd_SILICONVIDEO_getMinMaxResolution(int unitmap, int subsamplep[], int aoileftp[], 
int aoitopp[], int aoiwidthp[], int aoiheightp[], int scandirectionp[], int bitdepthp[], int rsvd3[], 
int rsvd4[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setExposureGainOffset(pxdstate_s*,
// int unitmap,int rsvd,double exposure,double gainA,double offsetA,double gainB,double offsetB);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setExposureGainOffset(void* , int unitmap, int rsvd, double exposure, 
double gainA, double offsetA, double gainB, double offsetB);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setExposureColorGainOffsets(pxdstate_s*,
// int unitmap,int rsvd,double exposure,double gainsA[4],double gainsB[4],double offsetsA[4],double offsetsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_SILICONVIDEO_setExposureColorGainOffsets(void* , int unitmap, int rsvd, double exposure, 
double gainsA[], double gainsB[], double offsetsA[], double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setExposure(pxdstate_s*,int unitmap,int rsvd,double exposure);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setExposure(void* , int unitmap, int rsvd, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setVideoAndTriggerMode(pxdstate_s*,
// int unitmap,int rsvd,int videomode,int controlledmode,int controlledtrigger,int rsdv1,int rsvd2,int rsvd3,int rsvd4);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setVideoAndTriggerMode(void* , int unitmap, int rsvd, int videomode, 
int controlledmode, int controlledtrigger, int rsdv1, int rsvd2, int rsvd3, int rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setResolutionAndTiming(pxdstate_s*,
// int unitmap,int rsvd1,int subsample,int aoileft,int aoitop,int aoiwidth,int aoiheight,int scandirection,
// int bitdepth,int rsvd3,int rsvd4,double pixelClkFreq,double framePeriod,double rsvd5,double rsvd6,double rsvd7);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setResolutionAndTiming(void* , int unitmap, int rsvd1, int subsample, 
int aoileft, int aoitop, int aoiwidth, int aoiheight, int scandirection, int bitdepth, int rsvd3, int rsvd4, 
double pixelClkFreq, double framePeriod, double rsvd5, double rsvd6, double rsvd7);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setCtrlRates(pxdstate_s*,
// int unitmap,int rsvd,double exposure,double framerate,double rsvd2,double rsvd3,double rsvd4);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setCtrlRates(void* , int unitmap, int rsvd, double exposure, double framerate, 
double rsvd2, double rsvd3, double rsvd4);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_setAxC(pxdstate_s*,int unitmap,int rsvd,
// int agcA,int agcB,int rsvd2,int rsvd3,int aec,int rsvd4,int rsvd5,int rsvd6,int rsvd7);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_setAxC(void* , int unitmap, int rsvd, int agcA, int agcB, int rsvd2, 
int rsvd3, int aec, int rsvd4, int rsvd5, int rsvd6, int rsvd7);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getCameraType(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getCameraType(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getExposure(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getExposure(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAoiTop(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAoiTop(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAoiLeft(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAoiLeft(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAoiWidth(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAoiWidth(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAoiHeight(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAoiHeight(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getPixelClock(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getPixelClock(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getFramePeriod(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getFramePeriod(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getSubsample(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getSubsample(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getScanDirection(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getScanDirection(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getVideoMode(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getVideoMode(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getCtrlExposure(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getCtrlExposure(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getCtrlFrameRate(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getCtrlFrameRate(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getCtrlVideoMode(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getCtrlVideoMode(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getCtrlTriggerMode(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getCtrlTriggerMode(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getGainA(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getGainA(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getGainB(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getGainB(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getGainsA(pxdstate_s*,int unitmap,double gainsA[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_SILICONVIDEO_getGainsA(void* , int unitmap, double gainsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getGainsB(pxdstate_s*,int unitmap,double gainsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `gainsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_SILICONVIDEO_getGainsB(void* , int unitmap, double gainsB[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getOffsetsA(pxdstate_s*,int unitmap,double offsetsA[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `offsetsA' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_SILICONVIDEO_getOffsetsA(void* , int unitmap, double offsetsA[]);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getOffsetsB(pxdstate_s*,int unitmap,double offsetsB[4]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `offsetsB' parameter has been declared expecting a dimensioned double array w. 4 elements to be passed
int __stdcall pxe_SILICONVIDEO_getOffsetsB(void* , int unitmap, double offsetsB[]);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getOffsetA(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getOffsetA(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getOffsetB(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getOffsetB(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxExposure(pxdstate_s*,int unitmap,double exposure);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxExposure(void* , int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxFramePeriod(pxdstate_s*,int unitmap,double framePeriod);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxFramePeriod(void* , int unitmap, double framePeriod);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxCtrlFrameRate(pxdstate_s*,int unitmap,double frameRate);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxCtrlFrameRate(void* , int unitmap, double frameRate);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxCtrlExposure(pxdstate_s*,int unitmap,double exposure);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxCtrlExposure(void* , int unitmap, double exposure);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxPixelClock(pxdstate_s*,int unitmap,double pixelClkFreq);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxPixelClock(void* , int unitmap, double pixelClkFreq);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxGainA(pxdstate_s*,int unitmap,double gain);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxGainA(void* , int unitmap, double gain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxGainB(pxdstate_s*,int unitmap,double gain);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxGainB(void* , int unitmap, double gain);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxOffsetA(pxdstate_s*,int unitmap,double offset);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxOffsetA(void* , int unitmap, double offset);

// _cDcl(__declspec(dllexport),__stdcall,double)pxe_SILICONVIDEO_getMinMaxOffsetB(pxdstate_s*,int unitmap,double offset);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
double __stdcall pxe_SILICONVIDEO_getMinMaxOffsetB(void* , int unitmap, double offset);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxAoiWidth(pxdstate_s*,int unitmap,int width);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxAoiWidth(void* , int unitmap, int width);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxAoiHeight(pxdstate_s*,int unitmap,int height);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxAoiHeight(void* , int unitmap, int height);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAgcA(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAgcA(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAgcB(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAgcB(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getAec(pxdstate_s*,int unitmap);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getAec(void* , int unitmap);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxAgcA(pxdstate_s*,int unitmap,int agc);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxAgcA(void* , int unitmap, int agc);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxAgcB(pxdstate_s*,int unitmap,int agc);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxAgcB(void* , int unitmap, int agc);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxAec(pxdstate_s*,int unitmap,int aec);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxAec(void* , int unitmap, int aec);

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_SILICONVIDEO_getMinMaxResolution(pxdstate_s*,int unitmap,int subsamplep[],int aoileftp[],int aoitopp[],int aoiwidthp[],int aoiheightp[],int scandirectionp[],int bitdepthp[],int rsvd3[],int rsvd4[]);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
// The `subsamplep' parameter has been declared expecting a dimensioned int array to be passed
// The `aoileftp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoitopp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoiwidthp' parameter has been declared expecting a dimensioned int array to be passed
// The `aoiheightp' parameter has been declared expecting a dimensioned int array to be passed
// The `scandirectionp' parameter has been declared expecting a dimensioned int array to be passed
// The `bitdepthp' parameter has been declared expecting a dimensioned int array to be passed
// The `rsvd3' parameter has been declared expecting a dimensioned int array to be passed
// The `rsvd4' parameter has been declared expecting a dimensioned int array to be passed
int __stdcall pxe_SILICONVIDEO_getMinMaxResolution(void* , int unitmap, int subsamplep[], int aoileftp[], 
int aoitopp[], int aoiwidthp[], int aoiheightp[], int scandirectionp[], int bitdepthp[], int rsvd3[], 
int rsvd4[]);

// _cDcl(__declspec(dllexport),__stdcall,xclibs_s*)pxd_xclibEscape(int rsvd1,int rsvd2,int rsvd3);
// Uses advanced or unrecognized C constructs

// _cDcl(__declspec(dllexport),__stdcall,int)pxd_xclibEscaped(int rsvd1,int rsvd2,int rsvd3);
int __stdcall pxd_xclibEscaped(int rsvd1, int rsvd2, int rsvd3);

// _cDcl(__declspec(dllexport),__stdcall,xclibs_s*)pxe_xclibEscape(pxdstate_s*,int rsvd1,int rsvd2,int rsvd3);
// Uses advanced or unrecognized C constructs

// _cDcl(__declspec(dllexport),__stdcall,int)pxe_xclibEscaped(pxdstate_s*,int rsvd1,int rsvd2,int rsvd3);
// The `noname1' parameter has been declared expecting a pxe_XCLIBinstantiate() instance to be passed
int __stdcall pxe_xclibEscaped(void* , int rsvd1, int rsvd2, int rsvd3);
