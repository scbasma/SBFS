#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

size_t sys_write(uint64_t fd, const void *buf, size_t count, off_t offset){

	struct sbfs_inode inode = (struct sbfs_inode *) fd;

}