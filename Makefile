CC = gcc
CFLAGS = -g -Werror -Wall -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -g
PROG = sbfs
MKFS = sb_mkfs

SRCS = sbfs.c sys.c disk_op.c datablock.c inode.c spblk.c bitmap.c dir.c 
SRCS_MKFS = sb_mkfs.c disk_op.c spblk.c inode.c datablock.c sys.c bitmap.c dir.c


all: $(PROG) $(MKFS) $(TDISK)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) $(SRCS) `pkg-config fuse --cflags --libs` -o $(PROG)

$(MKFS):	$(SRCS_MKFS)
	$(CC) $(CFLAGS) $(SRCS_MKFS) -o $(MKFS)

clean:
	rm -f $(PROG) $(MKFS)

