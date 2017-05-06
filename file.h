#ifndef FILE_H
#define FILE_H
#include "config.h"

//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////


/* Differents types de fichiers */
enum FILE_TYPE
{
  Directory = 0,
  File = 1,
  ToRemove = 2
};

/* Structure d'un fichier */
typedef struct
{
  char absolutePath[ABSOLUTE_PATH_MAX_SIZE];
  int id;
  enum FILE_TYPE type;
} File_t;

/* Fonction principale initiant le fichier file.dat */
int initFileFile();

/* Cree un fichier */
int file_createFile(char* iPath, char* iContent);

/* Gere les erreurs de file_createFile */
int file_createFile_catchErr(char* iPath, char* iContent);

/* Cree un repositoire */
int file_createRepo(char* iPath);

/*Gere les erreurs de file_createRepo */
int file_createRepo_catchErr(char* iPath);

/* Supprime un File_t du fichier de fichiers selon son chemin absolu */
void file_del(char* iPath);

/* Supprime un File_t, ses enfants et gere les erreurs */
void file_delAll(char *iPath);

/* Supprime tous les enfants d'un fichier avant de le supprimer lui meme */
int file_delChildren(char* iPath);

/* Retourne la position d'un File dans le fichier file.dat */
int file_find(char* iPath);

/* Retourne un fichier provenant de file.dat */
File_t* file_get(char* iPath);

/* Confirme si un fichier (iPath) possede un parent */
int file_hasParent(char* iPath, char oParsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE]);

/* Vient lire le contenu d'un fichier */
void file_read(char* iPath);

/* Ecrit une entree de fichier dans le fichier file.dat */
int file_write(File_t* iFile, char* iContent);
#endif
