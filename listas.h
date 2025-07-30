#ifndef LISTAS_H
#define LISTAS_H
#include <stdbool.h>

struct tItemC{
    char comando [100];
};


struct tnode{
    struct tItemC data;
    struct tnode *next;
};


typedef struct tnode *list;
typedef struct tnode *pos;



void createList(list *l);
bool insertItem (struct tItemC c,pos p, list *L);
void updateItem(struct tItemC c,pos p, list *L);
pos first(list L);
pos last(list L);
pos next(pos p, list L);
pos previous(pos p, list L);
void deleteAtPosition(pos p, list *L);
pos findItem(struct tItemC c, list L);
bool is_eol(pos p);
bool isEmptyList(list *L);
void freeList(list *historico);




#endif //LISTAS_H

