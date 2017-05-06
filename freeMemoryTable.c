#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "freeMemoryTable.h"

#define DATA_TYPE char

FILE* freeMemoryTable_CreateFile()
{
  FILE* freeMemoryTableFile;
  freeMemoryTableFile = fopen(FREE_MEMORY_TABLE_FILE_NAME, "wb");
  if (freeMemoryTableFile == NULL) {
    perror("Erreur d'ouverture du fichier FREE_MEMORY_TABLE_FILE_NAME.");
    exit(EXIT_FAILURE);
  }

  // Initialise avec des 0
  DATA_TYPE *wData = calloc(NUM_BLOCKS, sizeof(DATA_TYPE));
  fwrite(wData, sizeof(DATA_TYPE), NUM_BLOCKS, freeMemoryTableFile);
  free(wData);
  fclose(freeMemoryTableFile);
  return freeMemoryTableFile;
}

int* freeMemoryTable_GetAvailableBlock(int* ioAvailableBlock, int iNeededBlockNumber)
{
  FILE* freeMemoryTableFile;
  freeMemoryTableFile = fopen(FREE_MEMORY_TABLE_FILE_NAME, "r");
  if (freeMemoryTableFile == NULL) {
    perror("Erreur d'ouverture du fichier FREE_MEMORY_TABLE_FILE_NAME.");
    exit(EXIT_FAILURE);
  }
  if( freeMemoryTable_GetAvailableBlock_catchErr(iNeededBlockNumber) != ERROR){
    DATA_TYPE isIndexOccupied;
    int i=0;
    int j=0;

    while(j<NUM_BLOCKS && i<iNeededBlockNumber)
    {
      fread(&isIndexOccupied, sizeof(DATA_TYPE), 1, freeMemoryTableFile );
      if(!isIndexOccupied){
        ioAvailableBlock[i] = j;
        i++;
      }
      j++;
    }
  }

  fclose(freeMemoryTableFile);
  return ioAvailableBlock;
}

int freeMemoryTable_GetAvailableBlock_catchErr(int iNeededBlockNumber)
{
  if( iNeededBlockNumber > freeMemoryTable_getNumberOfAvailableBlock()){
    printf("\nErreur, nombre de block memoire demandé supperieur au nombre de block disponible.\n");
    return ERROR;
  }
  return SUCCESS;
}

int freeMemoryTable_getNumberOfAvailableBlock()
{
  FILE* freeMemoryTableFile;
  freeMemoryTableFile = fopen(FREE_MEMORY_TABLE_FILE_NAME, "r");
  if (freeMemoryTableFile == NULL) {
    perror("Erreur d'ouverture du fichier FREE_MEMORY_TABLE_FILE_NAME.");
    exit(EXIT_FAILURE);
  }
  int numberOfAvailableBlock = 0;
  DATA_TYPE isIndexOccupied;
  for(int i=0;i<NUM_BLOCKS;i++){
    fread(&isIndexOccupied, sizeof(DATA_TYPE), 1, freeMemoryTableFile );
    if(!isIndexOccupied)
      numberOfAvailableBlock++;
  }
  fclose(freeMemoryTableFile);
  return numberOfAvailableBlock;
}

void freeMemoryTable_lockBlock(int blockAddress)
{
  FILE* freeMemoryTableFile;
  freeMemoryTableFile = fopen(FREE_MEMORY_TABLE_FILE_NAME, "r+");
  if (freeMemoryTableFile == NULL) {
    perror("Erreur d'ouverture du fichier FREE_MEMORY_TABLE_FILE_NAME.");
    exit(EXIT_FAILURE);
  }
  DATA_TYPE lockedBlock = 1;
  fseek(freeMemoryTableFile, blockAddress, SEEK_SET);
  fwrite(&lockedBlock, sizeof(DATA_TYPE), 1, freeMemoryTableFile);
  fclose(freeMemoryTableFile);
}


void freeMemoryTable_unLockBlock(int blockAddress)
{
  FILE* freeMemoryTableFile;
  freeMemoryTableFile = fopen(FREE_MEMORY_TABLE_FILE_NAME, "r+");
  if (freeMemoryTableFile == NULL) {
    perror("Erreur d'ouverture du fichier FREE_MEMORY_TABLE_FILE_NAME.");
    exit(EXIT_FAILURE);
  }
  DATA_TYPE unLockedBlock = 0;
  fseek(freeMemoryTableFile, blockAddress, SEEK_SET);
  fwrite(&unLockedBlock, sizeof(DATA_TYPE), 1, freeMemoryTableFile);
  fclose(freeMemoryTableFile);
}
