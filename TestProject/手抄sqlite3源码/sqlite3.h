#ifndef SQLITE3_H
#define SQLITE3_H
#include<stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQLITE_EXTERN
#define SQLITE_EXTERN extern
#endif
#ifndef SQLITE_API
#define SQLITE_API
#endif
#ifndef SQLITE_CDECL
#define SQLITE_CDECL
#endif
#ifndef SQLITE_STDCALL
#define SQLITE_STDCALL SQLITE_APICALL
#endif
#ifndef SQLITE_CALLBACK
#define SQLITE_CALLBACK
#endif
#ifndef SQLITE_SYSAPI
#define SQLITE_SYSAPI
#endif

#define SQLITE_DEPRECATED
#define SQLITE_EXPERIMENTAL

#ifdef SQLITE_VERSION
#undef SQLITE_VERSION
#endif
#ifdef SQLITE_VERSION_NUMBER
#undef SQLITE_VERSION_NUMBER
#endif

#define SQLITE_VERSION    "xxx"
#define SQLITE_VERSION_NUMBER xxxx
#define SQLITE_SOURCE_ID    "xxxx"

SQLITE_API SQLITE_EXTERN const char sqlite3_version[];
SQLITE_API const char *sqlite3_libversion(void);
SQLITE_API const char *sqlite3_sourceid(void);
SQLITE_API int sqlite3_libversion_number(void);

#ifndef SQLITE_OMIT_COMPILEOPTION_DIAGS
SQLITE_API int sqlite3_compileoption_used(const char *zOptName);
SQLITE_API const char *sqlite3_compileoption_get(int N);
#else
#define sqlite3_comileoption_used(x) 0
#define sqlite3_comileoption_get(x) ((void*)0)
#endif

SQLITE_API int sqlite3_threadsafe(void);

typedef struct sqlite3 sqlite3;

#ifdef SQLITE_INT64_TYPE
    typedef SQLITE_INT64_TYPE sqlite_int64;
#ifdef SQLITE_UINT64_TYPE
    typedef SQLITE_UINT64_TYPE sqlite_uint64;
#else
    typedef unsigned SQLITE_INT64_TYPE sqlite_uint64;
#endif
#elif defined(_MSC_VER) || defined(__BORLANDC__)
    typedef __int64 sqlite_int64;
    typedef unsigned __int64 sqlite_uint64;
#else
    typedef long long int sqlite_int64;
    typedef unsigned long long int sqlite_uint64;
#endif
typedef sqlite_int64 sqlite3_int64;
typedef sqlite_uint64 sqlite3_uint64;

#ifdef SQLITE_OMIT_FLOATING_POINT
#define double sqlite3_int64
#endif

SQLITE_API int sqlite3_close(sqlite3*);
SQLITE_API int sqlite3_close_v2(sqlite3*);

typedef int (*sqlite3_callback)(void*, int, char**, char**);

SQLITE_API int sqlite3_exec(
    sqlite3*,
    const char *sql,
    int (*callback)(void*, int, char**, char**),
    void *,
    char **errmsg
);

#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Internal use only */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Not used */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */


