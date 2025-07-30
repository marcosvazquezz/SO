#include "listaDirectorios.h"
#define DNULL NULL
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>

void createListD(listD *l) {
    *l = DNULL;
}
bool isEmptyListD(listD *L) {
    return *L == DNULL;
}
bool createNodeD(posD *p) {
    *p = malloc(sizeof(struct dnode));
    return *p != DNULL;
}
void freeListD(listD *memory_list) {
    posD current = *memory_list;
    posD next;
    while (current != DNULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *memory_list = DNULL;
}
bool insertItemD (struct tItemD c, posD p, listD *L) {
    posD q, r;
    if(!createNodeD(&q)){
        return false;
    }
    q->data = c;
    q->next = DNULL;
    if(*L ==DNULL){
        *L=q;
    }else {
        if (p == DNULL){
            for (r = *L; r->next !=DNULL; r = r->next){}
            r->next = q;
        } else {
            if (p == *L){
                q->next = *L;
                *L = q;
            } else {
                q->next = p->next;
                p->next = q;

            }
        }
    }
    return true;
}

posD firstD(listD L) {
    return L;
}

posD lastD(listD L) {
    posD p;
    for(p = L;  p != DNULL; p=p->next){}
    return p;
}

posD nextD(posD p){
    return p-> next;
}

posD findItemD(struct tItemD c, listD *L) {
    posD p;
    for(p=*L; (p!=DNULL) && (p->data.dir != c.dir); p = p->next){}
    return p;
}

void deleteAtPositionD(posD p, listD *L) {
    posD q;
    if (p == firstD(*L))
        *L = (*L) -> next;
    else {
        if (p->next == DNULL) {
            for (q = *L; q->next != p; q = q->next){}
            q->next = DNULL;
        } else {
            q = p->next;
            p->data = q->data;
            p->next = q->next;
            p = q;
        }
    }
    free(p);
}
