#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
typedef unsigned int u32;
typedef unsigned short int u16;

# define SHELL_STRINGIFY_(f) #f
# define SHELL_STRINGIFY(f) SHELL_STRINGIFY_(f)
#ifdef SQLITE_CUSTOM_INCLUDE
# include SHELL_STRINGIFY(SQLITE_CUSTOM_INCLUDE)
#endif

#if !defined(SQLITE_OS_WINRT)
# define SQLITE_OS_WINRT 0
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 4054)
#pragma warning(disable : 4055)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4130)
#pragma warning(disable : 4152)
#pragma warning(disable : 4189)
#pragma warning(disable : 4206)
#pragma warning(disable : 4210)
#pragma warning(disable : 4232)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4306)
#pragma warning(disable : 4702)
#pragma warning(disable : 4706)
#endif

#if (defined(__RTP__) || defined(_WRS_KERNEL)) && !SQLITE_OMIT_LOAD_EXTENSION
#define SQLITE_OMIT_LOAD_EXTENSION  1
#endif

#ifndef SQLITE_DISABLE_LFS
#define _LARGE_FILE
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#define _LARGEFILE_SOURCE  1
#endif

#if defined(SQLITE_SHELL_FIDDLE) && !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE
#endif

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<assert.h>
#include<math.h>
#include "sqlite3.h"
typedef sqlite3_int64 i64;
typedef sqlite3_uint64 u64;
typedef unsigned char u8;
#if SQLITE_USER_AUTHENTICATION
#include "sqlite3userauth.h"
#endif
#include<ctype.h>
#include<stdarg.h>

#if !defined(_WIN32) && !defined(WIN32)
#include<signal.h>
#if !defined(__RTP__) && !defined(_WRS_KERNEL) && !defined(SQLITE_WASI)
#include <pwd.h>
#endif
#if(!defined(_WIN32) && !defined(WIN32)) || define(__MINGW32__)
#include<unistd.h>
#include<dirent.h>
#define GETPID getpid
#if defined(__MINGW32__)
#define DIRENT dirent
#ifndef S_ISLNK
#define S_ISLNK(mode) (0)
#endif
#endif
#else
#define GETPID(int)GetCurrentProcessId
#endif
#include<sys/types.h>
#include<sys/stat.h>

if HAVE_READLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif

#if HAVE_EDITLINE || HAVE_READLINE
#define shell_add_history(X) add_history(X)
#define shell_read_history(X) read_history(X)
#define shell_write_history(X) write_history(X)
#define shell_stifle_history(X) stifle_history(X)
#define shell_readline(X) readline(X)

#elif HAVE_LINENOISE

# include "linenoise.h"
# define shell_add_history(X) linenoiseHistoryAdd(X)
# define shell_read_history(X) linenoiseHistoryLoad(X)
# define shell_write_history(X) linenoiseHistorySave(X)
# define shell_stifle_history(X) linenoiseHistorySetMaxLen(X)
# define shell_readline(X) linenoise(X)

#else

#define shell_read_history(X)
#define shell_write_history(X)
#define shell_stifle_history(X)

# define SHELL_USE_LOCAL_GETLINE 1
#endif

#ifndef deliberate_fall_through
# if defined(GCC_VERSION) && GCC_VERSION>=7000000
#  define deliberate_fall_through __attribute__((fallthrough));
# else
#  define deliberate_fall_through
# endif
#endif

#if defined(_WIN32) || defined(WIN32)
# if SQLITE_OS_WINRT
#  define SQLITE_OMIT_POPEN 1
# else
#  include <io.h>
#  include <fcntl.h>
#  define isatty(h) _isatty(h)
#  ifndef access
#   define access(f,m) _access((f),(m))
#  endif
#  ifndef unlink
#   define unlink _unlink
#  endif
#  ifndef strdup
#   define strdup _strdup
#  endif
#  undef popen
#  define popen _popen
#  undef pclose
#  define pclose _pclose
# endif
#else

 extern int isatty(int);

# if !defined(__RTP__) && !defined(_WRS_KERNEL) && !defined(SQLITE_WASI)
  /* popen and pclose are not C89 functions and so are
  ** sometimes omitted from the <stdio.h> header */
   extern FILE *popen(const char*,const char*);
   extern int pclose(FILE*);
# else
#  define SQLITE_OMIT_POPEN 1
# endif
#endif

#if defined(_WIN32_WCE)
#define isatty(x) 1
#endif

#define IsSpace(X)  isspace((unsigned char)X)
#define IsDigit(X)  isdigit((unsigned char)X)
#define ToLower(X)  (char)tolower((unsigned char)X)

#if defined(_WIN32) || defined(WIN32)
#if SQLITE_OS_WINRT
#include <intrin.h>
#endif
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern char *sqlite3_win32_unicode_to_utf8(LPCWSTR);
extern LPWSTR sqlite3_win32_utf8_to_unicode(const char *zText);
#endif

#define SQLITE_INTERNAL_LINKAGE static

#ifdef SQLITE_SHELL_FIDDLE
/* Deselect most features from the console I/O package for Fiddle. */
# define SQLITE_CIO_NO_REDIRECT
# define SQLITE_CIO_NO_CLASSIFY
# define SQLITE_CIO_NO_TRANSLATE
# define SQLITE_CIO_NO_SETMODE
#endif

#define HAVE_CONSOLE_IO_H 1
#ifndef SQLITE_INTERNAL_LINKAGE
# define SQLITE_INTERNAL_LINKAGE extern
# include <stdio.h>
#else
# define SHELL_NO_SYSINC
#endif

#ifndef SQLITE3_H
#endif

#ifndef SQLITE_CIO_NO_CLASSIFY

typedef enum StreamAreConsole{
    SAC_NoConsole = 0,
    SAC_InConsole = 1, SAC_OutConsole = 2, SAC_ErrConsole = 4,
    SAC_AnyConsole = 0x7
}StreamsAreConsole;

SQLITE_INTERNAL_LINKAGE StreamsAreConsole
consoleClassifySetup(FILE *pfIn, FILE *pfOut, FILE *pfErr);
#define SQLITE_STD_CONSOLE_INIT() consoleClassifySetup(stdin, stdout, stderr)

SQLITE_INITERNAL_LINKAGE void consoleRenewSetup(void);

#else
#define consoleClassifySetup(i,o,e)
#define consoleRenewSetup()
#define consoleRestore()
#endif

#ifndef SQLITE_CIO_NO_REDIRECT

SQLITE_INTERNAL_LINKAGE FILE *invalidFileStream;
SQLITE_INTERNAL_LINKAGE FILE *setOutputStream(FILE *pf);
#ifdef CONSIO_SET_ERROR_STREAM
SQLITE_INTERNAL_LINKAGE FILE *setErrorStream(FILE *pf);
#endif
#else
#define setOutputStream(pf)
#define setErrorStream(pf)
#endif

#ifndef SQLITE_CIO_NO_TRANSLATE

SQLITE_INTERNAL_LINKAGE int fPrintfUtf8(FILE *pfO, const char *zFormat, ...);
SQLITE_INTERNAL_LINKAGE int oPrintfUtf8(const char *zFormat, ...);
SQLITE_INTERNAL_LINKAGE int ePrintfUtf8(const char *zFormat, ...);

SQLITE_INTERNAL_LINKAGE int fPutsUtf8(const char *z, FILE *pfO);
SQLITE_INTERNAL_LINKAGE int oPutsUtf8(const char *z);
SQLITE_INTERNAL_LINKAGE int ePutsUtf8(const char *z);

#ifdef CONSIO_SPUTB
SQLITE_INTERNAL_LINKAGE int
fPutbUtf8(FILE *pfOut, const char *cBuf, int nAccept);
#endif
SQLITE_INTERNAL_LINKAGE int
oPutbUtf8(const char *cBuf, int nAccept);
#ifdef CONSIO_EPUTB
SQLITE_INTERNAL_LINKAGE int
ePutbUtf8(const char *cBuf, int nAccept);
#endif

SQLITE_INTERNAL_LINKAGE char* fGetsUtf8(char *cBuf, int ncMax, FILE *pfIn);

#endif
#ifndef SQLITE_CIO_NO_SETMODE

SQLITE_INTERNAL_LINKAGE void setBinaryMode(FILE *, short bFlush);
SQLITE_INTERNAL_LINKAGE void setTextMode(FILE *, short bFlush);
#endif

#ifdef SQLITE_CIO_PROMPTED_IN
typedef struct Prompts{
    int numPrompts;
    const char **azPrompts;
}Prompts;

SQLITE_INTERNAL_LINKAGE char *
shellGetLine(FILE *pfIn, char *zBufPrior, int nLen, short isContinuation, Prompts azPrompt);
#endif

#if !(defined(SQLITE_CIO_NO_UTF8SCAN) && defined(SQLITE_CIO_NO_TRANSLATE))
SQLITE_INTERNAL_LINKAGE const char*
zSkipValidUtf8(const char *z, int nAccept, long ccm);

#endif

#ifndef SQLITE_CDECL
# define SQLITE_CDECL
#endif

#ifndef SHELL_NO_SYSINC
# include <stdarg.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <assert.h>
#endif
#ifndef HAVE_CONSOLE_IO_H
#include "console_io.h"
#endif
#if defined(_MSC_VER)
#pragma warning(disable : 4204)
#endif

#ifndef SQLITE_CIO_NO_TRANSLATE
#if(defined(_WIN32) || defined(WIN32)) && !SQLITE_OS_WINRT
#ifndef SHELL_NO_SYSINC
# include <io.h>
# include <fcntl.h>
# undef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif
#  define CIO_WIN_WC_XLATE 1
# else
#  ifndef SHELL_NO_SYSINC
#   include <unistd.h>
#  endif
#  define CIO_WIN_WC_XLATE 0
# endif
#else
# define CIO_WIN_WC_XLATE 0
#endif
#if CIO_WIN_WC_XLATE
static WCHAR cBadGroup = 0xfffd;
#endif

#if CIO_WIN_WC_XLATE
static HANDLE handleOfFile(FILE *pf){
  int fileDesc = _fileno(pf);
  union { intptr_t osfh; HANDLE fh; } fid = {
    (fileDesc>=0)? _get_osfhandle(fileDesc) : (intptr_t)INVALID_HANDLE_VALUE
  };
  return fid.fh;
}
#endif

#ifndef SQLITE_CIO_NO_TRANSLATE
typedef struct PerStreamTags{
# if CIO_WIN_WC_XLATE
  HANDLE hx;
  DWORD consMode;
  char acIncomplete[4];
# else
  short reachesConsole;
# endif
  FILE *pf;
}PerStreamTags;

# define SHELL_INVALID_FILE_PTR ((FILE *)~0)
# if CIO_WIN_WC_XLATE
#  define SHELL_INVALID_CONS_MODE 0xFFFF0000
# endif

# if CIO_WIN_WC_XLATE
#  define PST_INITIALIZER { INVALID_HANDLE_VALUE, SHELL_INVALID_CONS_MODE, \
      {0,0,0,0}, SHELL_INVALID_FILE_PTR }
# else
#  define PST_INITIALIZER { 0, SHELL_INVALID_FILE_PTR }
# endif

# if CIO_WIN_WC_XLATE
static short pstReachesConsole(PerStreamTags *ppst){
  return (ppst->hx != INVALID_HANDLE_VALUE);
}
# else
#  define pstReachesConsole(ppst) 0
# endif

# if CIO_WIN_WC_XLATE
static void restoreConsoleArb(PerStreamTags *ppst){
  if( pstReachesConsole(ppst) ) SetConsoleMode(ppst->hx, ppst->consMode);
}
# else
#  define restoreConsoleArb(ppst)
# endif

static short streamOfConsole(FILE *of, PerStreamTags *ppst){
    #if CIO_WIN_WC_XLATE
        short rv = 0;
        DWORD dwCM = SHELL_INVALID_CONS_MODE;
        HANDLE fh = handleOfFile(pf);
        ppst->pf = pf;
        if(INVALID_HANDLE_VALUE != fh){
            rv = (GetFileType(fh) == FILE_TYPE_CHAR && GetConsoleMode(fh, &dwCM));
        }
        ppst->hx = (rv) ? fh : INVALID_HANDLE_VALUE;
        ppst->consMode = dwCM;
        return rv;
    #else
        ppst->pf = pf;
        ppst->reachesConsole = ((short)isatty(fileno(pf)));
        return ppst->reachesConsole;
    #endif
}

# ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#  define ENABLE_VIRTUAL_TERMINAL_PROCESSING  (0x4)
# endif

# if CIO_WIN_WC_XLATE
# define SHELL_CONI_MODE \
  (ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT | 0x80 \
  | ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT)
# define SHELL_CONO_MODE (ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT \
  | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
# endif

typedef struct ConsoleInfo {
  PerStreamTags pstSetup[3];
  PerStreamTags pstDesignated[3];
  StreamsAreConsole sacSetup;
} ConsoleInfo;

static short isValidStreamInfo(PerStreamTags *ppst){
  return (ppst->pf != SHELL_INVALID_FILE_PTR);
}

static ConsoleInfo consoleInfo = {
  { /* pstSetup */ PST_INITIALIZER, PST_INITIALIZER, PST_INITIALIZER },
  { /* pstDesignated[] */ PST_INITIALIZER, PST_INITIALIZER, PST_INITIALIZER },
  SAC_NoConsole /* sacSetup */
};

SQLITE_INTERNAL_LINKAGE FILE* invalidFileStream = (FILE *)~0;

# if CIO_WIN_WC_XLATE
static void maybeSetupAsConsole(PerStreamTags *ppst, short odir){
  if( pstReachesConsole(ppst) ){
    DWORD cm = odir? SHELL_CONO_MODE : SHELL_CONI_MODE;
    SetConsoleMode(ppst->hx, cm);
  }
}
# else
#  define maybeSetupAsConsole(ppst,odir)
# endif

SQLITE_INTERNAL_LINKAGE void consoleRenewSetup(void){
    #if CIO_WIN_WC_XLATE
        int ix = 0;
        while(ix < 6){
            PerStreamTags *ppst = (ix<3) ?
            &consoleInfo.pstSetup[ix] : &consoleInfo.pstDesignated[ix-3];
        maybeSetupAsConsole(ppst, (ix % 3) > 0);
        ++ix;
        }
    #endif
}

SQLITE_INTERNAL_LINKAGE StreamAreConsole
consoleClassifySetup(FILE *pfIn, FILE *pfOut, FILE *pfErr){
    StreamsAreConsole rv = SAC_NoConsole;
    FILE* apf[3] = { pfIn, pfOut, pfErr };
    int ix;
    for(ix = 2; ix >= 0; --ix){
        PerStreamTags *ppst = &consoleInfo.pstSetup[ix];
        if(streamOfConsole(apf[ix], ppst)){
            rv |= (SAC_InConsole >> ix);
        }
        consoleInfo.pstDesignated[ix] = *ppst;
        if(ix > 0) fflush(apf[ix]);
    }
    consoleInfo.sacSetup = rv;
    consoleRenewSetup();
    return rv;
}
SQLITE_INTERNAL_LINKAGE void SQLITE_CDECL consoleRestore(void){
# if CIO_WIN_WC_XLATE
  static ConsoleInfo *pci = &consoleInfo;
  if( pci->sacSetup ){
    int ix;
    for( ix=0; ix<3; ++ix ){
      if( pci->sacSetup & (SAC_InConsole<<ix) ){
        PerStreamTags *ppst = &pci->pstSetup[ix];
        SetConsoleMode(ppst->hx, ppst->consMode);
      }
    }
  }
# endif
}
#endif

