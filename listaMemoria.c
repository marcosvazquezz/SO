#include "listaMemoria.h"
#define MNULL NULL

void createListM(listM *L){
    *L = MNULL;
}

bool isEmptyListM(listM *L) {
    return (*L == MNULL);
}

bool createNodeM(posM *p){
    *p = malloc(sizeof(struct mnode));
    return *p != MNULL;
}

void freeListM(listM *memory_list) {
    posM current = *memory_list;
    posM next;
    while (current != MNULL) {
        next = current->next;
        free(current->data.memory_adress);
        free(current);
        current = next;
    }
    *memory_list = MNULL;
}

bool insertItemM (struct  tItemM c, posM p, listM *L){
    posM q, r;
    if(!createNodeM(&q)){
        return false;
    }
    q->data = c;
    q->next = MNULL;
    if(*L ==MNULL){
        *L=q;
    }else {
        if (p == MNULL){
            for (r = *L; r->next !=MNULL; r = r->next){}
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

posM firstM(listM L){
    return L;
}

posM lastM(listM L){
    posM p;
    for(p = L;  p != MNULL; p=p->next){}
    return p;
}


/*posM findItemM(struct tItemM c, listM *L){
    posM p = firstM(*L);

    for(p=firstM(*L); (p!=MNULL) ; p = p->next) {
        if (strcmp(p->data.allocationtype, "malloc") == 0 && (p->data.memory_adress == c.memory_adress || p->data.size == c.size)){
            return p;
        }
        if (strcmp(p->data.allocationtype, "shared") == 0 && (p->data.key == c.key || p->data.memory_adress == c.memory_adress)){
            return p;
        }
        if (strcmp(p->data.allocationtype, "mmap") == 0 && (p->data.memory_adress == c.memory_adress || strcmp(p->data.name,c.name) == 0)){
            return p;
        }

    }
    return p;
}*/

posM findItemM(struct tItemM c, listM *L) {
    posM p;
    for (p = *L; p != MNULL; p = p->next) {
        if (c.memory_adress != NULL && p->data.memory_adress == c.memory_adress) {
            return p;
        }
        if (strcmp(c.allocationtype, p->data.allocationtype) == 0) {
            if (strcmp(c.allocationtype, "malloc") == 0 && c.size > 0 && p->data.size == c.size) {
                return p;
            } else if (strcmp(c.allocationtype, "mmap") == 0 && strlen(c.name) > 0 && strcmp(p->data.name, c.name) == 0) {
                return p;
            } else if ((strcmp(c.allocationtype, "shared") == 0 || strcmp(c.allocationtype, "createshared") == 0) && c.key != 0 && p->data.key == c.key) {
                return p;
            }
        }
    }
    return MNULL;
}

void deleteAtPositionM(posM p, listM *L){
    posM q;
    if (p == firstM(*L))
        *L = (*L) -> next;
    else {
        if (p->next == MNULL) {
            for (q = *L; q->next != p; q = q->next){}
            q->next = MNULL;
        } else {
            q = p->next;
            p->data = q->data;
            p->next = q->next;
            p = q;
        }
    }
    free(p);
}

struct tItemM getNodeM(struct tItemM c, listM L) {
    posM p = findItemM(c, &L);
    strcpy(c.allocationtype,p->data.allocationtype);
    c.filedescriptor = p->data.filedescriptor;
    c.key = p->data.key;
    c.size = p->data.size;
    c.memory_adress = p->data.memory_adress;
    c.size = p->data.size;
    strcpy(c.name,p->data.name);
    c.time = p->data.time;
    return c;


}

