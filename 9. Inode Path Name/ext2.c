#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/fs.h>
#include <ext2fs/ext2_fs.h>
#define DEVICE_FILE_PATH "/dev/sda5"
#define MAX_PATH_TOKS    (256)

typedef uint32_t _u32;
typedef uint16_t _u16;
typedef uint8_t  _u8;
typedef uint64_t _u64;
typedef int64_t  _s64;

#define exit_err(...)                           \
    {                                           \
        fprintf(stderr, __VA_ARGS__);           \
        exit(EXIT_FAILURE);                     \
    }
#define INVALID_REQUEST  (2)
#define INODE_REQUEST    (0)

#define DATA_REQUEST     (1)

static inline _u32 GETTOKENS(_u8 *ARGS, _u8 *delimiter, _u8 *tokens[MAX_PATH_TOKS]) {
    _u8 *token;
    _u32 token_i = 0;
    while (token = strtok_r(ARGS, delimiter, (char **)&ARGS))
        tokens[token_i++] = strdup(token);
    return token_i;
}

static inline _u8 GETREQUESTTYPE(_u8 *ARGS) {
    if (!strcmp(ARGS, "inode"))
        return INODE_REQUEST;
    else if (!strcmp(ARGS, "data")) 
        return DATA_REQUEST;
    else
        return INVALID_REQUEST;
}

#define EXT2_SUPER_BLOCK_OFFSET    (1024u)
#define EXT2_SUPER_BLOCK_SIZE      (sizeof(struct ext2_super_block))
#define EXT2_DIR_ENTRY_SIZE        (sizeof(struct ext2_dir_entry_2))
#define EXT2_IS_INODE_DIR(ino_st)  ((((ino_st)->i_mode) & 0xF000) == (0x4000))
#define EXT2_IS_INODE_REG_FILE(ino_st)  ((((ino_st)->i_mode) & 0xF000) == (0x8000))
#define EXT2_PHY_TO_LOG_BLKS(sup, phy_nb)                           \
    ({                                                              \
        uint64_t log_to_phy_fact;                                   \
        uint64_t log_nb;                                            \
        log_to_phy_fact = EXT2_BLOCK_SIZE((sup)) / (512u);          \
        log_nb = ((phy_nb) / log_to_phy_fact);                      \
        log_nb += (((phy_nb) % log_to_phy_fact) != 0);              \
    })

char *_ft_to_str[EXT2_FT_MAX] = {"Unknown  ", "Regular  ", "Directory",
                                 "Character", "Block    ", "Fifo     ",
                                 "Socket   ", "Softlink "};

static struct ext2_super_block _sb;
static _u32 F;
static inline void EXT2_READ(_u64 OFFSET, void *BUFFER, _u64 SIZE) {
    lseek64(F, OFFSET, SEEK_SET);
    read(F, BUFFER, SIZE);
}

static inline void EXT_SEEK_SET(_u64 OFFSET) {
    lseek64(F, OFFSET, SEEK_SET);
}


static inline void SEEK_CURRENT(_s64 OFFSET) {
    lseek64(F, OFFSET, SEEK_CUR);
}


void INIT() {
    F = open(DEVICE_FILE_PATH, O_RDONLY);
    if (F == -1)
        exit_err("Failed to open the device file\n");
    EXT2_READ(EXT2_SUPER_BLOCK_OFFSET, &_sb, EXT2_SUPER_BLOCK_SIZE);
}

void DEINIT() {
    close(F);
}

static _u64 _EXT2_DIRECT_SEARCH(_u32 block_address, _u8 *next_argument) {
    struct ext2_dir_entry_2 dir_ent;
    _u64 block_offset;
    _u32 i = 0;
    block_offset = (_u64)block_address * EXT2_BLOCK_SIZE(&_sb);
    while (i < EXT2_BLOCK_SIZE(&_sb)) {
        EXT2_READ(block_offset + i, &dir_ent, EXT2_DIR_ENTRY_SIZE);
        if (!memcmp(next_argument, dir_ent.name, dir_ent.name_len))
            return dir_ent.inode;
        i += dir_ent.rec_len;
    }
    return EXT2_BAD_INO;
}

static void _EXT2_INODE_TO_INODE_ST(_u64 inode, struct ext2_inode *p) {
    _u64 GRP_NM;
    _u64 GRP_DESC_OFFSET;
    struct ext2_group_desc grp_desc;
    _u64 inode_table_offset;
    _u64 inode_idx;
    _u64 inode_off;
    GRP_NM = (inode - 1) / EXT2_INODES_PER_GROUP(&_sb);
    GRP_DESC_OFFSET = EXT2_BLOCK_SIZE(&_sb) + GRP_NM * EXT2_DESC_SIZE(&_sb);
    EXT2_READ(GRP_DESC_OFFSET, &grp_desc, EXT2_DESC_SIZE(&_sb));
    inode_table_offset = (_u64)grp_desc.bg_inode_table * EXT2_BLOCK_SIZE(&_sb);
    inode_idx = (inode - 1) % EXT2_INODES_PER_GROUP(&_sb);
    inode_off = inode_table_offset + inode_idx * EXT2_INODE_SIZE(&_sb);
    EXT2_READ(inode_off, p, sizeof(struct ext2_inode));
}


