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

