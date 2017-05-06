// TODO initier FREE_MEMORY_TABLE_FILE_NAME avec des 1 et refactorer isIndexOccupied
// TODO refactoring FREE_MEMORY_TABLE par FAT:
// TODO trouver bon vocabulaire pour les bits FREE_MEMORY_TABLE_FILE_NAME, 0 = free, 1 = ?? occupy ? locked ? > Used
// TODO suppression des blocs, tester d'overwrite un block complet avec un bloc plus court, voir si reste trace de lancien bloc
// TODO Quand verifier si le block est libre ? Dans disk_writeBlock ou disk_write ?
// TODO refactoring : choisir entre addresse ou index > id
// TODO mettre une variable global plutot que free.._getNumberOfAvailBlock()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "freeMemoryTable.h"
#include "disk.h"
#include "file.h"
#include "inode.h"
#include "cmd.h"

int init()
{
  int ret = SUCCESS;
  //init file disk si n'existe pas
  if (access(DISK_FILE_NAME, F_OK) == -1)
  {
    if(createDiskFile() == 0){
      ret = ERROR;
    }
  }
  
  //init freeMemoryTable file
  if (access(FREE_MEMORY_TABLE_FILE_NAME, F_OK) == -1)
  {
    if(freeMemoryTable_CreateFile() == 0){
      ret = ERROR;
    }
  }

  ret += initFileFile();

  ret += createInodeFile();

  return ret;
}

int main(int argc, char** argv)
{
  /*createDiskFile();
  freeMemoryTable_CreateFile();*/
  if (init() != SUCCESS)
  {
    perror("Erreur dans l'initialisation du systeme\n");
    exit(EXIT_FAILURE);
  }  
 /* 
  file_create("/etc","");
  file_create("/etc/asd.txt","asdasdasd");
  file_create("/etc/qwe.txt","qweqweqweqweqweqweqweqweqweqweqweqweqweqweqweqweqwe");
  file_del("/etc/asd.txt");
  file_create("/etc/zxc.txt","zxczxczxc");
 
  file_read("/etc/qwe.txt"); 
  file_read("/etc/zxc.txt");
  }
*/

  // file_create("/etc","");
  // file_create("/etc/asd.txt","asdasdasd");
  // file_create("/etc/qwe.txt","qweqweqwe");
  // file_del("/etc/asd.txt");
  // file_create("/etc/zxc.txt","zxczxczxc");

  //TEST COMMANDES
  cmd_parseInputFile(argc, argv);

  //TEST disk_cleanBlock
/*  disk_write("0123456789abcdef0123456789ABCDEF0123456789abcdef");
  freeMemoryTable_unLockBlock(1);
   disk_cleanBlock(1); 
  disk_writeBlock("bonjour",  1);
*/

  // TEST ECRIRE SUR DISK
  /* disk_write("Hello My name is groot!"); */
  /* disk_write("Hi My name is groot!"); */
  /* disk_write("0123456789abcdef0123456789ABCDEF0123456789abcdef"); */
  /* freeMemoryTable_lockBlock(0); */
  /* freeMemoryTable_unLockBlock(1); */
  /* disk_writeBlock("0123456789ABCDEF",  1); */
  /* freeMemoryTable_lockBlock(3); */
  /* disk_write("AAAAAAAAAAAAAAA"); */
  /* disk_write("BBBBBBBBBBBBBBBb"); */
  /* freeMemoryTable_unLockBlock(0); */
  /* disk_write("CCCCCCCCCCCCCCCcc"); */
  /* int *usedBlocks = disk_write("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDd"); */
  /* printf("usedBlocks :%d %d\n", usedBlocks[0], usedBlocks[1]); */
  /* int availableMemory = freeMemoryTable_getNumberOfAvailableBlock(); */
  /* printf("mem: %d \n", availableMemory); */
  /* disk_writeBlock("0123456789abcdef",  0); */
  /* disk_writeBlock("0123456789ABCDEF",  1); */
  
  return ( EXIT_SUCCESS);
}