static _u64 _EXT2_INDIRECT_SEARCH(_u32 block_address, _u8 *next_argument, _u8 indirect_level) {
    _u64 block_offset;
    _u32 next_block_address;
    _u64 next_inode;
    _u32 i = 0;
    block_offset = (_u64)block_address * EXT2_BLOCK_SIZE(&_sb);
    while (i < EXT2_ADDR_PER_BLOCK(&_sb)) {
        EXT2_READ(block_offset + 4 * i, &next_block_address, 4);
        if (!next_block_address) 
            return EXT2_BAD_INO;
        if (indirect_level == 1)
            next_inode = _EXT2_DIRECT_SEARCH(next_block_address, next_argument);
        else
            next_inode = _EXT2_INDIRECT_SEARCH(next_block_address, next_argument, indirect_level - 1);
        if (next_inode > EXT2_BAD_INO)
            return next_inode;
        i++;
    }
    return EXT2_BAD_INO;
}

static _u64 _EXT2_NEXT_INODE(_u64 inode, _u8 *next_argument) {
    struct ext2_inode inode_st;
    _u64 next_inode;
    _u32 block_address;
    _u32 i = 0;
    _EXT2_INODE_TO_INODE_ST(inode, &inode_st);
    if (!EXT2_IS_INODE_DIR(&inode_st))
        exit_err("The path consists of non-directory files\n");
    while ((i < EXT2_N_BLOCKS) && (block_address = inode_st.i_block[i])) {
        if (i < EXT2_NDIR_BLOCKS) 
            next_inode = _EXT2_DIRECT_SEARCH(block_address, next_argument);
        else if (i == EXT2_IND_BLOCK) 
            next_inode = _EXT2_INDIRECT_SEARCH(block_address, next_argument, 1);
        else if (i == EXT2_DIND_BLOCK) 
            next_inode = _EXT2_INDIRECT_SEARCH(block_address, next_argument, 2);
        else if (i == EXT2_TIND_BLOCK) 
            next_inode = _EXT2_INDIRECT_SEARCH(block_address, next_argument, 3);
        if (next_inode > EXT2_BAD_INO) 
            return next_inode;
        i++;
    }
    return EXT2_BAD_INO;
}

void _EXT2_PRINT_INODE_st(_u64 inode) {
    _u32 i = 0;
    struct ext2_inode inode_st;
    _EXT2_INODE_TO_INODE_ST(inode, &inode_st);
    printf("Inode: %u ", inode);
    printf("Type: 0x%x ", inode_st.i_mode & 0xF000);
    printf("Mode: 0%o ", inode_st.i_mode & 0x0FFF);
    printf("Flags: 0x%x\n", inode_st.i_flags);
    printf("Generation: %u\n", inode_st.i_generation);
    printf("User: %u ", inode_st.i_uid);
    printf("Group: %u ", inode_st.i_gid);
    printf("Size: %u\n", inode_st.i_size);
    printf("File ACL: %u\n", inode_st.i_file_acl);
    printf("Links: %u ", inode_st.i_links_count);
    printf("Blockcount: %u\n", inode_st.i_blocks);
    printf("ctime: 0x%x\n", inode_st.i_ctime);
    printf("atime: 0x%x\n", inode_st.i_atime);
    printf("mtime: 0x%x\n", inode_st.i_mtime);
    printf("BLOCKS:\n");
    while ((i < EXT2_N_BLOCKS) && (inode_st.i_block[i])) {
        if (i < EXT2_NDIR_BLOCKS)
            printf("Direct data block (%u): %u\n", i, inode_st.i_block[i]);
        else if (i == EXT2_IND_BLOCK) 
            printf("Single indirect data block: %u\n", inode_st.i_block[i]);
        else if (i == EXT2_DIND_BLOCK) 
            printf("Double indirect data block: %u\n", inode_st.i_block[i]);
        else if (i == EXT2_TIND_BLOCK)
            printf("Triple indirect data block: %u\n", inode_st.i_block[i]);
        i++;
    }
}
_u64 _EXT2_PATH_TO_INODE(_u8 *PATH){
    _u8 *tokens[MAX_PATH_TOKS];
    _u32 nb_tokens;
    _u32 i;
    _u64 inode;
    nb_tokens = GETTOKENS(PATH, "/", tokens);
    inode = EXT2_ROOT_INO;
    for (i = 0; i < nb_tokens; i++) {
        inode = _EXT2_NEXT_INODE(inode, tokens[i]);
	printf("%d",inode);
        printf("%d",EXT2_ROOT_INO);
        if (inode < EXT2_ROOT_INO) 
            exit_err("File search failed\n");
    }
    for (i = 0; i < nb_tokens; i++) 
        free(tokens[i]);
    return inode;
}