#ifdef SQLITE_CIO_INPUT_REDIR

static PerStreamTags * isKnownWritable(FILE *pf){
  static PerStreamTags *apst[] = {
    &consoleInfo.pstDesignated[1], &consoleInfo.pstDesignated[2],
    &consoleInfo.pstSetup[1], &consoleInfo.pstSetup[2], 0
  };
  int ix = 0;
  do {
    if( apst[ix]->pf == pf ) break;
  } while( apst[++ix] != 0 );
  return apst[ix];
}

static FILE *designateEmitStream(FILE *pf, unsigned chix){
  FILE *rv = consoleInfo.pstDesignated[chix].pf;
  if( pf == invalidFileStream ) return rv;
  else{
    /* Setting a possibly new output stream. */
    PerStreamTags *ppst = isKnownWritable(pf);
    if( ppst != 0 ){
      PerStreamTags pst = *ppst;
      consoleInfo.pstDesignated[chix] = pst;
    }else streamOfConsole(pf, &consoleInfo.pstDesignated[chix]);
  }
  return rv;
}

SQLITE_INTERNAL_LINKAGE FILE *setOutputStream(FILE *pf){
  return designateEmitStream(pf, 1);
}
# ifdef CONSIO_SET_ERROR_STREAM
SQLITE_INTERNAL_LINKAGE FILE *setErrorStream(FILE *pf){
  return designateEmitStream(pf, 2);
}
# endif
#endif

#ifndef SQLITE_CIO_NO_SETMODE
# if CIO_WIN_WC_XLATE
static void setModeFlushQ(FILE *pf, short bFlush, int mode){
  if( bFlush ) fflush(pf);
  _setmode(_fileno(pf), mode);
}
# else
#  define setModeFlushQ(f, b, m) if(b) fflush(f)
# endif

SQLITE_INTERNAL_LINKAGE void setBinaryMode(FILE *pf, short bFlush){
  setModeFlushQ(pf, bFlush, _O_BINARY);
}
SQLITE_INTERNAL_LINKAGE void setTextMode(FILE *pf, short bFlush){
  setModeFlushQ(pf, bFlush, _O_TEXT);
}
# undef setModeFlushQ

#else
#define setBinaryMode(f, bFlush) do{ if((bFlush)) fflush(f); }while(0)
#define setTextMode(f, bFlush) do{if((bFlush)) fflush(f); }while(0)
#endif

#ifndef SQLITE_CIO_NO_TRANSLATE
#if CIO_WIN_WC_XLATE

static int conZstrEmit(PerStreamTags *ppst, const char *z, int ncTake){
  int rv = 0;
  if( z!=NULL ){
    int nwc = MultiByteToWideChar(CP_UTF8,0, z,ncTake, 0,0);
    if( nwc > 0 ){
      WCHAR *zw = sqlite3_malloc64(nwc*sizeof(WCHAR));
      if( zw!=NULL ){
        nwc = MultiByteToWideChar(CP_UTF8,0, z,ncTake, zw,nwc);
        if( nwc > 0 ){
          /* Translation from UTF-8 to UTF-16, then WCHARs out. */
          if( WriteConsoleW(ppst->hx, zw,nwc, 0, NULL) ){
            rv = ncTake;
          }
        }
        sqlite3_free(zw);
      }
    }
  }
  return rv;
}

static int conioVmPrintf(PerStreamTags *ppst, const char *zFormat, va_list ap){
  char *z = sqlite3_vmprintf(zFormat, ap);
  if( z ){
    int rv = conZstrEmit(ppst, z, (int)strlen(z));
    sqlite3_free(z);
    return rv;
  }else return 0;
}
#endif

# ifdef CONSIO_GET_EMIT_STREAM
static PerStreamTags * getDesignatedEmitStream(FILE *pf, unsigned chix,
                                               PerStreamTags *ppst){
  PerStreamTags *rv = isKnownWritable(pf);
  short isValid = (rv!=0)? isValidStreamInfo(rv) : 0;
  if( rv != 0 && isValid ) return rv;
  streamOfConsole(pf, ppst);
  return ppst;
}
# endif

static PerStreamTags *
getEmitStreamInfo(unsigned chix, PerStreamTags *ppst,
                  /* in/out */ FILE **ppf){
  PerStreamTags *ppstTry;
  FILE *pfEmit;
  if( chix > 0 ){
    ppstTry = &consoleInfo.pstDesignated[chix];
    if( !isValidStreamInfo(ppstTry) ){
      ppstTry = &consoleInfo.pstSetup[chix];
      pfEmit = ppst->pf;
    }else pfEmit = ppstTry->pf;
    if( !isValidStreamInfo(ppstTry) ){
      pfEmit = (chix > 1)? stderr : stdout;
      ppstTry = ppst;
      streamOfConsole(pfEmit, ppstTry);
    }
    *ppf = pfEmit;
  }else{
    ppstTry = isKnownWritable(*ppf);
    if( ppstTry != 0 ) return ppstTry;
    streamOfConsole(*ppf, ppst);
    return ppst;
  }
  return ppstTry;
}

SQLITE_INTERNAL_LINKAGE int oPrintfUtf8(const char *zFormat, ...){
  va_list ap;
  int rv;
  FILE *pfOut;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(1, &pst, &pfOut);
# else
  getEmitStreamInfo(1, &pst, &pfOut);
# endif
  assert(zFormat!=0);
  va_start(ap, zFormat);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    rv = conioVmPrintf(ppst, zFormat, ap);
  }else{
# endif
    rv = vfprintf(pfOut, zFormat, ap);
# if CIO_WIN_WC_XLATE
  }
# endif
  va_end(ap);
  return rv;
}

SQLITE_INTERNAL_LINKAGE int ePrintfUtf8(const char *zFormat, ...){
  va_list ap;
  int rv;
  FILE *pfErr;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(2, &pst, &pfErr);
# else
  getEmitStreamInfo(2, &pst, &pfErr);
# endif
  assert(zFormat!=0);
  va_start(ap, zFormat);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    rv = conioVmPrintf(ppst, zFormat, ap);
  }else{
# endif
    rv = vfprintf(pfErr, zFormat, ap);
# if CIO_WIN_WC_XLATE
  }
# endif
  va_end(ap);
  return rv;
}

SQLITE_INTERNAL_LINKAGE int fPrintfUtf8(FILE *pfO, const char *zFormat, ...){
  va_list ap;
  int rv;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(0, &pst, &pfO);
# else
  getEmitStreamInfo(0, &pst, &pfO);
# endif
  assert(zFormat!=0);
  va_start(ap, zFormat);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    maybeSetupAsConsole(ppst, 1);
    rv = conioVmPrintf(ppst, zFormat, ap);
    if( 0 == isKnownWritable(ppst->pf) ) restoreConsoleArb(ppst);
  }else{
# endif
    rv = vfprintf(pfO, zFormat, ap);
# if CIO_WIN_WC_XLATE
  }
# endif
  va_end(ap);
  return rv;
}

SQLITE_INTERNAL_LINKAGE int fPutsUtf8(const char *z, FILE *pfO){
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(0, &pst, &pfO);
# else
  getEmitStreamInfo(0, &pst, &pfO);
# endif
  assert(z!=0);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    int rv;
    maybeSetupAsConsole(ppst, 1);
    rv = conZstrEmit(ppst, z, (int)strlen(z));
    if( 0 == isKnownWritable(ppst->pf) ) restoreConsoleArb(ppst);
    return rv;
  }else {
# endif
    return (fputs(z, pfO)<0)? 0 : (int)strlen(z);
# if CIO_WIN_WC_XLATE
  }
# endif
}

SQLITE_INTERNAL_LINKAGE int ePutsUtf8(const char *z){
  FILE *pfErr;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(2, &pst, &pfErr);
# else
  getEmitStreamInfo(2, &pst, &pfErr);
# endif
  assert(z!=0);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ) return conZstrEmit(ppst, z, (int)strlen(z));
  else {
# endif
    return (fputs(z, pfErr)<0)? 0 : (int)strlen(z);
# if CIO_WIN_WC_XLATE
  }
# endif
}

SQLITE_INTERNAL_LINKAGE int oPutsUtf8(const char *z){
  FILE *pfOut;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(1, &pst, &pfOut);
# else
  getEmitStreamInfo(1, &pst, &pfOut);
# endif
  assert(z!=0);
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ) return conZstrEmit(ppst, z, (int)strlen(z));
  else {
# endif
    return (fputs(z, pfOut)<0)? 0 : (int)strlen(z);
# if CIO_WIN_WC_XLATE
  }
# endif
}

#endif

#if !(defined(SQLITE_CIO_NO_UTF8SCAN) && defined(SQLITE_CIO_NO_TRANSLATE))
SQLITE_INTERNAL_LINKAGE const char*
zSkipValidUtf8(const char *z, int nAccept, long ccm){
    int ng = (nAccept < 0) ? -nAccept : 0;
    const char *pcLimit = (nAccept >= 0) ? z+nAccept : 0;
    assert(z!=0);
    while((pcLimit) ? (z<pcLimit) : (ng-- != 0)){
        char c = *z;
        if((c & 0x80) == 0){
            if(ccm != 0L && c < 0x20 && ((1L<<c) & ccm) != 0) return z;
            ++z;
        }else if((c & 0xC0) != 0xC0) return z;
        else{
            const char *zt = z+1;
            do{
                if(pcLimit && zt >= pcLimit) return z;
                else{
                    char ct = *zt++;
                    if(ct == 0 || (zt-z) > 4 || (ct & 0xc0) != 0x80){
                        return z;
                    }
                }
            }while(((c <<= 1) & 0x40) == 0x40);
            z = zt;
        }
    }
    return z;
}
#endif

#ifndef SQLITE_CIO_NO_TRANSLATE
# ifdef CONSIO_SPUTB
SQLITE_INTERNAL_LINKAGE int
fPutbUtf8(FILE *pfO, const char *cBuf, int nAccept){
  assert(pfO!=0);
#  if CIO_WIN_WC_XLATE
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
  PerStreamTags *ppst = getEmitStreamInfo(0, &pst, &pfO);
  if( pstReachesConsole(ppst) ){
    int rv;
    maybeSetupAsConsole(ppst, 1);
    rv = conZstrEmit(ppst, cBuf, nAccept);
    if( 0 == isKnownWritable(ppst->pf) ) restoreConsoleArb(ppst);
    return rv;
  }else {
#  endif
    return (int)fwrite(cBuf, 1, nAccept, pfO);
#  if CIO_WIN_WC_XLATE
  }
#  endif
}
# endif

SQLITE_INTERNAL_LINKAGE int
oPutbUtf8(const char *cBuf, int nAccept){
  FILE *pfOut;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
# if CIO_WIN_WC_XLATE
  PerStreamTags *ppst = getEmitStreamInfo(1, &pst, &pfOut);
# else
  getEmitStreamInfo(1, &pst, &pfOut);
# endif
# if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    return conZstrEmit(ppst, cBuf, nAccept);
  }else {
# endif
    return (int)fwrite(cBuf, 1, nAccept, pfOut);
# if CIO_WIN_WC_XLATE
  }
# endif
}

# ifdef CONSIO_EPUTB
SQLITE_INTERNAL_LINKAGE int
ePutbUtf8(const char *cBuf, int nAccept){
  FILE *pfErr;
  PerStreamTags pst = PST_INITIALIZER; /* for unknown streams */
  PerStreamTags *ppst = getEmitStreamInfo(2, &pst, &pfErr);
#  if CIO_WIN_WC_XLATE
  if( pstReachesConsole(ppst) ){
    return conZstrEmit(ppst, cBuf, nAccept);
  }else {
#  endif
    return (int)fwrite(cBuf, 1, nAccept, pfErr);
#  if CIO_WIN_WC_XLATE
  }
#  endif
}
# endif

SQLITE_INTERNAL_LINKAGE char* fGetsUtf8(char *cBuf, int ncMax, FILE *pfIn){
  if( pfIn==0 ) pfIn = stdin;
# if CIO_WIN_WC_XLATE
  if( pfIn == consoleInfo.pstSetup[0].pf
      && (consoleInfo.sacSetup & SAC_InConsole)!=0 ){
#  if CIO_WIN_WC_XLATE==1
#   define SHELL_GULP 150
    WCHAR wcBuf[SHELL_GULP+1];
    int lend = 0, noc = 0;
    if( ncMax > 0 ) cBuf[0] = 0;
    while( noc < ncMax-8-1 && !lend ){
        int na = (ncMax > SHELL_GULP*4+1 + noc)? SHELL_GULP : (ncMax-1 - noc)/4;
#   undef SHELL_GULP
      DWORD nbr = 0;
      BOOL bRC = ReadConsoleW(consoleInfo.pstSetup[0].hx, wcBuf, na, &nbr, 0);
      if( bRC && nbr>0 && (wcBuf[nbr-1]&0xF800)==0xD800 ){
        DWORD nbrx;
        bRC &= ReadConsoleW(consoleInfo.pstSetup[0].hx, wcBuf+nbr, 1, &nbrx, 0);
        if( bRC ) nbr += nbrx;
      }
      if( !bRC || (noc==0 && nbr==0) ) return 0;
      if( nbr > 0 ){
        int nmb = WideCharToMultiByte(CP_UTF8, 0, wcBuf,nbr,0,0,0,0);
        if( nmb != 0 && noc+nmb <= ncMax ){
          int iseg = noc;
          nmb = WideCharToMultiByte(CP_UTF8, 0, wcBuf,nbr,cBuf+noc,nmb,0,0);
          noc += nmb;
          if( noc > 0 ){
            if( cBuf[noc-1]=='\n' ){
              lend = 1;
              if( noc > 1 && cBuf[noc-2]=='\r' ) cBuf[--noc-1] = '\n';
            }
          }
          while( iseg < noc ){
            if( cBuf[iseg]=='\x1a' ){
              noc = iseg; /* Chop ^Z and anything following. */
              lend = 1; /* Counts as end of line too. */
              break;
            }
            ++iseg;
          }
        }else break; /* Drop apparent garbage in. (Could assert.) */
      }else break;
    }
    if( noc > 0 ){
        cBuf[noc] = 0;
        return cBuf;
    }else return 0;
#  endif
    }else{
# endif
    return fgets(cBuf, ncMax, pfIn);
# if CIO_WIN_WC_XLATE
  }
# endif
}
#endif

#if defined(_MSC_VER)
# pragma warning(default : 4204)
#endif

