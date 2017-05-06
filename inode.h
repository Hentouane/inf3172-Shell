#ifndef INODE_H
#define INODE_H
//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////

// Toutes les fonctions concernant le fichier inode.dat ( ou plutot INODE_FILE_NAME).

/* Structure d'une indirection simple */
typedef struct{
  int block[NUMBER_OF_INODE_BLOCK];
} IndBlock_t;

typedef struct{
  int size;
  int block[NUMBER_OF_INODE_BLOCK];
  IndBlock_t singleIndirect;
} Inode_t;

/* Cree le fichier inode.dat */
int createInodeFile();

/* Ecrit la string iDataToWrite sur les blocs disponibles ( Meme si ils ne sont pas contigue) et retourne les index des blocks utilisées */
void inode_create(int* iUsedBlocks, int iInodeId, int iNumberOfUsedBlocks);

/* Cherche une inode du fichier inode.dat */
Inode_t* inode_get(int iInodeId);

#endif
