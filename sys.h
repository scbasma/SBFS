#ifndef _SBFS_SYS_H
#define _SBFS_SYS_H
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int sys_mkdir(char *path, mode_t mode);

int sys_mknod(const char *path, mode_t mode, dev_t dev); //what's up with the dev_t?->kind of file? yes i think so

int sys_opendir(const char *path);

//struct dir_entry *sys_readdir(struct *dir_entries); //returns the next dir_entry in the dir_entry stream dir_entries

int sys_unlink(const char *path); //remove file if no links

int sys_open(const char *path, int flags, mode_t mode);

//close

int sys_getattr(const char *path, struct stat *statbuf);

size_t sys_read(uint64_t fd, void *buf, size_t count, off_t offset); //return number of bytes read from file from offset with descriptor fd, into buf

size_t sys_write(uint64_t fd, const char *buf, size_t count, off_t offset); //writes up to count bytes to the file referenced by the file descriptor fd from buf
//returns number of bytes written

//have to read the specified block in order to add stuff at specified place

#endif