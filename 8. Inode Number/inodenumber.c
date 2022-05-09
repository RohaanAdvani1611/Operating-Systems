#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/fs.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<ext2fs/ext2_fs.h>


void main(int argc, char *argv[]){

    int INODE_NUMBER = atoi(argv[2]), F;
    int BLOCK_size;
    int  INODE_LOCATION, GROUP_NO, TOTAL_GROUPS , INODE_OFFSET , GROUP_DESCRIPTOR_OFFSET;
    
    F = open(argv[1], O_RDONLY);
    if (F < 0){
        perror("Error in there in opening the input file :");
        exit(errno);
    }

   
    
    struct ext2_group_desc group_descriptor_list[4096];
     struct ext2_super_block SUPER_BLOCK;
     struct ext2_inode node;

    lseek(F, 1024, SEEK_SET);
    read(F, &SUPER_BLOCK, sizeof(struct ext2_super_block));

    if(SUPER_BLOCK.s_magic != EXT2_SUPER_MAGIC){
        fprintf(stderr, "Not an Ext2 filesystem!\n");
        exit(1);
    }

    BLOCK_size = 1024 << SUPER_BLOCK.s_log_block_size;
    TOTAL_GROUPS = SUPER_BLOCK.s_blocks_count / SUPER_BLOCK.s_blocks_per_group;
    //unsigned int descr_list_size = TOTAL_GROUPS * sizeof(struct ext2_group_desc);
    
    if(BLOCK_size == 1024){
        GROUP_DESCRIPTOR_OFFSET = 1024 + BLOCK_size;
    }
    else{
        GROUP_DESCRIPTOR_OFFSET = BLOCK_size;
    }

    lseek(F, GROUP_DESCRIPTOR_OFFSET, SEEK_SET);
    read(F, group_descriptor_list, sizeof(struct ext2_group_desc) * TOTAL_GROUPS);

    GROUP_NO = INODE_NUMBER / SUPER_BLOCK.s_inodes_per_group;
    INODE_LOCATION = INODE_NUMBER - (GROUP_NO * SUPER_BLOCK.s_inodes_per_group) - 1;
    INODE_OFFSET = 1024 + (group_descriptor_list[GROUP_NO].bg_inode_table - 1)*BLOCK_size;
    INODE_OFFSET += INODE_LOCATION * sizeof(struct ext2_inode);



    lseek(F, INODE_OFFSET, SEEK_SET);
    read(F, &node, sizeof(struct ext2_inode));

    printf("MAGIC NUMBER : %x\n", SUPER_BLOCK.s_magic);
    printf("BLOCK SIZE : %d\n", BLOCK_size);
    printf("INODES PER GROUP : %d\n", SUPER_BLOCK.s_inodes_per_group);
    printf("BLOCKS PER GROUP : %d\n", SUPER_BLOCK.s_blocks_per_group);

    printf("INODE TABLE = %d\n", group_descriptor_list[GROUP_NO].bg_inode_table);
    printf("GROUP NO: %d\n", GROUP_NO);
    printf("INODE TABLE OFFSET = %d\n", INODE_OFFSET);

    printf(" MODE = %u\n UID = %u\n SIZE = %u\n ACCESS TIME = %u\n CREATION TIME = %u\n MODIFICATION TIME = %u\n DELETION TIME = %u\n LINK COUNT = %u\n BLOCK COUNT = %u\n FLAGS = %u\n", node.i_mode, node.i_uid, node.i_size, node.i_atime, node.i_ctime, node.i_mtime, node.i_dtime, node.i_links_count, node.i_blocks, node.i_flags);


    return ;

}