#undef SHELL_INVALID_FILE_PTR

#ifndef SQLITE_SHELL_FIDDLE

# define fgets(b,n,f) fGetsUtf8(b,n,f)

# define sputz(s,z) fPutsUtf8(z,s)
# define sputf fPrintfUtf8
# define oputz(z) oPutsUtf8(z)
# define oputf oPrintfUtf8
# define eputz(z) ePutsUtf8(z)
# define eputf ePrintfUtf8
# define oputb(buf,na) oPutbUtf8(buf,na)

#else
# define sputz(fp,z) fputs(z,fp)
# define sputf(fp,fmt, ...) fprintf(fp,fmt,__VA_ARGS__)
# define oputz(z) fputs(z,stdout)
# define oputf(fmt, ...) printf(fmt,__VA_ARGS__)
# define eputz(z) fputs(z,stderr)
# define eputf(fmt, ...) fprintf(stderr,fmt,__VA_ARGS__)
# define oputb(buf,na) fwrite(buf,1,na,stdout)
#endif

static int enableTimer = 0;

static int cli_strcmp(const char *a, const char *b){
  if( a==0 ) a = "";
  if( b==0 ) b = "";
  return strcmp(a,b);
}
static int cli_strncmp(const char *a, const char *b, size_t n){
  if( a==0 ) a = "";
  if( b==0 ) b = "";
  return strncmp(a,b,n);
}

static sqlite3_int64 timeOfDay(void){
  static sqlite3_vfs *clockVfs = 0;
  sqlite3_int64 t;
  if( clockVfs==0 ) clockVfs = sqlite3_vfs_find(0);
  if( clockVfs==0 ) return 0;  /* Never actually happens */
  if( clockVfs->iVersion>=2 && clockVfs->xCurrentTimeInt64!=0 ){
    clockVfs->xCurrentTimeInt64(clockVfs, &t);
  }else{
    double r;
    clockVfs->xCurrentTime(clockVfs, &r);
    t = (sqlite3_int64)(r*86400000.0);
  }
  return t;
}

#if !defined(_WIN32) && !defined(WIN32) && !defined(__minux)
#include <sys/time.h>
#include <sys/resource.h>

#if defined(_WRS_KERNEL) || defined(__RTP__)
struct rusage {
  struct timeval ru_utime; /* user CPU time used */
  struct timeval ru_stime; /* system CPU time used */
};
#define getrusage(A,B) memset(B,0,sizeof(*B))
#endif

static struct rusage sBegin;
static sqlite3_int64 iBegin;

static void beginTimer(void){
  if( enableTimer ){
    getrusage(RUSAGE_SELF, &sBegin);
    iBegin = timeOfDay();
  }
}

static double timeDiff(struct timeVal *pStart, struct timeVal *pEnd){
    return (pEnd->tv_usec - pStart->tv_usec)*0.000001 + (double)(pEnd->tv_sec - pStart->tv_sec);
}

static void endTimer(void){
    if(enableTimer){
        sqlite3_int64 iEnd = timeOfDay();
        struct rusage sEnd;
        getrusage(RUSAGE_SELF, &sEnd);
        sputf(stdout, "RunTime: real %.3f user %f sys %f\n",
            (iEnd - iBegin) * 0.001,
            timeDiff(&sBegin.ru_utime, &sEnd.ru_utime),
            timeDiff(&sBegin.ru_stime, &sEnd.ru_stime));
    }
}

#define BEGIN_TIMER beginTimer()
#define END_TIMER endTimer()
#define HAS_TIMER 1

#elif (defined(_WIN32) || defined(WIN32))

static HANDLE hProcess;
static FILETIME ftKernelBegin;
static FILETIME ftUserBegin;
static sqlite3_int64 ftWallBegin;
typedef BOOL(WINAPI *GETPROCTIMES)(HANDLE, LPFILETIME, LPFILETIME,
                                    LPFILETIME, LPFILETIME);
static int hasTimer(void){
    if(getProcessTimesAddr){
        return 1;
    }else{
    #if !SQLITE_OS_WINRT
        hProcess = GetCurrentProcess();
        if(hProcess){
            HINSTANCE hinstLib = LoadLibrary(TEXT("Kernel32.dll"));
            if(NULL != hinstLib){
                getProcessTimesAddr = 
                    (GETPROCETIMES) GetProcAddress(hinstLib, "GetProcessTimes");
            if(NULL != getProcessTimesAddr){
                return 1;
            }
            FreeLibrary(hinstLib);
            }
        }
    #endif
    }
    return 0;
}

static void beginTimer(void){
    if(enableTimer && getProcessTimesAddr){
        FILETIME ftCreation, ftExit;
        getProcessTimesAddr(hProcess,&ftCreation,&ftExit,
                            &ftKernelBegin,&ftUserBegin);
        ftWallBegin = timeOfDay();
    }
}

static double timeDiff(FILETIME *pStart, FILETIME *pEnd){
    sqlite_int64 i64Start = *((sqlite_int64 *) pStart);
    sqlite_int64 i64End = *((sqlite_int64 *) pEnd);
    return (double)((i64End - i64Start) / 10000000.0);
}

static void endTimer(void){
    if(enableTimer && getProcessTimesAddr){
        FILETIME ftCreation, ftExit, ftKernelEnd, ftUserEnd;
        sqlite3_int64 ftWallEnd = timeOfDay();
        getProcessTimesAddr(hProcess, &ftCreation, &ftExit, &ftKernelEnd, &ftUserEnd);
        sputf(stdout, "Run Time: real %.3f user %f sys %f\n",
            (ftWallEnd - ftWallBegin)*0.001,
            timeDiff(&ftUserBegin, &ftUserEnd),
            timeDiff(&ftKernelBegin, &ftKernelEnd));
    }
}

#define BEGIN_TIMER beginTimer()
#define END_TIMER endTimer()
#define HAS_TIMER hasTimer()

#else
#define BEGIN_TIMER
#define END_TIMER
#define HAS_TIMER 0
#endif

#define UNUSED_PARAMETER(x) (void)(x)

#define ArraySize(X) (int)(sizeof(X)/sizeof(X[0]))

static int bail_on_error = 0;

static int stdin_is_interactive = 1;

static int stdout_is_console = 1;

static sqlite3 *globalDb = 0;

static volatile int seenInterrupt = 0;

static char *Argv0;

#define PROMPT_LEN_MAX 20

static char mainPrompt[PROMPT_LEN_MAX];

static char continuePrompt[PROMPT_LEN_MAX];

static char *shell_strncpy(char *dest, const char *src, size_t n){
    size_t i;
    for(i = 0; i < n-1 && src[i]!=0; ++i) dest[i] = src[i];
    dest[i] = 0;
    return dest;
}

#ifdef SQLITE_OMIT_DYNAPROMPT
# define CONTINUATION_PROMPT continuePrompt
# define CONTINUE_PROMPT_RESET
# define CONTINUE_PROMPT_AWAITS(p,s)
# define CONTINUE_PROMPT_AWAITC(p,c)
# define CONTINUE_PAREN_INCR(p,n)
# define CONTINUE_PROMPT_PSTATE 0
typedef void *t_NoDynaPrompt;
# define SCAN_TRACKER_REFTYPE t_NoDynaPrompt
#else
# define CONTINUATION_PROMPT dynamicContinuePrompt()
# define CONTINUE_PROMPT_RESET \
  do {setLexemeOpen(&dynPrompt,0,0); trackParenLevel(&dynPrompt,0);} while(0)
# define CONTINUE_PROMPT_AWAITS(p,s) \
  if(p && stdin_is_interactive) setLexemeOpen(p, s, 0)
# define CONTINUE_PROMPT_AWAITC(p,c) \
  if(p && stdin_is_interactive) setLexemeOpen(p, 0, c)
# define CONTINUE_PAREN_INCR(p,n) \
  if(p && stdin_is_interactive) (trackParenLevel(p,n))
# define CONTINUE_PROMPT_PSTATE (&dynPrompt)
typedef struct DynaPrompt *t_DynaPromptRef;
# define SCAN_TRACKER_REFTYPE t_DynaPromptRef

static struct DynaPrompt{
    char dynamicPrompt[PROMPT_LEN_MAX];
    char acAwait[2];
    int inParenLevel;
    char *zScannerAwaits;
}DynaPrompt = {{0}, {0}, 0, 0};

static void trackParenLevel(struct DynaPrompt *p, int ni){
    p->inParenLevel += ni;
    if(ni == 0) p->inParenLevel = 0;
    p->zScannerAwaits = 0;
}

static void setLexemeOpen(struct DynaPrompt *p, char *s, char c){
    if(s != 0 || c == 0){
        p->zScannerAwaits = s;
        p->acAwait[0] = 0;
    }else{
        p->acAwait[0] = c;
        p->zScannerAwaits = p->acAwait;
    }
}

static char *dynamicContinuePrompt(void){
    if(continuePrompt[0] == 0 
    || (dynPrompt.zScannerAwaits == 0 && dynPrompt.inParenLevel == 0)){
        return continuePrompt;
    }else{
        if( dynPrompt.zScannerAwaits ){
      size_t ncp = strlen(continuePrompt);
      size_t ndp = strlen(dynPrompt.zScannerAwaits);
      if( ndp > ncp-3 ) return continuePrompt;
      strcpy(dynPrompt.dynamicPrompt, dynPrompt.zScannerAwaits);
      while( ndp<3 ) dynPrompt.dynamicPrompt[ndp++] = ' ';
      shell_strncpy(dynPrompt.dynamicPrompt+3, continuePrompt+3,
              PROMPT_LEN_MAX-4);
    }else{
      if( dynPrompt.inParenLevel>9 ){
        shell_strncpy(dynPrompt.dynamicPrompt, "(..", 4);
      }else if( dynPrompt.inParenLevel<0 ){
        shell_strncpy(dynPrompt.dynamicPrompt, ")x!", 4);
      }else{
        shell_strncpy(dynPrompt.dynamicPrompt, "(x.", 4);
        dynPrompt.dynamicPrompt[2] = (char)('0'+dynPrompt.inParenLevel);
      }
      shell_strncpy(dynPrompt.dynamicPrompt+3, continuePrompt+3,
                    PROMPT_LEN_MAX-4);
    }
    }
    return dynPrompt.dynamicPrompt;
}
#endif

static void shell_out_of_memory(void){
    eputz("Error: out of memory\n");
    exit(1);
}

static void shell_check_oom(const void *p){
    if(p == 0) shell_out_of_memory();
}

#ifndef SQLITE_ENABLE_IOTRACE
static FILE *iotrace = 0;
#endif

#ifdef SQLITE_ENABLE_IOTRACE
static void SQLITE_CDECL iotracePrintf(const char*zFormat, ...){
    va_list ap;
    char *z;
    if(iotrace == 0) return;
    va_start(ap, zFormat);
    z = sqlite3_vmprintf(zFormat, ap);
    va_end(ap);
    sputf(iotrace,"%s", z);
    sqlite3_free(z);
}
#endif

static void utf8_width_print(int w, const char *zUtf){
    int i;
    int n;
    int aw = w<0?-w:w;
    if( zUtf==0 ) zUtf = "";
    for(i=n=0; zUtf[i]; i++){
        if( (zUtf[i]&0xc0)!=0x80 ){
        n++;
        if( n==aw ){
            do{ i++; }while( (zUtf[i]&0xc0)==0x80 );
            break;
        }
        }
    }
    if( n>=aw ){
        oputf("%.*s", i, zUtf);
    }else if( w<0 ){
        oputf("%*s%s", aw-n, "", zUtf);
    }else{
        oputf("%s%*s", zUtf, aw-n, "");
    }
}

static int isNumber(const char *z, int *realnum){
  if( *z=='-' || *z=='+' ) z++;
  if( !IsDigit(*z) ){
    return 0;
  }
  z++;
  if( realnum ) *realnum = 0;
  while( IsDigit(*z) ){ z++; }
  if( *z=='.' ){
    z++;
    if( !IsDigit(*z) ) return 0;
    while( IsDigit(*z) ){ z++; }
    if( realnum ) *realnum = 1;
  }
  if( *z=='e' || *z=='E' ){
    z++;
    if( *z=='+' || *z=='-' ) z++;
    if( !IsDigit(*z) ) return 0;
    while( IsDigit(*z) ){ z++; }
    if( realnum ) *realnum = 1;
  }
  return *z==0;
}

static int strlen30(const char *z){
  const char *z2 = z;
  while(*z2){z2++;}
  return 0x3fffffff & (int)(z2 - z);
}

static int strlenChar(const char *z){
  int n = 0;
  while(*z){
    if((0xc0&*(z++)) != 0x80) n++;
  }
  return n;
}

static FILE * openChrSource(const char *zFile){
#if defined(_WIN32) || defined(WIN32)
  struct __stat64 x = {0};
# define STAT_CHR_SRC(mode) ((mode & (_S_IFCHR|_S_IFIFO|_S_IFREG))!=0)
  FILE *rv = fopen(zFile, "rb");
  if( rv == 0) return 0;
  if( _fstat64(_fileno(rv), &x) != 0 || !STAT_CHR_SRC(x.st_mode)){
    fclose(rv);
    rv = 0;
  }
  return rv;
#else
  struct stat x = {0};
  int rc = stat(zFile, &x);
# define STAT_CHR_SRC(mode) (S_ISREG(mode)||S_ISFIFO(mode)||S_ISCHR(mode))
  if( rc!=0 ) return 0;
  if( STAT_CHR_SRC(x.st_mode) ){
    return fopen(zFile, "rb");
  }else{
    return 0;
  }
#endif
#undef STAT_CHR_SRC
}

