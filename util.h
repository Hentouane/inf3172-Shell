#ifndef UTIL_H
#define UTIL_H
//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////

/* Parse un chemin absolu en un tableau selon les divers repertoire le composant */
int parsePath(char* iPath, char oParsedPath[ABSOLUTE_PATH_MAX_REPO][ABSOLUTE_PATH_MAX_SIZE]);
#endif
