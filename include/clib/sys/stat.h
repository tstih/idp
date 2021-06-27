/*
 * sys/stat.h
 *
 * standard C header file
 * 
 * NOTES:
 *  Based on https://github.com/dmo9000/cpmlibc
 * 
 * MIT License (see: LICENSE)
 * copyright (c) 2021 tomaz stih
 *
 * 01.05.2021   tstih
 *
 */
#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#include <sys/types.h>

struct stat
{
    short   st_mode;                    /* flags */
    long    st_atime;                   /* access time */
    long    st_mtime;                   /* modification time */
    long    st_size;                    /* file size in bytes */
};

/* Flag bits in st_mode */
#define S_IFMT          0x600           /* type bits */
#define S_IFDIR         0x400           /* is a directory */
#define S_IFREG         0x200           /* is a regular file */
#define S_IREAD         0400            /* file can be read */
#define S_IWRITE        0200            /* file can be written */
#define S_IEXEC         0100            /* file can be executed */
#define S_HIDDEN        0x1000          /* file is hidden */
#define S_SYSTEM        0x2000          /* file is marked system */
#define S_ARCHIVE       0x4000          /* file has been written to */

/* Tests */
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

/* Set file mode. */
extern int chmod(const char *path, mode_t mode);

/* Get file info. */
extern int stat(char *path, struct stat *buf);

#endif /* __SYS_STAT_H__ */