static char *local_getline(char *zLine, FILE *in){
  int nLine = zLine==0 ? 0 : 100;
  int n = 0;
  while(1){
    if(n + 100 > nLine){
      nLine = nLine*2 + 100;
      zLine = realloc(zLine, nLine);
      shell_check_oom(zLine);
    }
    if(fgets(&zLine[n], nLine - n, in) == 0){
      if(n == 0) {
        free(zLine);
        return 0;
      }
      zLine[n] = 0;
      break;
    }
    while(zLine[n]) n++;
    if(n > 0 && zLine[n-1] == '\n'){
      n--;
      if(n > 0 && zLine[n-1] == '\r') n--;
      zLine[n] = 0;
      break;
    }
  }
  return zLine;

#ifndef SQLITE_SHELL_FIDDLE
static char *one_input_line(FILE *in, char *zPrior, int isContinuation){
  char *zPrompt;
  char *zResult;
  if( in!=0 ){
    zResult = local_getline(zPrior, in);
  }else{
    zPrompt = isContinuation ? CONTINUATION_PROMPT : mainPrompt;
#if SHELL_USE_LOCAL_GETLINE
    sputz(stdout, zPrompt);
    fflush(stdout);
    do{
      zResult = local_getline(zPrior, stdin);
      zPrior = 0;
      /* ^C trap creates a false EOF, so let "interrupt" thread catch up. */
      if( zResult==0 ) sqlite3_sleep(50);
    }while( zResult==0 && seenInterrupt>0 );
#else
    free(zPrior);
    zResult = shell_readline(zPrompt);
    while( zResult==0 ){
      /* ^C trap creates a false EOF, so let "interrupt" thread catch up. */
      sqlite3_sleep(50);
      if( seenInterrupt==0 ) break;
      zResult = shell_readline("");
    }
    if( zResult && *zResult ) shell_add_history(zResult);
#endif
  }
  return zResult;
}
#endif

static int hexDigitValue(char c){
  if(c >= '0' && c <= '9') return c - '0';
  if(c >= 'a' && c <= 'f') return c - 'a' + 10;
  if(c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

static sqlite3_int64 integerValue(const char *zArg){
  sqlite3_int64 v = 0;
  static const struct{ char *zSuffix; int iMult; } aMult[] = {
    { "KiB", 1024},
    { "MiB", 1024*1024},
    { "GiB", 1024*1024*1024 },
    { "KB",  1000 },
    { "MB",  1000000 },
    { "GB",  1000000000 },
    { "K",   1000 },
    { "M",   1000000 },
    { "G",   1000000000 },
  };
  int i;
  int isNeg = 0;
  if( zArg[0]=='-' ){
    isNeg = 1;
    zArg++;
  }else if( zArg[0]=='+' ){
    zArg++;
  }
  if( zArg[0]=='0' && zArg[1]=='x' ){
    int x;
    zArg += 2;
    while( (x = hexDigitValue(zArg[0]))>=0 ){
      v = (v<<4) + x;
      zArg++;
    }
  }else{
    while( IsDigit(zArg[0]) ){
      v = v*10 + zArg[0] - '0';
      zArg++;
    }
  }
  for(i=0; i<ArraySize(aMult); i++){
    if( sqlite3_stricmp(aMult[i].zSuffix, zArg)==0 ){
      v *= aMult[i].iMult;
      break;
    }
  }
  return isNeg? -v : v;
}

typedef struct ShellText ShellText;
struct ShellText{
  char *z;
  int n,
  int nAlloc;
};

static void initText(ShellText *p){
  memset(p, 0, sizeof(*p));
}
static void freeText(ShellText *p){
  free(p->z);
  initText(p);
}

static void appendText(ShellText *p, const char *zAppend, char quote){
  i64 len;
  i64 i;
  i64 nAppend = strlen30(zAppend);

  len = nAppend + p->n+1;
  if(quote){
    len += 2；
    for(i = 0; i < nAppend; i++){
      if(zAppend[i] == quote) len++;
    }
  }
  if(p->z == 0 || p->n+len >= p->nAlloc){
    p->nAlloc = p->nAlloc*2 + len+20;
    p->z = realloc(p->z, p->nAlloc);
    shell_check_oom(p->z);
  }
  if( quote ){
    char *zCsr = p->z+p->n;
    *zCsr++ = quote;
    for(i=0; i<nAppend; i++){
      *zCsr++ = zAppend[i];
      if( zAppend[i]==quote ) *zCsr++ = quote;
    }
    *zCsr++ = quote;
    p->n = (int)(zCsr - p->z);
    *zCsr = '\0';
  }else{
    memcpy(p->z+p->n, zAppend, nAppend);
    p->n += nAppend;
    p->z[p->n] = '\0';
  }
}

static char quoteChar(const char *zName){
  int i;
  if( zName==0 ) return '"';
  if( !isalpha((unsigned char)zName[0]) && zName[0]!='_' ) return '"';
  for(i=0; zName[i]; i++){
    if( !isalnum((unsigned char)zName[i]) && zName[i]!='_' ) return '"';
  }
  return sqlite3_keyword_check(zName, i) ? '"' : 0;
}

static char *shellFakeSchema(
  sqlite3 *db,
  const char *zSchema,
  const char *zName
){
  sqlite3_stmt *pStmt = 0;
  char *zSql;
  ShellText s;
  cahr cQuote;
  char *zDiv = "(";
  int nRow = 0;

  zSql = sqlite3_mprintf("PRAGMA \"%w\".table_info=%Q;",
                        zSchema ? zSchema : "main", zName);
  shell_check_oom(zSql);
  sqlite3_prepare_v2(db, zSql, -1, &pStmt, 0);
  sqlite3_free(zSql);
  initText(&s);
  if(zSchema){
    cQuote = quoteChar(zSchema);
    if(cQuote && sqlite3_stricmp(zSchema, "temp") == 0) cQuote = 0;
    appendText(&s, zSchema, cQuote);
    appendText(&s, ".", 0);
  }
  cQuote = quoteChar(zName);
  appendText(&s, zName, cQuote);
  while( sqlite3_step(pStmt)==SQLITE_ROW ){
    const char *zCol = (const char*)sqlite3_column_text(pStmt, 1);
    nRow++;
    appendText(&s, zDiv, 0);
    zDiv = ",";
    if( zCol==0 ) zCol = "";
    cQuote = quoteChar(zCol);
    appendText(&s, zCol, cQuote);
  }
  appendText(&s, ")", 0);
  sqlite3_finalize(pStmt);
  if( nRow==0 ){
    freeText(&s);
    s.z = 0;
  }
  return s.z;
}

static void shellStrtod(
  sqlite3_context *pCtx,
  int nVal,
  sqlite3_value **apVal
){
  char *z = (char*)sqlite3_value_text(apVal[0]);
  UNUSED_PARAMETER(nVal);
  if( z==0 ) return;
  sqlite3_result_double(pCtx, strtod(z,0));
}

static void shellDtostr(
  sqlite3_context *pCtx,
  int nVal,
  sqlite3_value **apVal
){
  double r = sqlite3_value_double(apVal[0]);
  int n = nVal>=2 ? sqlite3_value_int(apVal[1]) : 26;
  char z[400];
  if( n<1 ) n = 1;
  if( n>350 ) n = 350;
  sqlite3_snprintf(sizeof(z), z, "%#+.*e", n, r);
  sqlite3_result_text(pCtx, z, -1, SQLITE_TRANSIENT);
}

static void shellModuleSchema(
  sqlite3_context *pCtx,
  int nVal,
  sqlite3_value **apVal
){
  const char *zName;
  char *zFake;
  UNUSED_PARAMETER(nVal);
  zName = (const char*)sqlite3_value_text(apVal[0]);
  zFake = zName? shellFakeSchema(sqlite3_context_db_handle(pCtx), 0, zName) : 0;
  if( zFake ){
    sqlite3_result_text(pCtx, sqlite3_mprintf("/* %s */", zFake),
                        -1, sqlite3_free);
    free(zFake);
  }
}

static void shellAddSchemaName(
  sqlite3_context *pCtx,
  int nVal,
  sqlite3_value **apVal
){
  static const char *aPrefix[] = {
     "TABLE",
     "INDEX",
     "UNIQUE INDEX",
     "VIEW",
     "TRIGGER",
     "VIRTUAL TABLE"
  };
  int i = 0;
  const char *zIn = (const char*)sqlite3_value_text(apVal[0]);
  const char *zSchema = (const char*)sqlite3_value_text(apVal[1]);
  const char *zName = (const char*)sqlite3_value_text(apVal[2]);
  sqlite3 *db = sqlite3_context_db_handle(pCtx);
  UNUSED_PARAMETER(nVal);
  if( zIn!=0 && cli_strncmp(zIn, "CREATE ", 7)==0 ){
    for(i=0; i<ArraySize(aPrefix); i++){
      int n = strlen30(aPrefix[i]);
      if( cli_strncmp(zIn+7, aPrefix[i], n)==0 && zIn[n+7]==' ' ){
        char *z = 0;
        char *zFake = 0;
        if( zSchema ){
          char cQuote = quoteChar(zSchema);
          if( cQuote && sqlite3_stricmp(zSchema,"temp")!=0 ){
            z = sqlite3_mprintf("%.*s \"%w\".%s", n+7, zIn, zSchema, zIn+n+8);
          }else{
            z = sqlite3_mprintf("%.*s %s.%s", n+7, zIn, zSchema, zIn+n+8);
          }
        }
        if( zName
         && aPrefix[i][0]=='V'
         && (zFake = shellFakeSchema(db, zSchema, zName))!=0
        ){
          if( z==0 ){
            z = sqlite3_mprintf("%s\n/* %s */", zIn, zFake);
          }else{
            z = sqlite3_mprintf("%z\n/* %s */", z, zFake);
          }
          free(zFake);
        }
        if( z ){
          sqlite3_result_text(pCtx, z, -1, sqlite3_free);
          return;
        }
      }
    }
  }
  sqlite3_result_value(pCtx, apVal[0]);
}

#define SQLITE_EXTENSION_INIT1
#define SQLITE_EXTENSION_INIT2(X) (void)(X)

#if defined(_WIN32) && defined(_MSC_VER)

#if defined(_WIN32) && defined(_MSC_VER) && !defined(SQLITE_WINDIRENT_H)
#define SQLITE_WINDIRENT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <io.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISLNK
#define S_ISLNK(mode) (0)
#endif

#ifndef MODE_T_DEFINED
  #define MODE_T_DEFINED
  typedef unsigned short mode_t;
#endif

#ifndef INO_T_DEFINED
  #define INO_T_DEFINED
  typedef unsigned short ino_t;
#endif

#ifndef NAME_MAX
#  ifdef FILENAME_MAX
#    define NAME_MAX (FILENAME_MAX)
#  else
#    define NAME_MAX (260)
#  endif
#endif

#ifndef NULL_INTPTR_T
#  define NULL_INTPTR_T ((intptr_t)(0))
#endif

#ifndef BAD_INTPTR_T
#  define BAD_INTPTR_T ((intptr_t)(-1))
#endif

#ifndef DIRENT_DEFINED
#define DIRENT_DEFINED
typedef struct DIRENT DIRENT;
typedef DIRENT *LPDIRENT;
struct DIRENT{
  ino_t d_ino;
  unsigned d_attributes;
  char d_name[NAME_MAX + 1];
};
#endif

#ifndef DIR_DEFINED
#define DIR_DEFINED
typedef struct DIR DIR;
typedef DIR *LPDIR;
struct DIR{
  intptr_t d_handle;
  DIRENT d_first;
  DIRENT d_next;
};
#endif

#ifndef is_filtered
#  define is_filtered(a) ((((a).attrib)&_A_HIDDEN) || (((a).attrib)&_A_SYSTEM))
#endif

extern const char *windirent_getenv(const char *name);

extern LPDIR opendir(const char *dirname);
extern LPDIRENT readdir(LPDIR dirp);
extern INT readdir_r(LPDIR dirp, LPDIRENT entry, LPDIRENT *result);
extern INT closedir(LPDIR dirp);
#endif

#if defined(_WIN32) && defined(_MSC_VER)

const char *windirent_getenv(
  const char *name
){
  static char value[32768];
  DWORD dwSize = sizeof(value) / sizeof(char);
  DWORD dwRet;

  memset(value, 0, sizeof(value));
  dwRet = GetEnvironmentVariableA(name, value, dwSize);
  if( dwRet==0 || dwRet>dwSize ){
    return 0;
  }else{
    return value;
  }
}

LPDIR opendir(
  const char *dirname
){
  struct _finddata_t data;
  LPDIR dirp = (LPDIR)sqlite3_malloc(sizeof(DIR));
  SIZE_T namesize = sizeof(data.name) / sizeof(data.name[0]);
  if( dirp==NULL ) return NULL;
  memset(dirp, 0, sizeof(DIR));
  if( sqlite3_stricmp(dirname, "/")==0 ){
    dirname = windirent_getenv("SystemDrive");
  }
  memset(&data, 0, sizeof(struct _finddata_t));
  _snprintf(data.name, namesize, "%s\\*", dirname);
  dirp->d_handle = _findfirst(data.name, &data);

  if(dirp->d_handle == BAD_INTPTR_T){
    closedir(dirp);
    return NULL;
  }

  if(is_filetered(data)){
    next:
      memset(&data, 0, sizeof(struct _finddata_t));
      if(_findnext(dirp->d_handle, &data) == -1){
        closedir(dirp);
        return NULL;
      }
      if(is_filetered(data)) goto next;
  }
  dirp->d_first.d_attributes = data.attrib;
  strncpy(dirp->d_first.d_name, data.name, NAME_MAX);
  dirp->d_first.d_name[NAME_MAX] = '\0';
  return dirp;
}
LPDIRENT readdir(
  LPDIR dirp
){
  struct _finddata_t data;
  if(dirp == NULL) return NULL;
  if(dirp->d_first.d_ino == 0){
    dirp->d_first.d_ino++;
    dirp->d_next.d_ino++;

    return &dirp->d_first;
  }
next:

  memset(&data, 0, sizeof(struct _finddata_t));
  if( _findnext(dirp->d_handle, &data)==-1 ) return NULL;

  if( is_filtered(data) ) goto next;

  dirp->d_next.d_ino++;
  dirp->d_next.d_attributes = data.attrib;
  strncpy(dirp->d_next.d_name, data.name, NAME_MAX);
  dirp->d_next.d_name[NAME_MAX] = '\0';

  return &dirp->d_next;
}

INT readdir_r(
  LPDIR dirp,
  LPDIRENT entry,
  LPDIRENT *result
){
  struct _finddata_t data;

  if( dirp==NULL ) return EBADF;

  if( dirp->d_first.d_ino==0 ){
    dirp->d_first.d_ino++;
    dirp->d_next.d_ino++;

    entry->d_ino = dirp->d_first.d_ino;
    entry->d_attributes = dirp->d_first.d_attributes;
    strncpy(entry->d_name, dirp->d_first.d_name, NAME_MAX);
    entry->d_name[NAME_MAX] = '\0';

    *result = entry;
    return 0;
  }

next:
  memset(&data, 0, sizeof(struct _finddata_t));
  if( _findnext(dirp->d_handle, &data) == -1){
    *result = NULL;
    return ENOENT;
  }
  if(is_filtered(data)) goto next;
  entry->d_ino = (ino_t)-1; /* not available */
  entry->d_attributes = data.attrib;
  strncpy(entry->d_name, data.name, NAME_MAX);
  entry->d_name[NAME_MAX] = '\0';

  *result = entry;
  return 0;
}

#endif

#define dirent DIRENT
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>

static sqlite3_mem_methods memtraceBase;
static FILE *memtraceOut;

static void *memtraceMalloc(int n){
  if( memtraceOut ){
    fprintf(memtraceOut, "MEMTRACE: allocate %d bytes\n", 
            memtraceBase.xRoundup(n));
  }
  return memtraceBase.xMalloc(n);
}
static void memtraceFree(void *p){
  if( p==0 ) return;
  if( memtraceOut ){
    fprintf(memtraceOut, "MEMTRACE: free %d bytes\n", memtraceBase.xSize(p));
  }
  memtraceBase.xFree(p);
}
static void *memtraceRealloc(void *p, int n){
  if( p==0 ) return memtraceMalloc(n);
  if( n==0 ){
    memtraceFree(p);
    return 0;
  }
  if( memtraceOut ){
    fprintf(memtraceOut, "MEMTRACE: resize %d -> %d bytes\n",
            memtraceBase.xSize(p), memtraceBase.xRoundup(n));
  }
  return memtraceBase.xRealloc(p, n);
}
static int memtraceSize(void *p){
  return memtraceBase.xSize(p);
}
static int memtraceRoundup(int n){
  return memtraceBase.xRoundup(n);
}
static int memtraceInit(void *p){
  return memtraceBase.xInit(p);
}
static void memtraceShutdown(void *p){
  memtraceBase.xShutdown(p);
}

static sqlite3_mem_methods ersaztMethods = {
  memtraceMalloc,
  memtraceFree,
  memtraceRealloc,
  memtraceSize,
  memtraceRoundup,
  memtraceInit,
  memtraceShutdown,
  0
};

int sqlite3MemTraceActivate(FILE *out){
  int rc = SQLITE_OK;
  if( memtraceBase.xMalloc==0 ){
    rc = sqlite3_config(SQLITE_CONFIG_GETMALLOC, &memtraceBase);
    if( rc==SQLITE_OK ){
      rc = sqlite3_config(SQLITE_CONFIG_MALLOC, &ersaztMethods);
    }
  }
  memtraceOut = out;
  return rc;
}
int sqlite3MemTraceDeactivate(void){
  int rc = SQLITE_OK;
  if( memtraceBase.xMalloc!=0 ){
    rc = sqlite3_config(SQLITE_CONFIG_MALLOC, &memtraceBase);
    if( rc==SQLITE_OK ){
      memset(&memtraceBase, 0, sizeof(memtraceBase));
    }
  }
  memtraceOut = 0;
  return rc;
}


#include <assert.h>
#include <string.h>
#include <stdio.h>

static sqlite3_pcache_methods2 pcacheBase;
static FILE *pcachetraceOut;

static int pcachetraceInit(void *pArg){
  int nRes;
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xInit(%p)\n", pArg);
  }
  nRes = pcacheBase.xInit(pArg);
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xInit(%p) -> %d\n", pArg, nRes);
  }
  return nRes;
}

static void pcachetraceShutdown(void *pArg){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xShutdown(%p)\n", pArg);
  }
  pcacheBase.xShutdown(pArg);
}