#define SQLITE_ERROR_MISSING_COLLSEQ   (SQLITE_ERROR | (1<<8))
#define SQLITE_ERROR_RETRY             (SQLITE_ERROR | (2<<8))
#define SQLITE_ERROR_SNAPSHOT          (SQLITE_ERROR | (3<<8))
#define SQLITE_IOERR_READ              (SQLITE_IOERR | (1<<8))
#define SQLITE_IOERR_SHORT_READ        (SQLITE_IOERR | (2<<8))
#define SQLITE_IOERR_WRITE             (SQLITE_IOERR | (3<<8))
#define SQLITE_IOERR_FSYNC             (SQLITE_IOERR | (4<<8))
#define SQLITE_IOERR_DIR_FSYNC         (SQLITE_IOERR | (5<<8))
#define SQLITE_IOERR_TRUNCATE          (SQLITE_IOERR | (6<<8))
#define SQLITE_IOERR_FSTAT             (SQLITE_IOERR | (7<<8))
#define SQLITE_IOERR_UNLOCK            (SQLITE_IOERR | (8<<8))
#define SQLITE_IOERR_RDLOCK            (SQLITE_IOERR | (9<<8))
#define SQLITE_IOERR_DELETE            (SQLITE_IOERR | (10<<8))
#define SQLITE_IOERR_BLOCKED           (SQLITE_IOERR | (11<<8))
#define SQLITE_IOERR_NOMEM             (SQLITE_IOERR | (12<<8))
#define SQLITE_IOERR_ACCESS            (SQLITE_IOERR | (13<<8))
#define SQLITE_IOERR_CHECKRESERVEDLOCK (SQLITE_IOERR | (14<<8))
#define SQLITE_IOERR_LOCK              (SQLITE_IOERR | (15<<8))
#define SQLITE_IOERR_CLOSE             (SQLITE_IOERR | (16<<8))
#define SQLITE_IOERR_DIR_CLOSE         (SQLITE_IOERR | (17<<8))
#define SQLITE_IOERR_SHMOPEN           (SQLITE_IOERR | (18<<8))
#define SQLITE_IOERR_SHMSIZE           (SQLITE_IOERR | (19<<8))
#define SQLITE_IOERR_SHMLOCK           (SQLITE_IOERR | (20<<8))
#define SQLITE_IOERR_SHMMAP            (SQLITE_IOERR | (21<<8))
#define SQLITE_IOERR_SEEK              (SQLITE_IOERR | (22<<8))
#define SQLITE_IOERR_DELETE_NOENT      (SQLITE_IOERR | (23<<8))
#define SQLITE_IOERR_MMAP              (SQLITE_IOERR | (24<<8))
#define SQLITE_IOERR_GETTEMPPATH       (SQLITE_IOERR | (25<<8))
#define SQLITE_IOERR_CONVPATH          (SQLITE_IOERR | (26<<8))
#define SQLITE_IOERR_VNODE             (SQLITE_IOERR | (27<<8))
#define SQLITE_IOERR_AUTH              (SQLITE_IOERR | (28<<8))
#define SQLITE_IOERR_BEGIN_ATOMIC      (SQLITE_IOERR | (29<<8))
#define SQLITE_IOERR_COMMIT_ATOMIC     (SQLITE_IOERR | (30<<8))
#define SQLITE_IOERR_ROLLBACK_ATOMIC   (SQLITE_IOERR | (31<<8))
#define SQLITE_IOERR_DATA              (SQLITE_IOERR | (32<<8))
#define SQLITE_IOERR_CORRUPTFS         (SQLITE_IOERR | (33<<8))
#define SQLITE_IOERR_IN_PAGE           (SQLITE_IOERR | (34<<8))
#define SQLITE_LOCKED_SHAREDCACHE      (SQLITE_LOCKED |  (1<<8))
#define SQLITE_LOCKED_VTAB             (SQLITE_LOCKED |  (2<<8))
#define SQLITE_BUSY_RECOVERY           (SQLITE_BUSY   |  (1<<8))
#define SQLITE_BUSY_SNAPSHOT           (SQLITE_BUSY   |  (2<<8))
#define SQLITE_BUSY_TIMEOUT            (SQLITE_BUSY   |  (3<<8))
#define SQLITE_CANTOPEN_NOTEMPDIR      (SQLITE_CANTOPEN | (1<<8))
#define SQLITE_CANTOPEN_ISDIR          (SQLITE_CANTOPEN | (2<<8))
#define SQLITE_CANTOPEN_FULLPATH       (SQLITE_CANTOPEN | (3<<8))
#define SQLITE_CANTOPEN_CONVPATH       (SQLITE_CANTOPEN | (4<<8))
#define SQLITE_CANTOPEN_DIRTYWAL       (SQLITE_CANTOPEN | (5<<8)) /* Not Used */
#define SQLITE_CANTOPEN_SYMLINK        (SQLITE_CANTOPEN | (6<<8))
#define SQLITE_CORRUPT_VTAB            (SQLITE_CORRUPT | (1<<8))
#define SQLITE_CORRUPT_SEQUENCE        (SQLITE_CORRUPT | (2<<8))
#define SQLITE_CORRUPT_INDEX           (SQLITE_CORRUPT | (3<<8))
#define SQLITE_READONLY_RECOVERY       (SQLITE_READONLY | (1<<8))
#define SQLITE_READONLY_CANTLOCK       (SQLITE_READONLY | (2<<8))
#define SQLITE_READONLY_ROLLBACK       (SQLITE_READONLY | (3<<8))
#define SQLITE_READONLY_DBMOVED        (SQLITE_READONLY | (4<<8))
#define SQLITE_READONLY_CANTINIT       (SQLITE_READONLY | (5<<8))
#define SQLITE_READONLY_DIRECTORY      (SQLITE_READONLY | (6<<8))
#define SQLITE_ABORT_ROLLBACK          (SQLITE_ABORT | (2<<8))
#define SQLITE_CONSTRAINT_CHECK        (SQLITE_CONSTRAINT | (1<<8))
#define SQLITE_CONSTRAINT_COMMITHOOK   (SQLITE_CONSTRAINT | (2<<8))
#define SQLITE_CONSTRAINT_FOREIGNKEY   (SQLITE_CONSTRAINT | (3<<8))
#define SQLITE_CONSTRAINT_FUNCTION     (SQLITE_CONSTRAINT | (4<<8))
#define SQLITE_CONSTRAINT_NOTNULL      (SQLITE_CONSTRAINT | (5<<8))
#define SQLITE_CONSTRAINT_PRIMARYKEY   (SQLITE_CONSTRAINT | (6<<8))
#define SQLITE_CONSTRAINT_TRIGGER      (SQLITE_CONSTRAINT | (7<<8))
#define SQLITE_CONSTRAINT_UNIQUE       (SQLITE_CONSTRAINT | (8<<8))
#define SQLITE_CONSTRAINT_VTAB         (SQLITE_CONSTRAINT | (9<<8))
#define SQLITE_CONSTRAINT_ROWID        (SQLITE_CONSTRAINT |(10<<8))
#define SQLITE_CONSTRAINT_PINNED       (SQLITE_CONSTRAINT |(11<<8))
#define SQLITE_CONSTRAINT_DATATYPE     (SQLITE_CONSTRAINT |(12<<8))
#define SQLITE_NOTICE_RECOVER_WAL      (SQLITE_NOTICE | (1<<8))
#define SQLITE_NOTICE_RECOVER_ROLLBACK (SQLITE_NOTICE | (2<<8))
#define SQLITE_NOTICE_RBU              (SQLITE_NOTICE | (3<<8))
#define SQLITE_WARNING_AUTOINDEX       (SQLITE_WARNING | (1<<8))
#define SQLITE_AUTH_USER               (SQLITE_AUTH | (1<<8))
#define SQLITE_OK_LOAD_PERMANENTLY     (SQLITE_OK | (1<<8))
#define SQLITE_OK_SYMLINK              (SQLITE_OK | (2<<8)) /* internal use only */

