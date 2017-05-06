#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "file.h"
#include "util.h"
#include "inode.h"
#include "disk.h"

int initFileFile()
{
  FILE* fileFile;
  if (access(FILE_FILE_NAME, F_OK) == -1){ //Premiere utilisation alors on va creer un fichier vide
    fileFile = fopen(FILE_FILE_NAME, "wb");
    if (fileFile == NULL){
        perror("Erreur d'ouverture du fichier file.dat");
        exit(EXIT_FAILURE);
    }
    fclose(fileFile);
  }
  return SUCCESS;
}

int file_createFile(char* iPath, char* iContent)
{
  File_t file;
  int ret = ERROR;
  if (file_createFile_catchErr(iPath,iContent) != ERROR){
    strcpy(file.absolutePath, iPath);
    char parsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE];
    if (parsePath(iPath, parsedPath) == SUCCESS){
      file.type = File;
      if (file_hasParent(iPath,parsedPath) >= 0){
        file_write(&file, iContent);
        ret = SUCCESS;
      } else {
        printf("\nLe fichier (%s)  n'a pas de parent\n", iPath);
      }
    }
  }
  return ret;
}

int file_createFile_catchErr(char* iPath, char* iContent)
{
  if (strlen(iPath) == 0){
    printf("\nLe chemin du fichier ne peut etre vide.\n");
    return ERROR;
  }
  if (strlen(iPath) > ABSOLUTE_PATH_MAX_SIZE){
    printf("\nLe chemin du fichier (%s) est trop long.\n", iPath);
    return ERROR;
  }
  if (strlen(iContent) == 0){
    printf("\nUn fichier (%s) ne peut pas avoir un contenu vide\n", iPath);
    return ERROR;
  }
  if (strlen(iContent) > FILE_CONTENT_MAX_SIZE){
    printf("\nLe contenu du fichier (%s) est trop grand.\n", iPath);
    return ERROR;
  }

  return SUCCESS;
}

int file_createRepo(char* iPath)
{
  File_t file;
  int ret = SUCCESS;
  if (file_createRepo_catchErr(iPath) != ERROR){
    strcpy(file.absolutePath, iPath);
    char parsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE];
    if (parsePath(iPath, parsedPath) == SUCCESS){
      file.type = Directory;
      if (file_hasParent(iPath,parsedPath) >= 0){
        file_write(&file, "");
      } else {
        printf("\nLe repertoire (%s)  n'a pas de parent\n", iPath);
        ret = ERROR;
      }
    }
  }
  return ret;
}

int file_createRepo_catchErr(char* iPath)
{
  if (strlen(iPath) == 0){
    printf("\nLe chemin du repertoire ne peut etre vide.\n");
    return ERROR;
  }
  if (strlen(iPath) > ABSOLUTE_PATH_MAX_SIZE){
    printf("\nLe chemin du repertoire (%s) est trop long.\n", iPath);
    return ERROR;
  }
  return SUCCESS;
}

//Pour supprimer un fichier on change son type a ToRemove 
void file_del(char *iPath)
{
  File_t* tempFile = file_get(iPath);
  if (tempFile != 0){
    FILE* fileFile = fopen(FILE_FILE_NAME, "r+b");
    
    if (tempFile->type == File){
      //Libere l'espace du disque
      Inode_t* inode = inode_get(tempFile->id);
      if (inode->size < NUMBER_OF_INODE_BLOCK){
        disk_deleteBlocks(inode->block, inode->size);
      } else {
        int indSize = inode->size - NUMBER_OF_INODE_BLOCK;
        disk_deleteBlocks(inode->block, inode->size - indSize);
        disk_deleteBlocks(inode->singleIndirect.block, indSize);
      }
      free(inode);
    }

    tempFile->type = ToRemove;
    fseek(fileFile, tempFile->id * sizeof(File_t), SEEK_SET);
    fwrite(tempFile, sizeof (File_t), 1, fileFile);

    fclose(fileFile);

    free(tempFile);
  } else {
    printf("\n%s n'existe pas et ne peut pas etre supprimer\n", iPath);
  }
}

void file_delAll(char *iPath)
{
  int pos = file_find(iPath);
  if (pos >= 0){
    if (DEBUG) printf("\nOn delete %s \n", iPath); 
    file_delChildren(iPath);
  } else {
    printf("\n%s n'existe pas et ne peut pas etre supprimer\n", iPath);
  }
}

