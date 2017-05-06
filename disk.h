#ifndef DISK_H
#define DISK_H

//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////

// Toutes les fonctions concernant le fichier disk.dat ( ou plutot DISK_FILE_NAME).
// c'est un fichier texte contenant les donnes des fichiers. 

/* Creer le fichier et l'initie avec des zeros */
FILE* createDiskFile();

/* Ecrit la string iDataToWrite sur les blocs disponibles ( Meme si ils ne sont pas contigue) et retourne les index des blocks utilisées */
void disk_write(char* iDataToWrite, int* iUsedBlocks);

/* Gere les erreurs de disk_write */
int disk_write_catchErr( int dataSize, char* iDataToWrite, FILE* diskFile, int numberOfAvailableBlocks, int neededBlockNumber);

/* Ecrit la chaine de caractere iDataToWrite sur le un bloc du fichier DISK_FILE_NAME.dat */
void disk_writeBlock(char* iDataToWrite, int iBlockAdress);

/* Supprime une liste de blocks */
void disk_deleteBlocks(int *blocksToDelete, int iNumberOfBlocksToDelete);

/* Supprime un block en particulier par son adresse */
void disk_deleteBlock(int iBlockAddress );

/* Reinitialise un block avec des 0 */
void disk_cleanBlock(int iBlockId);

/* Lit la valeur d'un block par son adresse */
void disk_readBlock(int iBlockAddress);

#endif
