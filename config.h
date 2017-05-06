#define BLOCK_SIZE 16                                        // Nombre de Bytes contenus dans un bloc sur le "disque"
#define NUM_BLOCKS 32000                                    // Nombre de blocs dans le fichier "disque" (disk.dat)
#define DISK_SIZE BLOCK_SIZE * NUM_BLOCKS
#define DISK_FILE_NAME "disk.dat"
#define FREE_MEMORY_TABLE_FILE_NAME "free_memory_table.dat"
#define FILE_FILE_NAME "file.dat"

//Inodes
#define NUMBER_OF_INODE_BLOCK 8
#define NUMBER_OF_INDIRECT_BLOCK 8 
#define INODE_FILE_NAME "inode.dat"

#define COMMAND_MAX_SIZE 300
#define FILE_CONTENT_MAX_SIZE 256

#define ABSOLUTE_PATH_MAX_SIZE 40
#define ABSOLUTE_PATH_MAX_REPO 20
#define PATH_SEPARATION "/"

#define SUCCESS 0
#define ERROR  -1

#define DEBUG 0 