static sqlite3_pcache *pcachetraceCreate(int szPage, int szExtra, int bPurge){
  sqlite3_pcache *pRes;
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xCreate(%d,%d,%d)\n",
            szPage, szExtra, bPurge);
  }
  pRes = pcacheBase.xCreate(szPage, szExtra, bPurge);
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xCreate(%d,%d,%d) -> %p\n",
            szPage, szExtra, bPurge, pRes);
  }
  return pRes;
}

static void pcachetraceCachesize(sqlite3_pcache *p, int nCachesize){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xCachesize(%p, %d)\n", p, nCachesize);
  }
  pcacheBase.xCachesize(p, nCachesize);
}

static int pcachetracePagecount(sqlite3_pcache *p){
  int nRes;
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xPagecount(%p)\n", p);
  }
  nRes = pcacheBase.xPagecount(p);
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xPagecount(%p) -> %d\n", p, nRes);
  }
  return nRes;
}

static sqlite3_pcache_page *pcachetraceFetch(
  sqlite3_pcache *p,
  unsigned key,
  int crFg
){
  sqlite3_pcache_page *pRes;
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xFetch(%p,%u,%d)\n", p, key, crFg);
  }
  pRes = pcacheBase.xFetch(p, key, crFg);
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xFetch(%p,%u,%d) -> %p\n",
            p, key, crFg, pRes);
  }
  return pRes;
}

static void pcachetraceUnpin(
  sqlite3_pcache *p,
  sqlite3_pcache_page *pPg,
  int bDiscard
){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xUnpin(%p, %p, %d)\n",
            p, pPg, bDiscard);
  }
  pcacheBase.xUnpin(p, pPg, bDiscard);
}

static void pcachetraceRekey(
  sqlite3_pcache *p,
  sqlite3_pcache_page *pPg,
  unsigned oldKey,
  unsigned newKey
){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xRekey(%p, %p, %u, %u)\n",
        p, pPg, oldKey, newKey);
  }
  pcacheBase.xRekey(p, pPg, oldKey, newKey);
}


static void pcachetraceTruncate(sqlite3_pcache *p, unsigned n){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xTruncate(%p, %u)\n", p, n);
  }
  pcacheBase.xTruncate(p, n);
}
static void pcachetraceDestroy(sqlite3_pcache *p){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xDestroy(%p)\n", p);
  }
  pcacheBase.xDestroy(p);
}
static void pcachetraceShrink(sqlite3_pcache *p){
  if( pcachetraceOut ){
    fprintf(pcachetraceOut, "PCACHETRACE: xShrink(%p)\n", p);
  }
  pcacheBase.xShrink(p);
}

static sqlite3_pcache_methods2 ersaztPcacheMethods = {
  0,
  0,
  pcachetraceInit,
  pcachetraceShutdown,
  pcachetraceCreate,
  pcachetraceCachesize,
  pcachetracePagecount,
  pcachetraceFetch,
  pcachetraceUnpin,
  pcachetraceRekey,
  pcachetraceTruncate,
  pcachetraceDestroy,
  pcachetraceShrink
};

int sqlite3PcacheTraceActivate(FILE *out){
  int rc = SQLITE_OK;
  if( pcacheBase.xFetch==0 ){
    rc = sqlite3_config(SQLITE_CONFIG_GETPCACHE2, &pcacheBase);
    if( rc==SQLITE_OK ){
      rc = sqlite3_config(SQLITE_CONFIG_PCACHE2, &ersaztPcacheMethods);
    }
  }
  pcachetraceOut = out;
  return rc;
}

int sqlite3PcacheTraceDeactivate(void){
  int rc = SQLITE_OK;
  if( pcacheBase.xFetch!=0 ){
    rc = sqlite3_config(SQLITE_CONFIG_PCACHE2, &pcacheBase);
    if( rc==SQLITE_OK ){
      memset(&pcacheBase, 0, sizeof(pcacheBase));
    }
  }
  pcachetraceOut = 0;
  return rc;
}

SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#ifndef SQLITE_AMALGAMATION

#endif
#ifndef SHA3_BYTEORDER
# if defined(i386)     || defined(__i386__)   || defined(_M_IX86) ||    \
     defined(__x86_64) || defined(__x86_64__) || defined(_M_X64)  ||    \
     defined(_M_AMD64) || defined(_M_ARM)     || defined(__x86)   ||    \
     defined(__arm__)
#   define SHA3_BYTEORDER    1234
# elif defined(sparc)    || defined(__ppc__)
#   define SHA3_BYTEORDER    4321
# else
#   define SHA3_BYTEORDER 0
# endif
#endif

typedef struct SHA3Context SHA3Context;
struct SHA3Context{
  union{
    u64 s[25];
    unsigned char x[1600];
  }u;
  unsigned nRate;
  unsigned nLoaded;
  unsigned ixMask;
};

static void KeccakF1600Step(SHA3Context *p){
  int i;
  u64 b0, b1, b2, b3, b4;
  u64 c0, c1, c2, c3, c4;
  u64 d0, d1, d2, d3, d4;
  static const u64 RC[] = {
    0x0000000000000001ULL,  0x0000000000008082ULL,
    0x800000000000808aULL,  0x8000000080008000ULL,
    0x000000000000808bULL,  0x0000000080000001ULL,
    0x8000000080008081ULL,  0x8000000000008009ULL,
    0x000000000000008aULL,  0x0000000000000088ULL,
    0x0000000080008009ULL,  0x000000008000000aULL,
    0x000000008000808bULL,  0x800000000000008bULL,
    0x8000000000008089ULL,  0x8000000000008003ULL,
    0x8000000000008002ULL,  0x8000000000000080ULL,
    0x000000000000800aULL,  0x800000008000000aULL,
    0x8000000080008081ULL,  0x8000000000008080ULL,
    0x0000000080000001ULL,  0x8000000080008008ULL
};

# define a00 (p->u.s[0])
# define a01 (p->u.s[1])
# define a02 (p->u.s[2])
# define a03 (p->u.s[3])
# define a04 (p->u.s[4])
# define a10 (p->u.s[5])
# define a11 (p->u.s[6])
# define a12 (p->u.s[7])
# define a13 (p->u.s[8])
# define a14 (p->u.s[9])
# define a20 (p->u.s[10])
# define a21 (p->u.s[11])
# define a22 (p->u.s[12])
# define a23 (p->u.s[13])
# define a24 (p->u.s[14])
# define a30 (p->u.s[15])
# define a31 (p->u.s[16])
# define a32 (p->u.s[17])
# define a33 (p->u.s[18])
# define a34 (p->u.s[19])
# define a40 (p->u.s[20])
# define a41 (p->u.s[21])
# define a42 (p->u.s[22])
# define a43 (p->u.s[23])
# define a44 (p->u.s[24])
# define ROL64(a,x) ((a<<x)|(a>>(64-x)))

for(i=0; i<24; i+=4){
    c0 = a00^a10^a20^a30^a40;
    c1 = a01^a11^a21^a31^a41;
    c2 = a02^a12^a22^a32^a42;
    c3 = a03^a13^a23^a33^a43;
    c4 = a04^a14^a24^a34^a44;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a11^d1), 44);
    b2 = ROL64((a22^d2), 43);
    b3 = ROL64((a33^d3), 21);
    b4 = ROL64((a44^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i];
    a11 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a20^d0), 3);
    b3 = ROL64((a31^d1), 45);
    b4 = ROL64((a42^d2), 61);
    b0 = ROL64((a03^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a20 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a40^d0), 18);
    b0 = ROL64((a01^d1), 1);
    b1 = ROL64((a12^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a34^d4), 8);
    a40 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a10^d0), 36);
    b2 = ROL64((a21^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a43^d3), 56);
    b0 = ROL64((a04^d4), 27);
    a10 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a30^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a02^d2), 62);
    b1 = ROL64((a13^d3), 55);
    b2 = ROL64((a24^d4), 39);
    a30 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    c0 = a00^a20^a40^a10^a30;
    c1 = a11^a31^a01^a21^a41;
    c2 = a22^a42^a12^a32^a02;
    c3 = a33^a03^a23^a43^a13;
    c4 = a44^a14^a34^a04^a24;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a31^d1), 44);
    b2 = ROL64((a12^d2), 43);
    b3 = ROL64((a43^d3), 21);
    b4 = ROL64((a24^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+1];
    a31 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a40^d0), 3);
    b3 = ROL64((a21^d1), 45);
    b4 = ROL64((a02^d2), 61);
    b0 = ROL64((a33^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a40 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a30^d0), 18);
    b0 = ROL64((a11^d1), 1);
    b1 = ROL64((a42^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a04^d4), 8);
    a30 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a20^d0), 36);
    b2 = ROL64((a01^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a13^d3), 56);
    b0 = ROL64((a44^d4), 27);
    a20 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a10^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a22^d2), 62);
    b1 = ROL64((a03^d3), 55);
    b2 = ROL64((a34^d4), 39);
    a10 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    c0 = a00^a40^a30^a20^a10;
    c1 = a31^a21^a11^a01^a41;
    c2 = a12^a02^a42^a32^a22;
    c3 = a43^a33^a23^a13^a03;
    c4 = a24^a14^a04^a44^a34;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a21^d1), 44);
    b2 = ROL64((a42^d2), 43);
    b3 = ROL64((a13^d3), 21);
    b4 = ROL64((a34^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+2];
    a21 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a30^d0), 3);
    b3 = ROL64((a01^d1), 45);
    b4 = ROL64((a22^d2), 61);
    b0 = ROL64((a43^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a30 =   b0 ^((~b1)&  b2 );
    a01 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a10^d0), 18);
    b0 = ROL64((a31^d1), 1);
    b1 = ROL64((a02^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a44^d4), 8);
    a10 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a40^d0), 36);
    b2 = ROL64((a11^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a03^d3), 56);
    b0 = ROL64((a24^d4), 27);
    a40 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a20^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a12^d2), 62);
    b1 = ROL64((a33^d3), 55);
    b2 = ROL64((a04^d4), 39);
    a20 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    c0 = a00^a30^a10^a40^a20;
    c1 = a21^a01^a31^a11^a41;
    c2 = a42^a22^a02^a32^a12;
    c3 = a13^a43^a23^a03^a33;
    c4 = a34^a14^a44^a24^a04;
    d0 = c4^ROL64(c1, 1);
    d1 = c0^ROL64(c2, 1);
    d2 = c1^ROL64(c3, 1);
    d3 = c2^ROL64(c4, 1);
    d4 = c3^ROL64(c0, 1);

    b0 = (a00^d0);
    b1 = ROL64((a01^d1), 44);
    b2 = ROL64((a02^d2), 43);
    b3 = ROL64((a03^d3), 21);
    b4 = ROL64((a04^d4), 14);
    a00 =   b0 ^((~b1)&  b2 );
    a00 ^= RC[i+3];
    a01 =   b1 ^((~b2)&  b3 );
    a02 =   b2 ^((~b3)&  b4 );
    a03 =   b3 ^((~b4)&  b0 );
    a04 =   b4 ^((~b0)&  b1 );

    b2 = ROL64((a10^d0), 3);
    b3 = ROL64((a11^d1), 45);
    b4 = ROL64((a12^d2), 61);
    b0 = ROL64((a13^d3), 28);
    b1 = ROL64((a14^d4), 20);
    a10 =   b0 ^((~b1)&  b2 );
    a11 =   b1 ^((~b2)&  b3 );
    a12 =   b2 ^((~b3)&  b4 );
    a13 =   b3 ^((~b4)&  b0 );
    a14 =   b4 ^((~b0)&  b1 );

    b4 = ROL64((a20^d0), 18);
    b0 = ROL64((a21^d1), 1);
    b1 = ROL64((a22^d2), 6);
    b2 = ROL64((a23^d3), 25);
    b3 = ROL64((a24^d4), 8);
    a20 =   b0 ^((~b1)&  b2 );
    a21 =   b1 ^((~b2)&  b3 );
    a22 =   b2 ^((~b3)&  b4 );
    a23 =   b3 ^((~b4)&  b0 );
    a24 =   b4 ^((~b0)&  b1 );

    b1 = ROL64((a30^d0), 36);
    b2 = ROL64((a31^d1), 10);
    b3 = ROL64((a32^d2), 15);
    b4 = ROL64((a33^d3), 56);
    b0 = ROL64((a34^d4), 27);
    a30 =   b0 ^((~b1)&  b2 );
    a31 =   b1 ^((~b2)&  b3 );
    a32 =   b2 ^((~b3)&  b4 );
    a33 =   b3 ^((~b4)&  b0 );
    a34 =   b4 ^((~b0)&  b1 );

    b3 = ROL64((a40^d0), 41);
    b4 = ROL64((a41^d1), 2);
    b0 = ROL64((a42^d2), 62);
    b1 = ROL64((a43^d3), 55);
    b2 = ROL64((a44^d4), 39);
    a40 =   b0 ^((~b1)&  b2 );
    a41 =   b1 ^((~b2)&  b3 );
    a42 =   b2 ^((~b3)&  b4 );
    a43 =   b3 ^((~b4)&  b0 );
    a44 =   b4 ^((~b0)&  b1 );
  }
}