int file_delChildren(char* iPath)
{
  int count = 0;
  File_t* parent = file_get(iPath);

  char parentPath[ABSOLUTE_PATH_MAX_SIZE];
  strcpy(parentPath, iPath);
  strcat(parentPath, PATH_SEPARATION);

  if (parent != 0)
  {
    if (parent->type == Directory)
    {
      File_t tempFile;
      FILE* fileFile = fopen(FILE_FILE_NAME, "r+b");
      fseek(fileFile, 0, SEEK_SET);
      do{
        if (fread(&tempFile, sizeof(File_t),1, fileFile))
        {
          if (strstr(tempFile.absolutePath, parentPath) != 0 && tempFile.type != ToRemove)
          {
            file_delAll(tempFile.absolutePath);
            count++;
          }
        } else {
          perror("\nErreur dans la lecture du fichier de fichiers \n");
          exit(EXIT_FAILURE);
        }
     } while (!feof(fileFile));
     fclose(fileFile);
    }
    free(parent);
    file_del(iPath);
  }
  return count;
}

// Retourne la position dans le fichier de file
int file_find(char* iPath)
{
  File_t tempFile;
  int i = 0;
  FILE* fileFile = fopen(FILE_FILE_NAME, "rb");

  do
  {
    if (fread(&tempFile, sizeof (File_t),1, fileFile) && strcmp(tempFile.absolutePath, iPath) == 0 && tempFile.type != ToRemove){
      fclose(fileFile);
      return i;
    }
    i++;
  } while (!feof(fileFile));

  fclose(fileFile);

  return -1;
}

File_t* file_get(char* iPath)
{
  File_t* file = 0;
  int pos = file_find(iPath);
  if (pos >= 0){
    FILE* fileFile;
    fileFile = fopen(FILE_FILE_NAME, "rb");
    // Gestion des erreurs
    if (fileFile == NULL) {
      perror("Erreur d'ouverture du fichier de fichiers.");
      exit(EXIT_FAILURE);
    }
    fseek(fileFile, pos*sizeof(File_t), SEEK_SET);
    file = malloc(sizeof(File_t));
    fread(file, sizeof(File_t), 1, fileFile );
    
    fclose(fileFile);
  }
  return file;
}

int file_hasParent(char* iPath, char iParsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE])
{
  int depth = 0;
  while(strcmp(iParsedPath[depth],"") != 0)
    depth++;
  if (depth == 1) //root
  {
    return SUCCESS;
  } else 
  {
    size_t parentPathLength = strlen(iPath) - strlen(iParsedPath[depth-1]) + 1;
    char parentPath[parentPathLength];
    memset(parentPath, '\0', sizeof(parentPath));
    strncpy(parentPath,iPath, parentPathLength - 2); 

    return file_find(parentPath);
  }
}

void file_read(char *iPath)
{
  if(DEBUG) printf("file_read : ipath=%s \n", iPath);
  File_t* file = file_get(iPath);
  if (file != 0){
    if (file->type != File) //Seul un file peut contenir du texte
    {
      printf("\nErreur, le fichier est de mauvais type\n ");
      free(file);
      return;
    }

    Inode_t* inode = inode_get(file->id);
    for(int nbBlock = 0; nbBlock < inode->size; nbBlock++)
    {
      if (nbBlock < NUMBER_OF_INODE_BLOCK){
        disk_readBlock(inode->block[nbBlock]);
      } else {
        disk_readBlock(inode->singleIndirect.block[nbBlock - NUMBER_OF_INODE_BLOCK]);
      }
    }
    printf("\n");
    free(inode);
    free(file);
  }
}

int file_write(File_t* iFile, char* iContent)
{
  File_t tempFile;
  int id = 0;
  int ret = 0;
  if (file_find(iFile->absolutePath) < 0){
    FILE* fileFile = fopen(FILE_FILE_NAME, "r+b");
    do
    {
      if (!fread(&tempFile, sizeof (File_t),1, fileFile)){
        break;
      } else if (tempFile.type == ToRemove){
        break;
      }
      id++ ;
    } while (!feof(fileFile));

    iFile->id = id;

    fseek(fileFile, id * sizeof(File_t), SEEK_SET);
    fwrite(iFile, sizeof (File_t), 1, fileFile);
    fclose(fileFile);

    //Si c'est un fichier, on creer un inode
    if(iFile->type == File){
      int dataSize = (int)strlen(iContent);
      int neededBlockNumber = (dataSize-1) / BLOCK_SIZE + 1;
      if(DEBUG) printf("neededBlockNumber = %d \n", neededBlockNumber);
      int usedBlocks[neededBlockNumber];
      disk_write(iContent, usedBlocks);
      inode_create(usedBlocks, id, neededBlockNumber);
    }
    ret = SUCCESS;

  } else {
    printf("\nLe fichier (%s) existe deja!\n", iFile->absolutePath);
    ret = ERROR;
  }
  return ret; 
}
