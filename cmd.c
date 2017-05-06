#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "cmd.h"
#include "file.h"

void cmd_parseInputFile(int argc, char **argv)
{
  if(argc != 2){
    perror("Vous devez entrez un et un seul parametre : un fichier text contenant les commandes à executer.");
    exit(EXIT_FAILURE);
  }
  char *commandFileName = argv[1];
  FILE* commandFile;
  commandFile = fopen(commandFileName, "r");
  // Gestion des erreurs
  if (commandFile == NULL) {
    perror("Erreur d'ouverture du fichier disque.");
    exit(EXIT_FAILURE);
  }
  char line[COMMAND_MAX_SIZE];
  while(fgets(line, sizeof(line), commandFile) != NULL){
    cmd_parseLine(line);
  }
  fclose(commandFile);
}

void cmd_parseLine(char* line){
  printf("Executing: %s \n", line);
  char cmd[40];
  char path[ABSOLUTE_PATH_MAX_SIZE];
  char content[FILE_CONTENT_MAX_SIZE * 4] = "";
  sscanf(line, "%s %s %[^\n]",cmd, path, content); // lit 2eme et 3eme mot de la ligne
  cmd_execute(cmd, path, content);

}


void cmd_execute(char *cmd, char* path, char* content)
{
  if(cmd_equal(cmd,"creation_fichier" )){
    file_createFile(path, content);
  }else if(cmd_equal(cmd,"lire_fichier")){
    file_read(path);
  }else if(cmd_equal(cmd,"suppression_fichier")){
    file_delAll(path);
  }else if(cmd_equal(cmd,"creation_repertoire")){
    file_createRepo(path);
  }else if(cmd_equal(cmd,"suppression_repertoire")){
    file_delAll(path);
  }else{
    printf("Erreur dans '%s': \n commande invalide \n", cmd);
  }
}


int cmd_equal(char* command1, char* command2){
  return !strcmp(command1, command2);
}