void _EXT2_DIRECT_PRINT_dir(_u32 block_address) {
    _u64 block_offset;
    _u32 i = 0;
    struct ext2_dir_entry_2 dir_ent;
    block_offset = (_u64)block_address * EXT2_BLOCK_SIZE(&_sb);
    while (i < EXT2_BLOCK_SIZE(&_sb)) {
        EXT2_READ(block_offset + i, &dir_ent, EXT2_DIR_ENTRY_SIZE);
        printf("%d\t", dir_ent.inode);
        printf("%s\t", _ft_to_str[dir_ent.file_type]);
        printf("%.*s\n", dir_ent.name_len, dir_ent.name);
        i += dir_ent.rec_len;
    }
}

 
void _EXT2_DIRECT_PRINT_reg_file(_u32 block_address) {
    _u64 block_offset;
    _u32 i = 0;
    _u8 DATA;
    block_offset = (_u64)block_address * EXT2_BLOCK_SIZE(&_sb);
    while (i < EXT2_BLOCK_SIZE(&_sb)) {
        EXT2_READ(block_offset + i, &DATA, 1);
        printf("%c", DATA);
        i++;
    }
}


void _EXT2_DIRECT_PRINT(_u32 blk_addr, _u8 file_type) {
    if (file_type == EXT2_FT_REG_FILE) 
        _EXT2_DIRECT_PRINT_reg_file(blk_addr);
    else if (file_type == EXT2_FT_DIR) 
        _EXT2_DIRECT_PRINT_dir(blk_addr);
}


void _EXT2_INDIRECT_PRINT(_u32 block_address, _u8 ft, _u8 indir_level) {
    _u64 block_offset;
    _u32 nba;
    _u32 i = 0;
    block_offset = (_u64)block_address * EXT2_BLOCK_SIZE(&_sb);
    while (i < EXT2_ADDR_PER_BLOCK(&_sb)) {
        EXT2_READ(block_offset + 4 * i, &nba, 4);
        if (!nba) 
            return;
        if (indir_level == 1) 
            _EXT2_DIRECT_PRINT(nba, ft);
        else 
            _EXT2_INDIRECT_PRINT(nba, ft, indir_level - 1);
        i++;
    }
}


void _EXT2_PRINT_INODE_DATA(_u64 inode) {
    struct ext2_inode inode_st;
    _u8 ft;
    _u32 i = 0;
    _u32 block_address;
    _EXT2_INODE_TO_INODE_ST(inode, &inode_st);
    if (EXT2_IS_INODE_REG_FILE(&inode_st))
        ft = EXT2_FT_REG_FILE;
    else if (EXT2_IS_INODE_DIR(&inode_st))
        ft = EXT2_FT_DIR;
    else
        exit_err("File type not supported\n");
 
    while ((i < EXT2_N_BLOCKS) && (block_address = inode_st.i_block[i])) {
        if (i < EXT2_NDIR_BLOCKS)
            _EXT2_DIRECT_PRINT(block_address, ft);
        else if (i == EXT2_IND_BLOCK) 
            _EXT2_INDIRECT_PRINT(block_address, ft, 1);
        else if (i == EXT2_DIND_BLOCK)
            _EXT2_INDIRECT_PRINT(block_address, ft, 2);
        else if (i == EXT2_TIND_BLOCK)
            _EXT2_INDIRECT_PRINT(block_address, ft, 3);
        i++;
    }
}

void EXT2_PRINT_INODE(_u64 inode, _u8 request) {
    if (request == INODE_REQUEST)
        _EXT2_PRINT_INODE_st(inode);
    else if (request == DATA_REQUEST)
        _EXT2_PRINT_INODE_DATA(inode);
    else 
        exit_err("Invalid request\n");
}


int main(int argc, char *argv[]) {
    _u8 request;
    _u64 inode;
   if (argc != 3)
        exit_err("Invalid number of arguments given\n");
    INIT();
    inode = _EXT2_PATH_TO_INODE(argv[1]);
    request = GETREQUESTTYPE(argv[2]);
    EXT2_PRINT_INODE(inode, request);
    DEINIT();
    exit(EXIT_SUCCESS);
}
