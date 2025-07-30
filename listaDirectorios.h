#ifndef LISTADIRECTORIOS_H
#define LISTADIRECTORIOS_H
#include "listas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tItemD {
    char * dir;
};

struct dnode {
    struct tItemD data;
    struct dnode *next;
};

typedef struct dnode *listD;
typedef struct dnode *posD;

void createListD(listD *l);
bool isEmptyListD(listD *L);
bool createNodeD(posD *p);
void freeListD(listD *memory_list);
bool insertItemD (struct tItemD c, posD p, listD *L);
posD lastD(listD L);
posD findItemD(struct tItemD c, listD *L);
void deleteAtPositionD(posD p, listD *L);
posD firstD(listD L);
posD nextD(posD p);


#endif //LISTADIRECTORIOS_H
