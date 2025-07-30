

#include <stdio.h>
#include <stdbool.h>
#include "listas.h"
#define LNULL NULL
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>


void createList(list *l){
    *l = LNULL;
}

bool isEmptyList(list *L) {
    return (*L == NULL);
}

bool createNode(pos *p){
    *p = malloc(sizeof(struct tnode));
    return *p != LNULL;
}

bool insertItem ( struct tItemC c,pos p, list *L){
    pos q, r;
    if(!createNode(&q)){
        return false;
    }
    q->data = c;
    q->next = LNULL;
    if(*L ==LNULL){
        *L=q;
    }else {
        if (p == LNULL){
            for (r = *L; r->next !=LNULL; r = r->next){}
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


void updateItem( struct tItemC c,pos p, list *L){
    p-> data = c;
}

pos first(list L){
    return L;
}

pos last(list L){
    pos p;
    for(p = L;  p != LNULL; p=p->next){}
    return p;
}

pos next(pos p, list L){
    return p-> next;
}

pos previous(pos p, list L){
    pos q;
    if(p==L)
        q= LNULL;
    else
        for(q=L; q->next !=p; q=q->next){}
    return q;
}

void deleteAtPosition(pos p, list *L){
    pos q;
    if (p == first(*L))
        *L = (*L) -> next;
    else {
        if (p->next == LNULL) {
            for (q = *L; q->next != p; q = q->next){}
            q->next = LNULL;
        } else {
            q = p->next;
            p->data = q->data;
            p->next = q->next;
            p = q;
        }
    }
    free(p);
}

pos findItem(struct tItemC c, list L){
    pos p;
    for(p=L; (p!=LNULL) && (p->data.comando !=c.comando); p = p->next){}
    return p;
}

bool is_eol( pos p) {
    return p->next == NULL;
}

void freeList(list *historico) {
    pos current = *historico;
    pos next;
    while (current != LNULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *historico = LNULL;
}
