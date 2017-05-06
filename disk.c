#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freeMemoryTable.h"
#include "config.h"
#include "disk.h"

void disk_write(char* iDataToWrite, int* oUsedBlocks)
{
  FILE* diskFile;

  int dataSize = (int)strlen(iDataToWrite);
  int neededBlockNumber = (dataSize-1) / BLOCK_SIZE + 1;
  int numberOfAvailableBlocks = freeMemoryTable_getNumberOfAvailableBlock();
  diskFile = fopen(DISK_FILE_NAME, "r+");
  
  if (disk_write_catchErr(dataSize, iDataToWrite, diskFile, numberOfAvailableBlocks, neededBlockNumber) != ERROR){
    freeMemoryTable_GetAvailableBlock(oUsedBlocks, neededBlockNumber);

    for(int i=0; i < neededBlockNumber; ++i ){
      char* blockString = malloc(sizeof(char)*BLOCK_SIZE+1);// +1 pour le \0 ( end of string)
      blockString[BLOCK_SIZE] = '\0';
      strncpy(blockString, iDataToWrite, BLOCK_SIZE);
      if(DEBUG) printf("---- usedBlock[%d]=%d \n", i, oUsedBlocks[i]);
      disk_writeBlock(blockString, oUsedBlocks[i]);
      free(blockString);
      iDataToWrite = iDataToWrite + (BLOCK_SIZE) * sizeof(char);
    }
  }
  fclose(diskFile);
}

int disk_write_catchErr( int dataSize, char* iDataToWrite, FILE* diskFile, int numberOfAvailableBlocks, int neededBlockNumber)
{
  if(DEBUG) printf("disk_write:  iDataToWrite=%s\n", iDataToWrite);
  if (diskFile == NULL) {
    perror("Erreur d'ouverture du fichier disque. \n");
    exit(EXIT_FAILURE);
  }
  // On verifie qu'on dispose assez d<espace sur le disque
  if ( numberOfAvailableBlocks < neededBlockNumber ){
    printf("Seulement %d blocs disponibles pour un fichier de taille %d\n chars", numberOfAvailableBlocks, dataSize);
    return ERROR;
  }
  if ( dataSize < 1 ){
    printf("Ne peut pas ecrire une string de taille 0.");
    return ERROR;
  }
  return SUCCESS;
}
void disk_writeBlock(char* iDataToWrite, int iBlockId )
{
  int dataSize = strlen(iDataToWrite) ;
  if(DEBUG) printf("disk_writeBlock iBlockId=%d \n", iBlockId);
  FILE* diskFile;
  diskFile = fopen(DISK_FILE_NAME, "r+");
  // Gestion des erreurs
  if (diskFile == NULL) {
    perror("Erreur d'ouverture du fichier disque. \n");
    exit(EXIT_FAILURE);
  }

  if(dataSize < BLOCK_SIZE)
    disk_cleanBlock(iBlockId);
  fseek(diskFile, (iBlockId*BLOCK_SIZE), SEEK_SET);
  fwrite(iDataToWrite, sizeof ( char), dataSize, diskFile);
  fclose(diskFile);
  freeMemoryTable_lockBlock(iBlockId);

}
void disk_deleteBlocks(int *blocksToDelete, int iNumberOfBlocksToDelete)
{
  for(int i=0; i<iNumberOfBlocksToDelete; ++i){
    disk_deleteBlock(blocksToDelete[i]);
  }
}

void disk_cleanBlock(int iBlockId)
{
  FILE* diskFile;
  diskFile = fopen(DISK_FILE_NAME, "r+");
  // Gestion des erreurs
  if (diskFile == NULL) {
    perror("Erreur d'ouverture du fichier disque.");
    exit(EXIT_FAILURE);
  }
  char* resetToZero = calloc(BLOCK_SIZE, sizeof(char));
  fseek(diskFile, (iBlockId*BLOCK_SIZE), SEEK_SET);
  fwrite(resetToZero, sizeof(char), BLOCK_SIZE, diskFile);
  free(resetToZero);
  fclose(diskFile);
}

void disk_deleteBlock(int iBlockAddress )
{
  freeMemoryTable_unLockBlock(iBlockAddress);
}


void disk_readBlock(int iBlockAddress)
{
  FILE* diskFile;
  diskFile = fopen(DISK_FILE_NAME, "r");
  // Gestion des erreurs
  if (diskFile == NULL) {
    perror("Erreur d'ouverture du fichier disque.");
    exit(EXIT_FAILURE);
  }
  fseek(diskFile, (iBlockAddress*BLOCK_SIZE), SEEK_SET);
  char output[BLOCK_SIZE+1];
  fread(&output, sizeof(char), BLOCK_SIZE, diskFile );
  output[BLOCK_SIZE] = '\0';
  printf("%s", output);
  fclose(diskFile);
}

FILE* createDiskFile()
{
  FILE* diskFile;

  diskFile = fopen(DISK_FILE_NAME, "wb");

  if (diskFile == NULL) {
      perror("Erreur d'ouverture du fichier disque.");
      exit(EXIT_FAILURE);
  }

  // Initialise avec des NULL
  char *wData = calloc(DISK_SIZE, sizeof ( char));
  fwrite(wData, sizeof ( char), DISK_SIZE, diskFile);
  free(wData);
  fclose(diskFile);
  return diskFile;
}
