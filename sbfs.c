#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>




int sbfs_mknod(const char *pathname, mode_t mode, dev_t dev){
	//only care about pathname for now
};


int sbfs_mkdir(const char *path, mode_t mode){

};

int sbfs_rmdir(const char *path){

};

int sbfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){

};

int sbfs_chmod(const char *path, mode_t mode){

};

int sbfs_chown(const char *path, uid_t uid, gid_t gid){

};


int sbfs_unlink(const char *path){

};

int sbfs_open(char *pathname, struct fuse_file_info *fi){
	int fd;
	fd = sys_open(pathname, fi->flags, 0);
	fi->fh = fd;

	if(fd >= 0) return 0;
	return -1;
};

int sbfs_opendir(const char *path. struct fuse_file_info *fi){

};

int sbfs_read(char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int count =  sys_read(fi->fh, buf, size, offset);
	return count;
};

int sbfs_write(char *pathname, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int count = sys_write(fi->fh, buf, size, offset);
	return count;
};

void sbfs_init(struct fuse_conn_info *conn){

}

struct fuse_operation sbfs_operations = {
	.mkdir = sbfs_mkdir, 
	.mknod = sbfs_mknod,
	.rmdir = sbfs_rmdir,
	.readdir = sbfs_readdir,
	.unlink  = sbfs_unlink,
	.open = sbfs_open,
	.opendir = sbfs_opendir,
	.read = sbfs_read,
	.write = sbfs_write,
	.init = sbfs_init
};

int main(int argc, char *argv[]){

}

