#ifndef LISTAMEMORIA_H
#define LISTAMEMORIA_H

#include "listas.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MNULL NULL

struct tItemM {
    void *memory_adress;
    int  size;
    char allocationtype[100];
    int  key;
    char name[100];
    int  filedescriptor;
    time_t time;
};


struct mnode {
    struct tItemM data;
    struct mnode *next;
};

typedef struct mnode *listM;
typedef struct mnode *posM;

void createListM(listM *l);
bool isEmptyListM(listM *L);
bool createNodeM(posM *p);
void freeListM(listM *memory_list);
bool insertItemM (struct tItemM c, posM p, listM *L);
posM lastM(listM L);
posM findItemM(struct tItemM c, listM *L);
void deleteAtPositionM(posM p, listM *L);
struct tItemM getNodeM(struct tItemM c, listM L);

#endif //LISTAMEMORIA_H
