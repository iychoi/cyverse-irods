/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/*** This code is rewritten by Illyoung Choi (iychoi@email.arizona.edu)    ***
 *** funded by iPlantCollaborative (www.iplantcollaborative.org).          ***/
#ifndef IFUSE_FS_HPP
#define IFUSE_FS_HPP

#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include "iFuse.Lib.hpp"
#include "iFuse.Lib.Fd.hpp"
#include "iFuse.Lib.Conn.hpp"

#define DEF_FILE_MODE	0660
#define DEF_DIR_MODE	0770
#define FILE_BLOCK_SIZE	512
#define DIR_SIZE        4096

#define IOCTL_APP_NUMBER 0xEE

#define IFUSEIOC_RESET_METADATA_CACHE _IO(IOCTL_APP_NUMBER, 0)
#define IFUSEIOC_SHOW_CONNECTIONS _IOR(IOCTL_APP_NUMBER, 1, iFuseFsConnReport_t)

typedef int (*iFuseDirFiller) (void *buf, const char *name, const struct stat *stbuf, off_t off);

void iFuseFsInit();
void iFuseFsDestroy();
int iFuseFsGetAttr(const char *iRodsPath, struct stat *stbuf);
int iFuseFsOpen(const char *iRodsPath, iFuseFd_t **iFuseFd, int openFlag);
int iFuseFsClose(iFuseFd_t *iFuseFd);
int iFuseFsRead(iFuseFd_t *iFuseFd, char *buf, off_t off, size_t size);
int iFuseFsWrite(iFuseFd_t *iFuseFd, const char *buf, off_t off, size_t size);
int iFuseFsFlush(iFuseFd_t *iFuseFd);
int iFuseFsCreate(const char *iRodsPath, mode_t mode);
int iFuseFsUnlink(const char *iRodsPath);
int iFuseFsOpenDir(const char *iRodsPath, iFuseDir_t **iFuseDir);
int iFuseFsCloseDir(iFuseDir_t *iFuseDir);
int iFuseFsReadDir(iFuseDir_t *iFuseDir, iFuseDirFiller filler, void *buf, off_t offset);
int iFuseFsMakeDir(const char *iRodsPath, mode_t mode);
int iFuseFsRemoveDir(const char *iRodsPath);
int iFuseFsRename(const char *iRodsFromPath, const char *iRodsToPath);
int iFuseFsTruncate(const char *iRodsPath, off_t size);
int iFuseFsChmod(const char *iRodsPath, mode_t mode);
int iFuseFsIoctl(const char *iRodsPath, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data);
int iFuseFsCacheDir(const char *iRodsPath);

#endif	/* IFUSE_FS_HPP */
