#ifndef CMD_H
#define CMD_H

//////////////////////////////////////////////
// Auteurs : Antoine St-Denis - STDA01059305
//           Theo Meyer       - MEYT02089105
//
// Dans le cadre du cours INF3172
// Pour: Jacques Berger
//////////////////////////////////////////////


// Contient tout les fonctions permettant de lire les commands entrées en
// paramettres et d'executer les fonctions désirés


/* parse le fichier entré en parametre dans la ligne de commande et retourne un tableau de string contenant les commandes */
void cmd_parseInputFile(int argc, char **argv);

/* Parse une string /line/ := une ligne de commandes a executer */
void cmd_parseLine(char* line);

/* Compare deux strings */
int cmd_equal(char* command1, char* command2);

/* Execute une commande une fois celle-ci parser */
void cmd_execute(char *cmd, char* path, char* content);

#endif
