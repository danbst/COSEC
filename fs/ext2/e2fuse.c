#include <stdlib.h>
#include <stdio.h>
#include <sys/errno.h>

#define ETODO ENOTSUP 

#define FUSE_API_VERSION 26
#define FUSE_USE_VERSION 26

#include <fuse.h>
#include "ext2.h"

static void * fuse_ext2_init(struct fuse_conn_info *);
static void   fuse_ext2_destroy(void *);

static int fuse_ext2_getattr(const char *, struct stat *);
static int fuse_ext2_readlink(const char *, char *, size_t);
static int fuse_ext2_getdir(const char *, fuse_dirh_t, fuse_dirfil_t);
static int fuse_ext2_mknod(const char *, mode_t, dev_t);
static int fuse_ext2_mkdir(const char *, mode_t);
static int fuse_ext2_unlink(const char *);
static int fuse_ext2_rmdir(const char *);
static int fuse_ext2_symlink(const char *, const char *);
static int fuse_ext2_rename(const char *, const char *);
static int fuse_ext2_link(const char *, const char *);
static int fuse_ext2_chmod(const char *, mode_t);
static int fuse_ext2_chown(const char *, uid_t, gid_t);
static int fuse_ext2_truncate(const char *, off_t);
static int fuse_ext2_utime(const char *, struct utimbuf *);

static int fuse_ext2_access(const char *, int);
static int fuse_ext2_open(const char *, struct fuse_file_info *);
static int fuse_ext2_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
static int fuse_ext2_write(const char *, const char *, size_t, off_t,struct fuse_file_info *);
static int fuse_ext2_statfs(const char *, struct statvfs *);
static int fuse_ext2_flush(const char *, struct fuse_file_info *);
static int fuse_ext2_release(const char *, struct fuse_file_info *);
static int fuse_ext2_fsync(const char *, int, struct fuse_file_info *);
#ifndef __APPLE__
static int fuse_ext2_setxattr(const char *, const char *, const char *, size_t, int);
static int fuse_ext2_getxattr(const char *, const char *, char *, size_t);
#else
static int fuse_ext2_setxattr(const char *, const char *, const char *, size_t, int, uint32_t);
static int fuse_ext2_getxattr(const char *, const char *, char *, size_t, uint32_t);
#endif

static int fuse_ext2_listxattr(const char *, char *, size_t);
static int fuse_ext2_removexattr(const char *, const char *);


struct fuse_operations fs_ops = {
    .getattr =      fuse_ext2_getattr,
    .readlink =     fuse_ext2_readlink,
    .getdir =       fuse_ext2_getdir,
    .mknod =        fuse_ext2_mknod, 
    .mkdir =        fuse_ext2_mkdir,
    .unlink =       fuse_ext2_unlink,
    .rmdir =        fuse_ext2_rmdir,
    .symlink =      fuse_ext2_symlink,
    .rename =       fuse_ext2_rename,
    .link =         fuse_ext2_link,
    .chmod =        fuse_ext2_chmod,
    .chown =        fuse_ext2_chown,
    .truncate =     fuse_ext2_truncate,
    .utime =        fuse_ext2_utime,
    .access =       fuse_ext2_access,
    .open =         fuse_ext2_open,
    .read =         fuse_ext2_read,
    .write =        fuse_ext2_write,
    .statfs =       fuse_ext2_statfs,
    .flush =        fuse_ext2_flush,
    .release =      fuse_ext2_release,
    .fsync =        fuse_ext2_fsync,
    .setxattr =     fuse_ext2_setxattr,
    .getxattr =     fuse_ext2_getxattr,
    .listxattr =    fuse_ext2_listxattr,
    .removexattr =  fuse_ext2_removexattr,
    
    .init =         fuse_ext2_init,
    .destroy =      fuse_ext2_destroy,
};



static void * fuse_ext2_init(struct fuse_conn_info *conn) {
    printf("fuse_conn_info: \n");
    printf("  version: %d.%d\n", conn->proto_major, conn->proto_minor);
    //printf("  
    return NULL;
}

static void   fuse_ext2_destroy(void *data) {
}

static int fuse_ext2_getattr(const char *path, struct stat *stat) {
    return -ETODO;
}

static int fuse_ext2_readlink(const char *path, char *buf, size_t n) {
    return -ETODO;
}

static int fuse_ext2_getdir(const char *path, fuse_dirh_t fuse_dirh, fuse_dirfil_t dirfil) {
    return -ETODO;
}

static int fuse_ext2_mknod(const char *path, mode_t mode, dev_t dev) {
    return -ETODO;
}
static int fuse_ext2_mkdir(const char *path, mode_t mode) {
    return -ETODO;
}

static int fuse_ext2_unlink(const char *path) {
    return -ETODO;
}

static int fuse_ext2_rmdir(const char *path) {
    return -ETODO;
}

static int fuse_ext2_symlink(const char *path, const char *sympath) {
    return -ETODO;
}

static int fuse_ext2_rename(const char *path, const char *newpath) {
    return -ETODO;
}

static int fuse_ext2_link(const char *path, const char *lnpath) {
    return -ETODO;
}

static int fuse_ext2_chmod(const char *path, mode_t mode) {
    return -ETODO;
}

static int fuse_ext2_chown(const char *path, uid_t uid, gid_t gid) {
    return -ETODO;
}

static int fuse_ext2_truncate(const char *path, off_t size) {
    return -ETODO;
}

static int fuse_ext2_utime(const char *path, struct utimbuf *ut) {
    return -ETODO;
}

static int fuse_ext2_access(const char *path, int access_mode) {
    return 0;
}

static int fuse_ext2_open(const char *path, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

static int fuse_ext2_read(const char *path, char *buf, size_t bufsize, off_t at, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

static int fuse_ext2_write(const char *path, const char *buf, size_t bufsize, off_t at, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

static int fuse_ext2_statfs(const char *path, struct statvfs *stat) {
    return -ETODO;
}

static int fuse_ext2_flush(const char *path, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

static int fuse_ext2_release(const char *path, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

static int fuse_ext2_fsync(const char *path, int n, struct fuse_file_info *fuse_finfo) {
    return -ETODO;
}

#ifndef __APPLE__
static int fuse_ext2_setxattr(const char *path, const char *arg2, const char *arg3, size_t size, int n) {
#else
static int fuse_ext2_setxattr(const char *path, const char *arg2, const char *arg3, size_t size, int n, uint32_t arg) {
#endif
    return -ETODO;
}

#ifndef __APPLE__
static int fuse_ext2_getxattr(const char *path, const char *arg2, char *buf, size_t bufsize) {
#else
static int fuse_ext2_getxattr(const char *path, const char *arg2, char *buf, size_t bufsize, uint32_t arg) {
#endif
    return -ETODO;
}

static int fuse_ext2_listxattr(const char *path, char *buf, size_t bufsize) {
    return -ETODO;
}

static int fuse_ext2_removexattr(const char *path, const char *attr) {
    return -ETODO;
}


int main(int argc, char **argv) {
    fuse_main(argc, argv, &fs_ops, NULL);
    return 0;
}
