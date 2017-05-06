#ifndef FREEMEMORYTABLE_H
#define FREEMEMORYTABLE_H

//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////


// Toutes les fonctions concernant le fichier freeMemoryTable.dat.
// c'est un fichier texte contenant des 0 et des 1. Un 1
// à un index N signifie que la memoire du Nième block est disponible dans le fichier DISK_FILE_NAME .
// Le 0 signifie que le block est reservé.
// Ce fichier contient toutes les fonctions relative au fichier FREE_MEMORY_TABLE_FILE_NAME .

/* Cree le fichier et l'initie avec des zeros */
FILE* freeMemoryTable_CreateFile();

/* Retourne un pointeur vers un tableau listant les blocs disponibles */
int* freeMemoryTable_GetAvailableBlock(int* ioAvailableBlock, int iNeededBlockNumber);

/* Gere les erreurs de freeMemoryTable_GetAvailableBlock */
int freeMemoryTable_GetAvailableBlock_catchErr(int iNeededBlockNumber);

/* Compte le nombre de blocks disponibles restants sur le disque */
int freeMemoryTable_getNumberOfAvailableBlock();

/* Change le status d'un bloque pour locked car il est utilise */
void freeMemoryTable_lockBlock(int blockAddress);

/* Change le status d'un bloque pour unlocked car il est libre */
void freeMemoryTable_unLockBlock(int blockAddress);

#endif