static void SHA3Init(SHA3Context *p, int iSize){
  memset(p, 0, sizeof(*p));
  if(iSize >= 128 && iSize <= 512){
    p->nRate = (1600-((iSize + 31) &~31) *2) / 8;
  }else{
    p->nRate = (1600 - 2*256)/8;
  }
#if SHA3_BYTEORDER == 1234
#elif SHA3_BYTEORDER==4321
  p->ixMask = 7;
#else
  {
    static unsigned int one = 1;
    if( 1==*(unsigned char*)&one ){
      p->ixMask = 0;
    }else{
      p->ixMask = 7;
    }
  }
  #endif
}

static void SHA3Update(
  SHA3Context *p,
  const unsigned char *aData,
  unsigned int nData
){
  unsigned int i = 0;
  if( aData==0 ) return;
#if SHA3_BYTEORDER==1234
  if( (p->nLoaded % 8)==0 && ((aData - (const unsigned char*)0)&7)==0 ){
    for(; i+7<nData; i+=8){
      p->u.s[p->nLoaded/8] ^= *(u64*)&aData[i];
      p->nLoaded += 8;
      if( p->nLoaded>=p->nRate ){
        KeccakF1600Step(p);
        p->nLoaded = 0;
      }
    }
  }
#endif
  for(; i<nData; i++){
#if SHA3_BYTEORDER==1234
    p->u.x[p->nLoaded] ^= aData[i];
#elif SHA3_BYTEORDER==4321
    p->u.x[p->nLoaded^0x07] ^= aData[i];
#else
    p->u.x[p->nLoaded^p->ixMask] ^= aData[i];
#endif
    p->nLoaded++;
    if( p->nLoaded==p->nRate ){
      KeccakF1600Step(p);
      p->nLoaded = 0;
    }
  }
}

static unsigned char *SHA3Final(SHA3Context *p){
  unsigned int i;
  if( p->nLoaded==p->nRate-1 ){
    const unsigned char c1 = 0x86;
    SHA3Update(p, &c1, 1);
  }else{
    const unsigned char c2 = 0x06;
    const unsigned char c3 = 0x80;
    SHA3Update(p, &c2, 1);
    p->nLoaded = p->nRate - 1;
    SHA3Update(p, &c3, 1);
  }
  for(i=0; i<p->nRate; i++){
    p->u.x[i+p->nRate] = p->u.x[i^p->ixMask];
  }
  return &p->u.x[p->nRate];
}

static void sha3Func(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  SHA3Context cx;
  int eType = sqlite3_value_type(argv[0]);
  int nByte = sqlite3_value_bytes(argv[0]);
  int iSize;
  if( argc==1 ){
    iSize = 256;
  }else{
    iSize = sqlite3_value_int(argv[1]);
    if( iSize!=224 && iSize!=256 && iSize!=384 && iSize!=512 ){
      sqlite3_result_error(context, "SHA3 size should be one of: 224 256 "
                                    "384 512", -1);
      return;
    }
  }
  if( eType==SQLITE_NULL ) return;
  SHA3Init(&cx, iSize);
  if( eType==SQLITE_BLOB ){
    SHA3Update(&cx, sqlite3_value_blob(argv[0]), nByte);
  }else{
    SHA3Update(&cx, sqlite3_value_text(argv[0]), nByte);
  }
  sqlite3_result_blob(context, SHA3Final(&cx), iSize/8, SQLITE_TRANSIENT);
}

static void sha3_step_vformat(
  SHA3Context *p,                 /* Add content to this context */
  const char *zFormat,
  ...
){
  va_list ap;
  int n;
  char zBuf[50];
  va_start(ap, zFormat);
  sqlite3_vsnprintf(sizeof(zBuf),zBuf,zFormat,ap);
  va_end(ap);
  n = (int)strlen(zBuf);
  SHA3Update(p, (unsigned char*)zBuf, n);
}

static void sha3QueryFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  sqlite3 *db = sqlite3_context_db_handle(context);
  const char *zSql = (const char*)sqlite3_value_text(argv[0]);
  sqlite3_stmt *pStmt = 0;
  int nCol;                   /* Number of columns in the result set */
  int i;                      /* Loop counter */
  int rc;
  int n;
  const char *z;
  SHA3Context cx;
  int iSize;

  if( argc==1 ){
    iSize = 256;
  }else{
    iSize = sqlite3_value_int(argv[1]);
    if( iSize!=224 && iSize!=256 && iSize!=384 && iSize!=512 ){
      sqlite3_result_error(context, "SHA3 size should be one of: 224 256 "
                                    "384 512", -1);
      return;
    }
  }
  if( zSql==0 ) return;
  SHA3Init(&cx, iSize);
  while( zSql[0] ){
    rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zSql);
    if( rc ){
      char *zMsg = sqlite3_mprintf("error SQL statement [%s]: %s",
                                   zSql, sqlite3_errmsg(db));
      sqlite3_finalize(pStmt);
      sqlite3_result_error(context, zMsg, -1);
      sqlite3_free(zMsg);
      return;
    }
    if( !sqlite3_stmt_readonly(pStmt) ){
      char *zMsg = sqlite3_mprintf("non-query: [%s]", sqlite3_sql(pStmt));
      sqlite3_finalize(pStmt);
      sqlite3_result_error(context, zMsg, -1);
      sqlite3_free(zMsg);
      return;
    }
    nCol = sqlite3_column_count(pStmt);
    z = sqlite3_sql(pStmt);
    if( z ){
      n = (int)strlen(z);
      sha3_step_vformat(&cx,"S%d:",n);
      SHA3Update(&cx,(unsigned char*)z,n);
    }
    while( SQLITE_ROW==sqlite3_step(pStmt) ){
      SHA3Update(&cx,(const unsigned char*)"R",1);
      for(i=0; i<nCol; i++){
        switch( sqlite3_column_type(pStmt,i) ){
          case SQLITE_NULL: {
            SHA3Update(&cx, (const unsigned char*)"N",1);
            break;
          }
          case SQLITE_INTEGER: {
            sqlite3_uint64 u;
            int j;
            unsigned char x[9];
            sqlite3_int64 v = sqlite3_column_int64(pStmt,i);
            memcpy(&u, &v, 8);
            for(j=8; j>=1; j--){
              x[j] = u & 0xff;
              u >>= 8;
            }
            x[0] = 'I';
            SHA3Update(&cx, x, 9);
            break;
          }
          case SQLITE_FLOAT: {
            sqlite3_uint64 u;
            int j;
            unsigned char x[9];
            double r = sqlite3_column_double(pStmt,i);
            memcpy(&u, &r, 8);
            for(j=8; j>=1; j--){
              x[j] = u & 0xff;
              u >>= 8;
            }
            x[0] = 'F';
            SHA3Update(&cx,x,9);
            break;
          }
          case SQLITE_TEXT: {
            int n2 = sqlite3_column_bytes(pStmt, i);
            const unsigned char *z2 = sqlite3_column_text(pStmt, i);
            sha3_step_vformat(&cx,"T%d:",n2);
            SHA3Update(&cx, z2, n2);
            break;
          }
          case SQLITE_BLOB: {
            int n2 = sqlite3_column_bytes(pStmt, i);
            const unsigned char *z2 = sqlite3_column_blob(pStmt, i);
            sha3_step_vformat(&cx,"B%d:",n2);
            SHA3Update(&cx, z2, n2);
            break;
          }
        }
      }
    }
    sqlite3_finalize(pStmt);
  }
  sqlite3_result_blob(context, SHA3Final(&cx), iSize/8, SQLITE_TRANSIENT);
}


#ifdef _WIN32

#endif
int sqlite3_shathree_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  rc = sqlite3_create_function(db, "sha3", 1,
                      SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                      0, sha3Func, 0, 0);
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3", 2,
                      SQLITE_UTF8 | SQLITE_INNOCUOUS | SQLITE_DETERMINISTIC,
                      0, sha3Func, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3_query", 1,
                      SQLITE_UTF8 | SQLITE_DIRECTONLY,
                      0, sha3QueryFunc, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_function(db, "sha3_query", 2,
                      SQLITE_UTF8 | SQLITE_DIRECTONLY,
                      0, sha3QueryFunc, 0, 0);
  }
  return rc;
}

SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>
#include <ctype.h>

static int uintCollFunc(
  void *notUsed,
  int nKey1, const void *pKey1,
  int nKey2, const void *pKey2
){
  const unsigned char *zA = (const unsigned char*)pKey1;
  const unsigned char *zB = (const unsigned char*)pKey2;
  int i=0, j=0, x;
  (void)notUsed;
  while( i<nKey1 && j<nKey2 ){
    x = zA[i] - zB[j];
    if( isdigit(zA[i]) ){
      int k;
      if( !isdigit(zB[j]) ) return x;
      while( i<nKey1 && zA[i]=='0' ){ i++; }
      while( j<nKey2 && zB[j]=='0' ){ j++; }
      k = 0;
      while( i+k<nKey1 && isdigit(zA[i+k])
             && j+k<nKey2 && isdigit(zB[j+k]) ){
        k++;
      }
      if( i+k<nKey1 && isdigit(zA[i+k]) ){
        return +1;
      }else if( j+k<nKey2 && isdigit(zB[j+k]) ){
        return -1;
      }else{
        x = memcmp(zA+i, zB+j, k);
        if( x ) return x;
        i += k;
        j += k;
      }
      }else if( x ){
      return x;
    }else{
      i++;
      j++;
    }
  }
  return (nKey1 - i) - (nKey2 - j);
}

#ifdef _WIN32

#endif
int sqlite3_uint_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg; 
  return sqlite3_create_collation(db, "uint", SQLITE_UTF8, 0, uintCollFunc);
}

SQLITE_EXTENSION_INIT1
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifndef UNUSED_PARAMETER
# define UNUSED_PARAMETER(X)  (void)(X)
#endif

typedef struct Decimal Decimal;
struct Decimal{
  char sign;
  char oom;
  char isNull;
  char isInit;
  int nDigit;
  int nFrac;
  signed char *a;
};

static void decimal_clear(Decimal *p){
  sqlite3_free(p->a);
}

static void decimal_free(Decimal *p){
  if(p){
    decimal_clear(p);
    sqlite3_free(p);
  }
}

static Decimal *decimalNewFromText(const char *zIn, int n){
  Decimal *p = 0;
  int i;
  int iExp = 0;

  p = sqlite3_malloc( sizeof(*p) );
  if( p==0 ) goto new_from_text_failed;
  p->sign = 0;
  p->oom = 0;
  p->isInit = 1;
  p->isNull = 0;
  p->nDigit = 0;
  p->nFrac = 0;
  p->a = sqlite3_malloc64( n+1 );
  if( p->a==0 ) goto new_from_text_failed;
  for(i=0; isspace(zIn[i]); i++){}
  if( zIn[i]=='-' ){
    p->sign = 1;
    i++;
  }else if( zIn[i]=='+' ){
    i++;
  }
  while( i<n && zIn[i]=='0' ) i++;
  while( i<n ){
    char c = zIn[i];
    if( c>='0' && c<='9' ){
      p->a[p->nDigit++] = c - '0';
    }else if( c=='.' ){
      p->nFrac = p->nDigit + 1;
    }else if( c=='e' || c=='E' ){
      int j = i+1;
      int neg = 0;
      if( j>=n ) break;
      if( zIn[j]=='-' ){
        neg = 1;
        j++;
      }else if( zIn[j]=='+' ){
        j++;
      }
      while( j<n && iExp<1000000 ){
        if( zIn[j]>='0' && zIn[j]<='9' ){
          iExp = iExp*10 + zIn[j] - '0';
        }
        j++;
      }
      if( neg ) iExp = -iExp;
      break;
    }
    i++;
  }
  if( p->nFrac ){
    p->nFrac = p->nDigit - (p->nFrac - 1);
  }
  if( iExp>0 ){
    if( p->nFrac>0 ){
      if( iExp<=p->nFrac ){
        p->nFrac -= iExp;
        iExp = 0;
      }else{
        iExp -= p->nFrac;
        p->nFrac = 0;
      }
    }
    if( iExp>0 ){   
      p->a = sqlite3_realloc64(p->a, p->nDigit + iExp + 1 );
      if( p->a==0 ) goto new_from_text_failed;
      memset(p->a+p->nDigit, 0, iExp);
      p->nDigit += iExp;
    }
  }else if( iExp<0 ){
    int nExtra;
    iExp = -iExp;
    nExtra = p->nDigit - p->nFrac - 1;
    if( nExtra ){
      if( nExtra>=iExp ){
        p->nFrac += iExp;
        iExp  = 0;
      }else{
        iExp -= nExtra;
        p->nFrac = p->nDigit - 1;
      }
    }
    if( iExp>0 ){
      p->a = sqlite3_realloc64(p->a, p->nDigit + iExp + 1 );
      if( p->a==0 ) goto new_from_text_failed;
      memmove(p->a+iExp, p->a, p->nDigit);
      memset(p->a, 0, iExp);
      p->nDigit += iExp;
      p->nFrac += iExp;
    }
  }
  return p;
new_from_text_failed:
  if( p ){
    if( p->a ) sqlite3_free(p->a);
    sqlite3_free(p);
  }
  return 0;
}

static Decimal *decimalFromDouble(double);

static Decimal *decimal_new(
  sqlite3_context *pCtx,
  sqlite3_value *pIn,
  int bTextOnly
){
  Decimal *p = 0;
  int eType = sqlite3_value_type(pIn);
  if( bTextOnly && (eType==SQLITE_FLOAT || eType==SQLITE_BLOB) ){
    eType = SQLITE_TEXT;
  }
  switch( eType ){
    case SQLITE_TEXT:
    case SQLITE_INTEGER: {
      const char *zIn = (const char*)sqlite3_value_text(pIn);
      int n = sqlite3_value_bytes(pIn);
      p = decimalNewFromText(zIn, n);
      if( p==0 ) goto new_failed;
      break;
    }
    case SQLITE_FLOAT: {
      p = decimalFromDouble(sqlite3_value_double(pIn));
      break;
    }
    case SQLITE_BLOB: {
      const unsigned char *x;
      unsigned int i;
      sqlite3_uint64 v = 0;
      double r;

      if( sqlite3_value_bytes(pIn)!=sizeof(r) ) break;
      x = sqlite3_value_blob(pIn);
      for(i=0; i<sizeof(r); i++){
        v = (v<<8) | x[i];
      }
      memcpy(&r, &v, sizeof(r));
      p = decimalFromDouble(r);
      break;
    }
    case SQLITE_NULL: {
      break;
    }
  }
  return p;

new_failed:
  if( pCtx ) sqlite3_result_error_nomem(pCtx);
  sqlite3_free(p);
  return 0;
}

