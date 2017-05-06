#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freeMemoryTable.h"
#include "config.h"
#include "inode.h"


int createInodeFile()
{
  FILE* inodeFile;
  if (access(INODE_FILE_NAME, F_OK) == -1){ //Premiere utilisation alors on va creer un fichier vide
    inodeFile = fopen(INODE_FILE_NAME, "wb");
    if (inodeFile == NULL){
      perror("Erreur d'ouverture du fichier Inode.");
      exit(EXIT_FAILURE);
    }
    fclose(inodeFile);
  }
  return SUCCESS;
}

Inode_t* inode_get(int iInodeId){
  FILE* inodeFile;
  inodeFile = fopen(INODE_FILE_NAME, "r");
  if (inodeFile == NULL) {
    perror("Erreur d'ouverture du fichier disque. \n");
    exit(EXIT_FAILURE);
  }

  fseek(inodeFile, iInodeId * sizeof(Inode_t), SEEK_SET);
  Inode_t* oInode = malloc(sizeof(Inode_t));
  fread(oInode, sizeof(Inode_t), 1, inodeFile );
  fclose(inodeFile);
  return oInode;

}


void inode_create(int* iUsedBlocks, int iInodeId, int iNumberOfUsedBlocks)
{
  Inode_t inode;
  inode.size = iNumberOfUsedBlocks ; 
  for(int i=0; i<iNumberOfUsedBlocks; ++i){
    if (i < NUMBER_OF_INODE_BLOCK){
      inode.block[i] = iUsedBlocks[i];
    } else {
      inode.singleIndirect.block[i - NUMBER_OF_INODE_BLOCK] = iUsedBlocks[i]; 
    }
  }
  FILE* inodeFile;
  inodeFile = fopen(INODE_FILE_NAME, "r+b");
  // gestion des erreurs
  if (inodeFile == NULL) {
    perror("Erreur d'ouverture du fichier disque. \n");
    exit(EXIT_FAILURE);
  }

  fseek(inodeFile, iInodeId * sizeof(Inode_t), SEEK_SET);
  fwrite(&inode, sizeof(Inode_t), 1, inodeFile);
  fclose(inodeFile);
}
