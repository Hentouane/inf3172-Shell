#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "util.h"

int parsePath(char* iPath, char oParsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE])
{
  int i = 0;
  char* wPathCopy = strdup(iPath); //Pour pouvoir la modifier avec strtok
  char* token;
  token = strtok(wPathCopy, "/");
  while (token != NULL){
    strcpy(oParsedPath[i],token);
    token = strtok(NULL, PATH_SEPARATION);
    i++;
  }
  strcpy(oParsedPath[i],"");
  free(wPathCopy);
  return SUCCESS;
}


