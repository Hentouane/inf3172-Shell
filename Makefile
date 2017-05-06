# Déclaration des variables importantes
CC = gcc
CFLAGS = -Wall -std=gnu99
LDFLAGS = -lm
#
PROG = miniOs
SRCS = main.c freeMemoryTable.c disk.c cmd.c file.c util.c inode.c
DISK = disk.dat free_memory_table.dat inode.dat file.dat

# Utilise les meme noms de fichiers pour les .o
OBJS = $(SRCS:.c=.o)

# Cible de construction du programme
$(PROG) : $(OBJS)
	$(CC) $(CFLAG) $(LDFLAGS) $(OBJS) -o $(PROG)

# Dépendances
# TODO ne pas hardcoder
freeMemoryTable.o: freeMemoryTable.h config.h
disk.o: disk.h freeMemoryTable.h config.h
main.o: freeMemoryTable.h disk.h config.h
util.o: util.h
cmd.o: cmd.h config.h
file.o: file.h config.h inode.h util.h
inode.o: inode.h config.h 
main.o: freeMemoryTable.h disk.h cmd.h config.h file.h inode.h

# Utilitaires
clean:
	/bin/rm -f core *~ $(OBJS)

full-clean:
	/bin/rm -f core *~ $(OBJS) $(PROG)
	/bin/rm -f core *~ $(DISK)
run:
	/bin/rm -f core *~ $(OBJS) $(PROG)
	/bin/rm -f core *~ $(DISK)
	make
	./$(PROG) commandes.txt
disk:
	make run
	vim disk.dat

testFile:
	