#define SQLITE_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define SQLITE_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define SQLITE_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define SQLITE_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MEMORY           0x00000080  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define SQLITE_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define SQLITE_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define SQLITE_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define SQLITE_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define SQLITE_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define SQLITE_OPEN_SUPER_JOURNAL    0x00004000  /* VFS only */
#define SQLITE_OPEN_NOMUTEX          0x00008000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_SHAREDCACHE      0x00020000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_PRIVATECACHE     0x00040000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_WAL              0x00080000  /* VFS only */
#define SQLITE_OPEN_NOFOLLOW         0x01000000  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_EXRESCODE        0x02000000  /* Extended result codes */

#define SQLITE_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */

#define SQLITE_IOCAP_ATOMIC                 0x00000001
#define SQLITE_IOCAP_ATOMIC512              0x00000002
#define SQLITE_IOCAP_ATOMIC1K               0x00000004
#define SQLITE_IOCAP_ATOMIC2K               0x00000008
#define SQLITE_IOCAP_ATOMIC4K               0x00000010
#define SQLITE_IOCAP_ATOMIC8K               0x00000020
#define SQLITE_IOCAP_ATOMIC16K              0x00000040
#define SQLITE_IOCAP_ATOMIC32K              0x00000080
#define SQLITE_IOCAP_ATOMIC64K              0x00000100
#define SQLITE_IOCAP_SAFE_APPEND            0x00000200
#define SQLITE_IOCAP_SEQUENTIAL             0x00000400
#define SQLITE_IOCAP_UNDELETABLE_WHEN_OPEN  0x00000800
#define SQLITE_IOCAP_POWERSAFE_OVERWRITE    0x00001000
#define SQLITE_IOCAP_IMMUTABLE              0x00002000
#define SQLITE_IOCAP_BATCH_ATOMIC           0x00004000

#define SQLITE_LOCK_NONE          0       /* xUnlock() only */
#define SQLITE_LOCK_SHARED        1       /* xLock() or xUnlock() */
#define SQLITE_LOCK_RESERVED      2       /* xLock() only */
#define SQLITE_LOCK_PENDING       3       /* xLock() only */
#define SQLITE_LOCK_EXCLUSIVE     4       /* xLock() only */

#define SQLITE_SYNC_NORMAL        0x00002
#define SQLITE_SYNC_FULL          0x00003
#define SQLITE_SYNC_DATAONLY      0x00010

typedef struct sqlite3_file sqlite3_file;
struct sqlite3_file {
  const struct sqlite3_io_methods *pMethods;  /* Methods for an open file */
};

typedef struct sqlite3_io_methods sqlite3_io_methods;
struct sqlite3_io_methods{
    int iVerson;
    int (*xClose)(sqlite3_file*);
    int (*xRead)(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
    int (*xWrite)(sqlite3_file*, const void*, int iAmt, sqlite3_int64 iOfst);
    int (*xTruncate)(sqlite3_file*, sqlite3_int64 size);
    int (*xSync)(sqlite3_file*, int flags);
    int (*xFileSize)(sqlite3_file*, sqlite3_int64 *pSize);
    int (*xLock)(sqlite3_file*, int);
    int (*Unlock)(sqlite3_file*, int);
    int (*xCheckReservedLock)(sqlite3_file*, int *pResOut);
    int (*xFileControl)(sqlite3_file*, int op, void *pArg);
    int (*xSectorSize)(sqlite3_file*);
    int (*xDeviceCharacteristics)(sqlite3_file*);
    int (*xShmMap)(sqlite3_file*, int iPg, int pgsz, int, void volatile**);
    int (*xShmLock)(sqlite3_file*, int offset, int n, int flags);
    void (*xShmBarrier)(sqlite3_file*);
    int (*xShmUnmap)(sqlite3_file*, int deleteFlag);
    int (*xFetch)(sqlite3_file*, sqlite3_int64 iOfst, int iAmt, void **pp);
    int (*xUnfetch)(sqlite3_file*, sqlite3_int64 iOfst, void *p);
};