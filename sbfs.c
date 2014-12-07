 //#define _FILE_OFFSET_BITS 64
 #define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include "sys.h"
#include "inode.h"
#include "sb_mkfs.h"
#include "dir.h"
#include "dbg.h"




int sbfs_mknod(const char *pathname, mode_t mode, dev_t dev){
	sys_mknod(pathname, 1, mode);
	return 0;
};


int sbfs_mkdir(const char *path, mode_t mode){
	sys_mkdir(path, 2);
	return 0;
};

int sbfs_rmdir(const char *path){

};

int sbfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	log_info("READDIR CALLED!");
	int retstat = 0;

	sbfs_core_inode *inode = namei(path);
	if(inode == NULL){
		return -ENOENT;
	}

	if(inode->d_inode.type != 2){
		return -ENOENT;
	}
	int increment = -1;
	struct dir_entry *entry = malloc(sizeof(struct dir_entry)); 
	while(1){
	 	increment++;
	 	entry = get_dir(inode, increment*(sizeof(struct dir_entry))); 
	 			
		if((entry == NULL)){ //end of entries for this directory
			log_info("reached end of entries");
			free(entry);
			return 0; 
		}

		if(filler(buf, entry->name, NULL, 0) != 0){
			log_info("Memory error: buffer full");
			free(entry);
			return -ENOMEM;
		}
	}
	free(entry);
	return retstat;
}

int sbfs_chmod(const char *path, mode_t mode){

};

int sbfs_chown(const char *path, uid_t uid, gid_t gid){

};


int sbfs_unlink(const char *path){
	log_info("unlink called");
	return sys_unlink(path);
};

int sbfs_open(const char *pathname, struct fuse_file_info *fi){
	log_info("open called");
	int fd;
	fd = sys_open(pathname, fi->flags, 0);
	fi->fh = fd;

	if(fd >= 0) return 0;
	return -1;
};

int sbfs_opendir(const char *path, struct fuse_file_info *fi){
	log_info("open dir called");
	return 0;
};

int sbfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	log_info("read called.");
	int count =  sys_read(fi->fh, buf, size, offset);
	return count;
};

int sbfs_write(const char *pathname, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int count = sys_write(fi->fh, buf, size, offset);
	return count;
};

void sbfs_init(struct fuse_conn_info *conn){
	//return (FILE*)fuse_get_context()->private_data;
}

int sbfs_getattr(const char *path, struct stat *stbuf){
	log_info("getattr called!!");
	int res = 0;
	sbfs_core_inode *inode = namei(path);
	memset(stbuf, 0, sizeof(struct stat));
	if(inode == NULL){
		return -ENOENT;
	}
	log_info("inode nmbr: %d", inode->i_nmbr);
	stbuf->st_size = inode->d_inode.size;
	stbuf->st_ino = inode->i_nmbr;
	stbuf->st_gid = inode->d_inode.grp_id;
	stbuf->st_uid = inode->d_inode.user_id;
	stbuf->st_mode = S_IFDIR | 0755;
	stbuf->st_nlink = 2;
	stbuf->st_atime = inode->d_inode.a_time;
	stbuf->st_mtime = inode->d_inode.m_time;

	return res;
}





struct fuse_operations sbfs_operations = {
	.getattr = sbfs_getattr,
	.mkdir = sbfs_mkdir, 
	.mknod = sbfs_mknod,
	//.rmdir = sbfs_rmdir,
	.readdir = sbfs_readdir,
	//.unlink  = sbfs_unlink,
	.open = sbfs_open,
	//.opendir = sbfs_opendir,
	.read = sbfs_read,
	.write = sbfs_write,
	// .init = sbfs_init
};

int main(int argc, char *argv[]){

	if (argc < 3){
		printf("needs 2 args: path of device and path of mount point");
	}

	int stat;
	mkfs();
	stat = fuse_main(argc, argv, &sbfs_operations, NULL);

	return stat;
}