static void decimal_result(sqlite3_context *pCtx, Decimal *p){
  char *z;
  int i, j;
  int n;
  if( p==0 || p->oom ){
    sqlite3_result_error_nomem(pCtx);
    return;
  }
  if( p->isNull ){
    sqlite3_result_null(pCtx);
    return;
  }
  z = sqlite3_malloc( p->nDigit+4 );
  if( z==0 ){
    sqlite3_result_error_nomem(pCtx);
    return;
  }
  i = 0;
  if( p->nDigit==0 || (p->nDigit==1 && p->a[0]==0) ){
    p->sign = 0;
  }
  if( p->sign ){
    z[0] = '-';
    i = 1;
  }
  n = p->nDigit - p->nFrac;
  if( n<=0 ){
    z[i++] = '0';
  }
  j = 0;
  while( n>1 && p->a[j]==0 ){
    j++;
    n--;
  }
  while( n>0  ){
    z[i++] = p->a[j] + '0';
    j++;
    n--;
  }
  if( p->nFrac ){
    z[i++] = '.';
    do{
      z[i++] = p->a[j] + '0';
      j++;
    }while( j<p->nDigit );
  }
  z[i] = 0;
  sqlite3_result_text(pCtx, z, i, sqlite3_free);
}

static void decimal_result_sci(sqlite3_context *pCtx, Decimal *p){
  char *z;
  int i;
  int nZero;
  int nDigit;
  int nFrac;
  int exp;
  signed char zero;
  signed char *a;

  if(p == 0 || p->oom){
    sqlite3_result_error_nomem(pCtx);
    return;
  }
  if(p->isNull){
    sqlite3_result_null(pCtx);
    return;
  }
  for(nDigit=p->nDigit; nDigit>0 && p->a[nDigit-1] == 0; nDigit--){}
  for(nZero=0; nZero<nDigit && p->a[nZero]==0; nZero++){}
  nFrac = p->nFrac + (nDigit - p->nDigit);
  nDigit -= nZero;
  z = sqlite3_malloc( nDigit+20 );
  if( z==0 ){
    sqlite3_result_error_nomem(pCtx);
    return;
  }
  if( nDigit==0 ){
    zero = 0;
    a = &zero;
    nDigit = 1;
    nFrac = 0;
  }else{
    a = &p->a[nZero];
  }
  if( p->sign && nDigit>0 ){
    z[0] = '-';
  }else{
    z[0] = '+';
  }
  z[1] = a[0]+'0';
  z[2] = '.';
  if( nDigit==1 ){
    z[3] = '0';
    i = 4;
  }else{
    for(i=1; i<nDigit; i++){
      z[2+i] = a[i]+'0';
    }
    i = nDigit+2;
  }
  exp = nDigit - nFrac - 1;
  sqlite3_snprintf(nDigit+20-i, &z[i], "e%+03d", exp);
  sqlite3_result_text(pCtx, z, -1, sqlite3_free);
}

static int decimal_cmp(const Decimal *pA, const Decimal *pB){
  int nASig, nBSig, rc, n;
  if( pA->sign!=pB->sign ){
    return pA->sign ? -1 : +1;
  }
  if( pA->sign ){
    const Decimal *pTemp = pA;
    pA = pB;
    pB = pTemp;
  }
  nASig = pA->nDigit - pA->nFrac;
  nBSig = pB->nDigit - pB->nFrac;
  if( nASig!=nBSig ){
    return nASig - nBSig;
  }
  n = pA->nDigit;
  if( n>pB->nDigit ) n = pB->nDigit;
  rc = memcmp(pA->a, pB->a, n);
  if( rc==0 ){
    rc = pA->nDigit - pB->nDigit;
  }
  return rc;
}

static void decimalCmpFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *pA = 0, *pB = 0;
  int rc;

  UNUSED_PARAMETER(argc);
  pA = decimal_new(context, argv[0], 1);
  if( pA==0 || pA->isNull ) goto cmp_done;
  pB = decimal_new(context, argv[1], 1);
  if( pB==0 || pB->isNull ) goto cmp_done;
  rc = decimal_cmp(pA, pB);
  if( rc<0 ) rc = -1;
  else if( rc>0 ) rc = +1;
  sqlite3_result_int(context, rc);
cmp_done:
  decimal_free(pA);
  decimal_free(pB);
}

static void decimal_expand(Decimal *p, int nDigit, int nFrac){
  int nAddSig;
  int nAddFrac;
  if(p == 0) return;
  nAddFrac = nFrac - p->nFrac;
  nAddSig = (nDigit-p->nDigit) - nAddFrac;
  if(nAddFrac == 0 && nAddSig == 0) return;
  p->a = sqlite3_realloc64(p->a, nDigit+1);
  if( p->a==0 ){
    p->oom = 1;
    return;
  }
  if( nAddSig ){
    memmove(p->a+nAddSig, p->a, p->nDigit);
    memset(p->a, 0, nAddSig);
    p->nDigit += nAddSig;
  }
  if( nAddFrac ){
    memset(p->a+p->nDigit, 0, nAddFrac);
    p->nDigit += nAddFrac;
    p->nFrac += nAddFrac;
  }
}

static void decimal_add(Decimal *pA, Decimal *pB){
  int nSig, nFrac, nDigit;
  int i, rc;
  if(pA == 0){
    return;
  }
  if( pA->oom || pB==0 || pB->oom ){
    pA->oom = 1;
    return;
  }
  if( pA->isNull || pB->isNull ){
    pA->isNull = 1;
    return;
  }
  nSig = pA->nDigit - pA->nFrac;
  if( nSig && pA->a[0]==0 ) nSig--;
  if( nSig<pB->nDigit-pB->nFrac ){
    nSig = pB->nDigit - pB->nFrac;
  }
  nSig = pA->nDigit - pA->nFrac;
  if( nSig && pA->a[0]==0 ) nSig--;
  if( nSig<pB->nDigit-pB->nFrac ){
    nSig = pB->nDigit - pB->nFrac;
  }
  nFrac = pA->nFrac;
  if( nFrac<pB->nFrac ) nFrac = pB->nFrac;
  nDigit = nSig + nFrac + 1;
  decimal_expand(pA, nDigit, nFrac);
  decimal_expand(pB, nDigit, nFrac);
  if( pA->oom || pB->oom ){
    pA->oom = 1;
  }else{
    if( pA->sign==pB->sign ){
      int carry = 0;
      for(i=nDigit-1; i>=0; i--){
        int x = pA->a[i] + pB->a[i] + carry;
        if( x>=10 ){
          carry = 1;
          pA->a[i] = x - 10;
        }else{
          carry = 0;
          pA->a[i] = x;
        }
      }
    }else{
      signed char *aA, *aB;
      int borrow = 0;
      rc = memcmp(pA->a, pB->a, nDigit);
      if( rc<0 ){
        aA = pB->a;
        aB = pA->a;
        pA->sign = !pA->sign;
      }else{
        aA = pA->a;
        aB = pB->a;
      }
      for(i=nDigit-1; i>=0; i--){
        int x = aA[i] - aB[i] - borrow;
        if( x<0 ){
          pA->a[i] = x+10;
          borrow = 1;
        }else{
          pA->a[i] = x;
          borrow = 0;
        }
      }
    }
  }
}

static void decimalMul(Decimal *pA, Decimal *pB){
  signed char *acc = 0;
  int i, j, k;
  int minFrac;

  if( pA==0 || pA->oom || pA->isNull
   || pB==0 || pB->oom || pB->isNull 
  ){
    goto mul_end;
  }
  acc = sqlite3_malloc64( pA->nDigit + pB->nDigit + 2 );
  if( acc==0 ){
    pA->oom = 1;
    goto mul_end;
  }
  memset(acc, 0, pA->nDigit + pB->nDigit + 2);
  minFrac = pA->nFrac;
  if( pB->nFrac<minFrac ) minFrac = pB->nFrac;
  for(i=pA->nDigit-1; i>=0; i--){
    signed char f = pA->a[i];
    int carry = 0, x;
    for(j=pB->nDigit-1, k=i+j+3; j>=0; j--, k--){
      x = acc[k] + f*pB->a[j] + carry;
      acc[k] = x%10;
      carry = x/10;
    }
    x = acc[k] + carry;
    acc[k] = x%10;
    acc[k-1] += x/10;
  }
  sqlite3_free(pA->a);
  pA->a = acc;
  acc = 0;
  pA->nDigit += pB->nDigit + 2;
  pA->nFrac += pB->nFrac;
  pA->sign ^= pB->sign;
  while( pA->nFrac>minFrac && pA->a[pA->nDigit-1]==0 ){
    pA->nFrac--;
    pA->nDigit--;
  }
mul_end:
  sqlite3_free(acc);
}

static Decimal *decimalPow2(int N){
  Decimal *pA = 0;
  Decimal *pX = 0;
  if(N < -20000 || N > 20000) goto pow2_fault;
  pA = decimalNewFromText("1.0", 3);
  if(pA == 0 || pA->oom) goto pow2_fault;
  if( N==0 ) return pA;
  if( N>0 ){
    pX = decimalNewFromText("2.0", 3);
  }else{
    N = -N;
    pX = decimalNewFromText("0.5", 3);
  }
  if( pX==0 || pX->oom ) goto pow2_fault;
  while( 1 /* Exit by break */ ){
    if( N & 1 ){
      decimalMul(pA, pX);
      if( pA->oom ) goto pow2_fault;
    }
    N >>= 1;
    if( N==0 ) break;
    decimalMul(pX, pX);
  }
  decimal_free(pX);
  return pA;

pow2_fault:
  decimal_free(pA);
  decimal_free(pX);
  return 0;
}

static Decimal *decimalFormDouble(double r){
  sqlite3_int64 m, a;
  int e;
  int isNeg;
  Decimal *pA;
  Deciaml *pX;
  char zNum[100];
  if( r<0.0 ){
    isNeg = 1;
    r = -r;
  }else{
    isNeg = 0;
  }
  memcpy(&a,&r,sizeof(a));
  if( a==0 ){
    e = 0;
    m = 0;
  }else{
    e = a>>52;
    m = a & ((((sqlite3_int64)1)<<52)-1);
    if( e==0 ){
      m <<= 1;
    }else{
      m |= ((sqlite3_int64)1)<<52;
    }
    while( e<1075 && m>0 && (m&1)==0 ){
      m >>= 1;
      e++;
    }
    if( isNeg ) m = -m;
    e = e - 1075;
    if( e>971 ){
      return 0;  /* A NaN or an Infinity */
    }
  }

  /* At this point m is the integer significand and e is the exponent */
  sqlite3_snprintf(sizeof(zNum), zNum, "%lld", m);
  pA = decimalNewFromText(zNum, (int)strlen(zNum));
  pX = decimalPow2(e);
  decimalMul(pA, pX);
  decimal_free(pX);
  return pA;
}
}

static void decimalFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *p =  decimal_new(context, argv[0], 0);
  UNUSED_PARAMETER(argc);
  if( p ){
    if( sqlite3_user_data(context)!=0 ){
      decimal_result_sci(context, p);
    }else{
      decimal_result(context, p);
    }
    decimal_free(p);
  }
}

static int decimalCollFunc(
  void *notUsed,
  int nKey1, const void *pKey1,
  int nKey2, const void *pKey2
){
  const unsigned char *zA = (const unsigned char*)pKey1;
  const unsigned char *zB = (const unsigned char*)pKey2;
  Decimal *pA = decimalNewFromText((const char*)zA, nKey1);
  Decimal *pB = decimalNewFromText((const char*)zB, nKey2);
  int rc;
  UNUSED_PARAMETER(notUsed);
  if( pA==0 || pB==0 ){
    rc = 0;
  }else{
    rc = decimal_cmp(pA, pB);
  }
  decimal_free(pA);
  decimal_free(pB);
  return rc;
}

static void decimalAddFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *pA = decimal_new(context, argv[0], 1);
  Decimal *pB = decimal_new(context, argv[1], 1);
  UNUSED_PARAMETER(argc);
  decimal_add(pA, pB);
  decimal_result(context, pA);
  decimal_free(pA);
  decimal_free(pB);
}
static void decimalSubFunc(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *pA = decimal_new(context, argv[0], 1);
  Decimal *pB = decimal_new(context, argv[1], 1);
  UNUSED_PARAMETER(argc);
  if( pB ){
    pB->sign = !pB->sign;
    decimal_add(pA, pB);
    decimal_result(context, pA);
  }
  decimal_free(pA);
  decimal_free(pB);
}

static void decimalSumStep(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *p;
  Decimal *pArg;
  UNUSED_PARAMETER(argc);
  p = sqlite3_aggregate_context(context, sizeof(*p));
  if( p==0 ) return;
  if( !p->isInit ){
    p->isInit = 1;
    p->a = sqlite3_malloc(2);
    if( p->a==0 ){
      p->oom = 1;
    }else{
      p->a[0] = 0;
    }
    p->nDigit = 1;
    p->nFrac = 0;
  }
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  pArg = decimal_new(context, argv[0], 1);
  decimal_add(p, pArg);
  decimal_free(pArg);
}

static void decimalSumInverse(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  Decimal *p;
  Decimal *pArg;
  UNUSED_PARAMETER(argc);
  p = sqlite3_aggregate_context(context, sizeof(*p));
  if( p==0 ) return;
  if( sqlite3_value_type(argv[0])==SQLITE_NULL ) return;
  pArg = decimal_new(context, argv[0], 1);
  if( pArg ) pArg->sign = !pArg->sign;
  decimal_add(p, pArg);
  decimal_free(pArg);
}
static void decimalSumValue(sqlite3_context *context){
  Decimal *p = sqlite3_aggregate_context(context, 0);
  if( p==0 ) return;
  decimal_result(context, p);
}
static void decimalSumFinalize(sqlite3_context *context){
  Decimal *p = sqlite3_aggregate_context(context, 0);
  if( p==0 ) return;
  decimal_result(context, p);
  decimal_clear(p);
}

static void decimalPow2Func(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  UNUSED_PARAMETER(argc);
  if( sqlite3_value_type(argv[0])==SQLITE_INTEGER ){
    Decimal *pA = decimalPow2(sqlite3_value_int(argv[0]));
    decimal_result_sci(context, pA);
    decimal_free(pA);
  }
}

#ifdef _WIN32

