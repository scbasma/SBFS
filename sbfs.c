 //#define _FILE_OFFSET_BITS 64
 #define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "sys.h"
#include "inode.h"
#include "sb_mkfs.h"
#include "dir.h"
#include "dbg.h"
#include "spblk.h"
#include "sbfs.h"

sbfs_sp_blk *sp_blk;


int sbfs_mknod(const char *pathname, mode_t mode, dev_t dev){
	sys_mknod(pathname, 1, mode);
	return 0;
};


int sbfs_mkdir(const char *path, mode_t mode){
	sys_mkdir(path, 2);
	return 0;
};

int sbfs_rmdir(const char *path){
	return 0;
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
		log_info("ENTRY NAME IN READDIR: %s", entry->name);
		log_info("ENTRY INODE_NUMBER IN READDIR: %d", entry->inode_number);
		if(entry->inode_number != 0){
			if(filler(buf, entry->name, NULL, 0) != 0){
				log_info("Memory error: buffer full");
				free(entry);
				return -ENOMEM;
			}else {
				log_info("SBFSSONDREentry with inode_number: %d added to filler in readdir", (int) entry->inode_number);
			}
		}
	}
	free(entry);
	return retstat;
}

int sbfs_chmod(const char *path, mode_t mode){
	sbfs_core_inode *inode = namei(path);

	if(inode == NULL){
		return -ENOENT;
	}

	inode->d_inode.perm = mode;
	iput(inode);

	return 0;



};

int sbfs_chown(const char *path, uid_t uid, gid_t gid){
	sbfs_core_inode *inode = namei(path);
	if(inode == NULL){
		return -ENOENT;
	}

	inode->d_inode.grp_id = gid;
	inode->d_inode.user_id = uid;
	iput(inode);
	return 0;
};

int sbfs_utimens(const char *path, const struct timespec tv[2]){
	sbfs_core_inode *inode = namei(path);
	if(inode == NULL){
		return -ENOENT;
	}
	inode->d_inode.a_time = tv[0].tv_sec;
	inode->d_inode.m_time = tv[1].tv_sec;
	iput(inode);
	return 0;
}


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

int sbfs_truncate(const char *path, off_t size){
	sbfs_core_inode *inode = namei(path);
	if(inode == NULL){
		return -ENOENT;
	}
	char *buf = malloc(size);
	sys_write(inode->i_nmbr, buf, size, 0);
 return 0;

}

int sbfs_access(const char *path, int mask){
	log_info("access called with path: %s and mask: %d", path, mask);
	log_info("X_OK %d", X_OK);
	sbfs_core_inode *inode = namei(path);
	log_info("inode nmbr: %d", inode->i_nmbr);
	log_info("inode permissions: %d", inode->d_inode.perm);
	if(inode == NULL){
		return -ENOENT;
	}
	mode_t mode = inode->d_inode.perm;
	log_info("permissions of inode %d", inode->d_inode.perm);
	if(mask & F_OK){
		return 0;
	} 
	else if( (mask & R_OK) && (mode & S_IREAD) ){
		return 0;
	}
	else if ( (mask & W_OK ) && (mode & S_IWRITE) ){
		return 0;
	}
	else if( (mask & X_OK) && (mode & S_IEXEC)){
		return 0;
	}

	return -1;
}

int sbfs_write(const char *pathname, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int count = sys_write(fi->fh, buf, size, offset);
	return count;
};


int sbfs_getattr(const char *path, struct stat *stbuf){
	log_info("getattr called!!");
	int res = 0;
	sbfs_core_inode *inode = namei(path);
	memset(stbuf, 0, sizeof(struct stat));
	if(inode == NULL){
		return -ENOENT;
	}

	if(inode->i_nmbr == 0){
		free(inode);
		return -ENOENT;
	}
	log_info("inode nmbr: %d", inode->i_nmbr);
	stbuf->st_size = inode->d_inode.size;
	stbuf->st_ino = inode->i_nmbr;
	stbuf->st_gid = inode->d_inode.grp_id;
	stbuf->st_uid = inode->d_inode.user_id;
	if(inode->d_inode.type == 1){
		stbuf->st_mode = S_IFREG | 0777;
	}
	else if(inode->d_inode.type == 2){
		stbuf->st_mode = S_IFDIR | 0777;
	}
	stbuf->st_nlink = 2;
	stbuf->st_atime = inode->d_inode.a_time;
	stbuf->st_mtime = inode->d_inode.m_time;
	stbuf->st_ctime = inode->d_inode.c_time;
	free(inode);
	return res;
}


int sbfs_release(const char *path, struct fuse_file_info *fi){
	sbfs_core_inode *c_inode = namei(path);

	if(c_inode == NULL){
		return -ENOENT;
	}

	iput(c_inode);
	// free(c_inode);

	return 0;
}

void *sbfs_init(struct fuse_conn_info *conn){
	return (FILE*)fuse_get_context()->private_data;
}


int load_disk(char* disk_path)
{
    if ((disk = open(disk_path, O_RDWR)) == -1) {
        fprintf(stderr, "Disk loaded error!\n");
        return -1;
    }

    fprintf(stderr, "Disk loaded!\n");
    return 0;
}

void sbfs_destroy(void *user_data){
	close(disk);
}



struct fuse_operations sbfs_operations = {
	.getattr = sbfs_getattr,
	.mkdir = sbfs_mkdir, 
	.mknod = sbfs_mknod,
	.truncate = sbfs_truncate,
	.rmdir = sbfs_rmdir,
	.readdir = sbfs_readdir,
	.unlink  = sbfs_unlink,
	.open = sbfs_open,
	.opendir = sbfs_opendir,
	.read = sbfs_read,
	.write = sbfs_write,
	.utimens = sbfs_utimens,
	.access = sbfs_access,
	.release = sbfs_release,
	.chmod = sbfs_chmod,
	.chown = sbfs_chown,
	.init = sbfs_init,
	.destroy = sbfs_destroy
	// .init = sbfs_init
};

int main(int argc, char *argv[]){

	if ((argc < 3)||(argv[argc-2][0] == '-') || (argv[argc-1][0] == '-')) {
	    fprintf(stderr, "Last two argument must be path of device and path of mount point\n");
	    return -1;
	}


	char* disk_path = realpath(argv[argc-2], NULL);
	log_info("disk_path: %s",disk_path);
	if (!disk_path) {
	    fprintf(stderr, "Disk path is not correct!\n");
	    return -1;
	}
	fprintf(stderr, "Disk path: %s\n", disk_path);
	log_info("argv[argc-2]: %s", argv[argc-2]);
	log_info("argv[argc-1]: %s", argv[argc-1]);
	argv[argc-2] = argv[argc-1];
	argv[argc-1] = NULL;
	argc--;
	
	log_info("argv[argc-2] after: %s", argv[argc-2]);
	log_info("argc: %d", argc);

	if (load_disk(disk_path) == -1){
			fprintf(stderr, "load disk failed");
	    return -1;
	 }

	 get_super();
	 log_info("magic_number: %d", sp_blk->magic_number);

	int stat;

	umask(0);
	stat = fuse_main(argc, argv, &sbfs_operations, NULL);

	return stat;
}