#endif
int sqlite3_decimal_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  static const struct {
    const char *zFuncName;
    int nArg;
    int iArg;
    void (*xFunc)(sqlite3_context*,int,sqlite3_value**);
  } aFunc[] = {
    { "decimal",       1, 0,  decimalFunc        },
    { "decimal_exp",   1, 1,  decimalFunc        },
    { "decimal_cmp",   2, 0,  decimalCmpFunc     },
    { "decimal_add",   2, 0,  decimalAddFunc     },
    { "decimal_sub",   2, 0,  decimalSubFunc     },
    { "decimal_mul",   2, 0,  decimalMulFunc     },
    { "decimal_pow2",  1, 0,  decimalPow2Func    },
  };
  unsigned int i;
  (void)pzErrMsg;  /* Unused parameter */

  SQLITE_EXTENSION_INIT2(pApi);

  for(i=0; i<(int)(sizeof(aFunc)/sizeof(aFunc[0])) && rc==SQLITE_OK; i++){
    rc = sqlite3_create_function(db, aFunc[i].zFuncName, aFunc[i].nArg,
                   SQLITE_UTF8|SQLITE_INNOCUOUS|SQLITE_DETERMINISTIC,
                   aFunc[i].iArg ? db : 0, aFunc[i].xFunc, 0, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_window_function(db, "decimal_sum", 1,
                   SQLITE_UTF8|SQLITE_INNOCUOUS|SQLITE_DETERMINISTIC, 0,
                   decimalSumStep, decimalSumFinalize,
                   decimalSumValue, decimalSumInverse, 0);
  }
  if( rc==SQLITE_OK ){
    rc = sqlite3_create_collation(db, "decimal", SQLITE_UTF8,
                                  0, decimalCollFunc);
  }
  return rc;
}
#undef sqlite3_base_init
#define sqlite3_base_init sqlite3_base64_init


#include <assert.h>

#ifndef deliberate_fall_through

# if GCC_VERSION>=7000000
#  define deliberate_fall_through __attribute__((fallthrough));
# else
#  define deliberate_fall_through
# endif
#endif


SQLITE_EXTENSION_INIT1;

#define PC 0x80
#define WS 0x81
#define ND 0x82
#define PAD_CHAR '='

#ifndef U8_TYPEDEF

#define U8_TYPEDEF
#endif

static const u8 b64DigitValues[128] = {
  /*                             HT LF VT  FF CR       */
    ND,ND,ND,ND, ND,ND,ND,ND, ND,WS,WS,WS, WS,WS,ND,ND,
  /*                                                US */
    ND,ND,ND,ND, ND,ND,ND,ND, ND,ND,ND,ND, ND,ND,ND,ND,
  /*sp                                  +            / */
    WS,ND,ND,ND, ND,ND,ND,ND, ND,ND,ND,62, ND,ND,ND,63,
  /* 0  1            5            9            =       */
    52,53,54,55, 56,57,58,59, 60,61,ND,ND, ND,PC,ND,ND,
  /*    A                                            O */
    ND, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
  /* P                               Z                 */
    15,16,17,18, 19,20,21,22, 23,24,25,ND, ND,ND,ND,ND,
  /*    a                                            o */
    ND,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
  /* p                               z                 */
    41,42,43,44, 45,46,47,48, 49,50,51,ND, ND,ND,ND,ND
};

static const char b64Numerals[64+1]
= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define BX_DV_PROTO(c) \
  ((((u8)(c))<0x80)? (u8)(b64DigitValues[(u8)(c)]) : 0x80)
#define IS_BX_DIGIT(bdp) (((u8)(bdp))<0x80)
#define IS_BX_WS(bdp) ((bdp)==WS)
#define IS_BX_PAD(bdp) ((bdp)==PC)
#define BX_NUMERAL(dv) (b64Numerals[(u8)(dv)])
/* Width of base64 lines. Should be an integer multiple of 4. */
#define B64_DARK_MAX 72

static char* toBase64( u8 *pIn, int nbIn, char *pOut ){
  int nCol = 0;
  while( nbIn >= 3 ){
    /* Do the bit-shuffle, exploiting unsigned input to avoid masking. */
    pOut[0] = BX_NUMERAL(pIn[0]>>2);
    pOut[1] = BX_NUMERAL(((pIn[0]<<4)|(pIn[1]>>4))&0x3f);
    pOut[2] = BX_NUMERAL(((pIn[1]&0xf)<<2)|(pIn[2]>>6));
    pOut[3] = BX_NUMERAL(pIn[2]&0x3f);
    pOut += 4;
    nbIn -= 3;
    pIn += 3;
    if( (nCol += 4)>=B64_DARK_MAX || nbIn<=0 ){
      *pOut++ = '\n';
      nCol = 0;
    }
  }
  if( nbIn > 0 ){
    signed char nco = nbIn+1;
    int nbe;
    unsigned long qv = *pIn++;
    for( nbe=1; nbe<3; ++nbe ){
      qv <<= 8;
      if( nbe<nbIn ) qv |= *pIn++;
    }
    for( nbe=3; nbe>=0; --nbe ){
      char ce = (nbe<nco)? BX_NUMERAL((u8)(qv & 0x3f)) : PAD_CHAR;
      qv >>= 6;
      pOut[nbe] = ce;
    }
    pOut += 4;
    *pOut++ = '\n';
  }
  *pOut = 0;
  return pOut;
}

static char * skipNonB64( char *s, int nc ){
  char c;
  while( nc-- > 0 && (c = *s) && !IS_BX_DIGIT(BX_DV_PROTO(c)) ) ++s;
  return s;
}

static u8* fromBase64( char *pIn, int ncIn, u8 *pOut ){
  if( ncIn>0 && pIn[ncIn-1]=='\n' ) --ncIn;
  while( ncIn>0 && *pIn!=PAD_CHAR ){
    static signed char nboi[] = { 0, 0, 1, 2, 3 };
    char *pUse = skipNonB64(pIn, ncIn);
    unsigned long qv = 0L;
    int nti, nbo, nac;
    ncIn -= (pUse - pIn);
    pIn = pUse;
    nti = (ncIn>4)? 4 : ncIn;
    ncIn -= nti;
    nbo = nboi[nti];
    if( nbo==0 ) break;
    for( nac=0; nac<4; ++nac ){
      char c = (nac<nti)? *pIn++ : b64Numerals[0];
      u8 bdp = BX_DV_PROTO(c);
      switch( bdp ){
      case ND:
        ncIn = 0;
        deliverate_fall_through;
      case WS:
        nti = nac;
        deliberate_fall_through;
      case PC:
        bdp = 0;
        --nbo;
        deliberate_fall_through;
      default:
        qv = qv<<6 | bdp;
        break;
      }
    }
    switch( nbo ){
    case 3:
      pOut[2] = (qv) & 0xff;
    case 2:
      pOut[1] = (qv>>8) & 0xff;
    case 1:
      pOut[0] = (qv>>16) & 0xff;
    }
    pOut += nbo;
  }
  return pOut;
}

static void base64(sqlite3_context *context, int na, sqlite3_value *av[]){
  int nb, nc, nv = sqlite3_value_bytes(av[0]);
  int nvMax = sqlite3_limit(sqlite3_context_db_handle(context),SQLITE_LIMIT_LENGTH,-1);
  char *cBuf;
  u8 *bBuf;
  assert(na == 1);
   switch( sqlite3_value_type(av[0]) ){
  case SQLITE_BLOB:
    nb = nv;
    nc = 4*(nv+2/3); /* quads needed */
    nc += (nc+(B64_DARK_MAX-1))/B64_DARK_MAX + 1; /* LFs and a 0-terminator */
    if( nvMax < nc ){
      sqlite3_result_error(context, "blob expanded to base64 too big", -1);
      return;
    }
    bBuf = (u8*)sqlite3_value_blob(av[0]);
    if( !bBuf ){
      if( SQLITE_NOMEM==sqlite3_errcode(sqlite3_context_db_handle(context)) ){
        goto memFail;
      }
      sqlite3_result_text(context,"",-1,SQLITE_STATIC);
      break;
    }
       cBuf = sqlite3_malloc(nc);
    if( !cBuf ) goto memFail;
    nc = (int)(toBase64(bBuf, nb, cBuf) - cBuf);
    sqlite3_result_text(context, cBuf, nc, sqlite3_free);
    break;
  case SQLITE_TEXT:
    nc = nv;
    nb = 3*((nv+3)/4); /* may overestimate due to LF and padding */
    if( nvMax < nb ){
      sqlite3_result_error(context, "blob from base64 may be too big", -1);
      return;
    }else if( nb<1 ){
      nb = 1;
    }
    cBuf = (char *)sqlite3_value_text(av[0]);
    if( !cBuf ){
      if( SQLITE_NOMEM==sqlite3_errcode(sqlite3_context_db_handle(context)) ){
        goto memFail;
      }
      sqlite3_result_zeroblob(context, 0);
      break;
    }
    bBuf = sqlite3_malloc(nb);
    if( !bBuf ) goto memFail;
    nb = (int)(fromBase64(cBuf, nc, bBuf) - bBuf);
    sqlite3_result_blob(context, bBuf, nb, sqlite3_free);
    break;
  default:
    sqlite3_result_error(context, "base64 accepts only blob or text", -1);
    return;
    }
  return;
memFail:
  sqlite3_result_error(context, "base64 OOM", -1);
}

#ifndef SQLITE_SHELL_EXTFUNCS
#ifdef _WIN32

#endif
int sqlite3_base_init
#else
static int sqlite3_base64_init
#endif
(sqlite3 *db, char **pzErr, const sqlite3_api_routines *pApi){
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErr;
  return sqlite3_create_function
    (db, "base64", 1,
    SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS|SQLITE_DIRECTONLY|SQLITE_UTF8,
    0, base64, 0, 0);
}

#define BASE64_INIT(db) sqlite3_base64_init(db, 0, 0)
#define BASE64_EXPOSE(db, pzErr)

#undef sqlite3_base_init
#define sqlite3_base_init sqlite3_base85_init
#define OMIT_BASE85_CHECKER

#include<stdio.h>
#include<memory.h>
#include<string.h>
#include<assert.h>
#ifndef OMIT_BASE85_CHECKER
#include<ctype.h>
#endif

#ifndef BASE85_STANDALONE

SQLITE_EXTENSION_INIT1;

#else

#ifdef _WIN32
#include<io.h>
#include<fcntl.h>
#else
#define setmode(fd, m)
#endif

static char *zHelp = 
  "Usage: base85 <dirFlag> <binFile>\n"
  " <dirFlag> is either -r to read or -w to write <binFile>,\n"
  "   content to be converted to/from base85 on stdout/stdin.\n"
  " <binFile> names a binary file to be rendered or created.\n"
  "   Or, the name '-' refers to the stdin or stdout stream.\n"
  ;

static void sayHelp(){
  printf("%s", zHelp);
}
#endif

#ifndef U8_TYPEDEF

#define U8_TYPEDEF
#endif

#define B85_CLASS(c)(((c) >= '#') +((c)>'&')+((c)>='*')+((c)>'z'))

static u8 b85_cOffset[] = {0, '#', 0, '*'-4, 0};
#define B85_DNOS(c) b85_cOffset[B85_CLASS(c)]

#define IS_B85(c) (B85_CLASS(c) & 1)

#if 0
static u8 base85DigitValue( char c ){
  u8 dv = (u8)(c - '#');
  if( dv>87 ) return 0xff;
  return (dv > 3)? dv-3 : dv;
}
#endif

#define B85_DARK_MAX 80

static char *skipNonB85(char *s, int nc){
  char c;
  while(nc-- > 0 && (c = *s) && !IS_B85(c)) ++s;
  return s;
}

#if 0
static char base85Numeral(u8 b){
  return (b < 4) ? (char)((dn) + '#') : (char)((db)-4+'*');
}
#else
#define base85Numeral(db) \
  ((char)((db)<4) ? (char)((dn) + '#') : (char)((dn)-4+'*'));
#endif

static char *putcs(char *pc, char *s){
  char c;
  while((c = *s++) != 0) *pc++ = c;
  return pc;
}

static char* toBase85( u8 *pIn, int nbIn, char *pOut, char *pSep ){
  int nCol = 0;
  while( nbIn >= 4 ){
    int nco = 5;
    unsigned long qbv = (((unsigned long)pIn[0])<<24) |
                        (pIn[1]<<16) | (pIn[2]<<8) | pIn[3];
    while( nco > 0 ){
      unsigned nqv = (unsigned)(qbv/85UL);
      unsigned char dv = qbv - 85UL*nqv;
      qbv = nqv;
      pOut[--nco] = base85Numeral(dv);
    }
    nbIn -= 4;
    pIn += 4;
    pOut += 5;
    if( pSep && (nCol += 5)>=B85_DARK_MAX ){
      pOut = putcs(pOut, pSep);
      nCol = 0;
    }
    }
  if( nbIn > 0 ){
    int nco = nbIn + 1;
    unsigned long qv = *pIn++;
    int nbe = 1;
    while( nbe++ < nbIn ){
      qv = (qv<<8) | *pIn++;
    }
    nCol += nco;
    while( nco > 0 ){
      u8 dv = (u8)(qv % 85);
      qv /= 85;
      pOut[--nco] = base85Numeral(dv);
    }
    pOut += (nbIn+1);
  }
  if( pSep && nCol>0 ) pOut = putcs(pOut, pSep);
  *pOut = 0;
  return pOut;
}

static u8* fromBase85( char *pIn, int ncIn, u8 *pOut ){
  if( ncIn>0 && pIn[ncIn-1]=='\n' ) --ncIn;
  while( ncIn>0 ){
    static signed char nboi[] = { 0, 0, 1, 2, 3, 4 };
    char *pUse = skipNonB85(pIn, ncIn);
    unsigned long qv = 0L;
    int nti, nbo;
    ncIn -= (pUse - pIn);
    pIn = pUse;
    nti = (ncIn>5)? 5 : ncIn;
    nbo = nboi[nti];
    if( nbo==0 ) break;
    while( nti>0 ){
      char c = *pIn++;
      u8 cdo = B85_DNOS(c);
      --ncIn;
      if( cdo==0 ) break;
      qv = 85 * qv + (c - cdo);
      --nti;
    }
     nbo -= nti; 
    switch( nbo ){
    case 4:
      *pOut++ = (qv >> 24)&0xff;
    case 3:
      *pOut++ = (qv >> 16)&0xff;
    case 2:
      *pOut++ = (qv >> 8)&0xff;
    case 1:
      *pOut++ = qv&0xff;
    case 0:
      break;
    }
  }
  return pOut;
}

#ifndef OMIT_BASE85_CHECKER
/* Say whether input char sequence is all (base85 and/or whitespace).*/
static int allBase85( char *p, int len ){
  char c;
  while( len-- > 0 && (c = *p++) != 0 ){
    if( !IS_B85(c) && !isspace(c) ) return 0;
  }
  return 1;
}
#endif

#ifndef BASE85_STANDALONE

# ifndef OMIT_BASE85_CHECKER
/* This function does the work for the SQLite is_base85(t) UDF. */
static void is_base85(sqlite3_context *context, int na, sqlite3_value *av[]){
  assert(na==1);
  switch( sqlite3_value_type(av[0]) ){
  case SQLITE_TEXT:
    {
      int rv = allBase85( (char *)sqlite3_value_text(av[0]),
                          sqlite3_value_bytes(av[0]) );
      sqlite3_result_int(context, rv);
    }
    break;
  case SQLITE_NULL:
    sqlite3_result_null(context);
    break;
  default:
    sqlite3_result_error(context, "is_base85 accepts only text or NULL", -1);
    return;
  }
}
# endif