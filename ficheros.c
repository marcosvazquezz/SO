#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "ficheros.h"

#include "listas.h"
#define LNULL NULL


void createListF(listF *l){
    *l = LNULL;
}

bool isEmptyListF(listF *L) {
    return (*L == NULL);
}

bool createNodeF(posF *p){
    *p = malloc(sizeof(struct tnodeF));
    return *p != LNULL;
}

bool insertItemf ( struct tItemF c,posF p, listF *L){
    posF q, r;
    if(!createNodeF(&q)){
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


void updateItemF( struct tItemF c,posF p, listF *L){
    p-> data = c;
}

posF firstF(listF L){
    return L;
}

posF lastF(listF L){
    posF p;
    for(p = L;  p != LNULL; p=p->next){}
    return p;
}

posF nextF(posF p, listF L){
    return p-> next;
}

posF previousF(posF p, listF L){
    posF q;
    if(p==L)
        q= LNULL;
    else
        for(q=L; q->next !=p; q=q->next){}
    return q;
}

void deleteAtPositionF(posF p, listF *L){
    posF q;
    if (p == firstF(*L))
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

posF findItemF(struct tItemF c, listF *L){
    posF p;
    for(p=*L; (p!=LNULL) && (p->data.descriptor != c.descriptor); p = p->next){}
    return p;
}

bool is_eolF(listF l, posF p) {
    return p->next == NULL;
}


void inicializarFicheros(listF *l) {
    struct tItemF f;
    posF p = *l;
    for (int i = 0; i < 10; i++) {
        f.descriptor = i;
        if(i == 0) {
            strcpy(f.filename,"entrada estandar");
            strcpy(f.flags,"O_RDWR");
            strcat(f.flags," O_APPEND");
        } else if(i == 1) {
            strcpy(f.filename,"salida estandar");
            strcpy(f.flags,"O_RDWR");
            strcat(f.flags," O_APPEND");
        } else {
            strcpy(f.filename,"error estandar");
            strcpy(f.flags,"O_RDWR");
            strcat(f.flags," O_APPEND");
        }
        if (i >= 3) {
            strcpy(f.filename,"No usado");
            strcpy(f.flags,"");
        }


        insertItemf(f,p,l);
        if (p != NULL) {
            p = p->next;
        }

    }
}

void listarficherosabiertos(listF *l) { //Revisar iteraciones con el insert (da problemas)

    posF p;
    if(*l == NULL) {
        inicializarFicheros(l);
    }

    for (p = firstF(*l); p != lastF(*l); p = p->next) {//Print de la lista de descriptores
        printf("Descriptor %d: -> %s %s\n",p->data.descriptor,p->data.filename,p->data.flags);

    }
}
void AnadirFicherosAbiertos(int df, int mode,listF *file_list,char *cadena[]) {
    if (*file_list == NULL) {
        inicializarFicheros(file_list);
    }
    struct tItemF c;
    c.descriptor = df;
    strcpy(c.filename,"No usado");
    strcpy(c.flags,"");
    posF p =findItemF(c,file_list);
    char flag[1024];
    strcpy(flag,"");

    if(cadena[2] != NULL || strcmp(cadena[0],"dup") == 0 || strcmp(cadena[0], "allocate") == 0) {

        if (mode & O_CREAT) strcat(flag, "O_CREAT ");
        if(mode & O_EXCL) strcat(flag, "O_EXCL ");
        if (mode & O_RDONLY || mode == 32768 || mode == O_RDONLY) strcat(flag, "O_RDONLY ");
        if (mode & O_WRONLY) strcat(flag, "O_WRONLY ");
        if (mode & O_RDWR) strcat(flag, "O_RDWR ");
        if (mode & O_APPEND) strcat(flag, "O_APPEND ");
        if (mode & O_TRUNC) strcat(flag, "O_TRUNC ");
    }

    if (strcmp(cadena[0], "allocate") == 0) {
        char name[120];
        strcpy(name,"Mapeo de ");
        strcat(name, cadena[2]);
        strcpy(p->data.filename, name);
        p->data.descriptor = df;
        strcpy(p->data.flags ,flag);
        return;
    }
    strcpy(p->data.filename, cadena[1]);
    p->data.descriptor = df;
    strcpy(p->data.flags ,flag);


}

void EliminarFicherosAbiertos(int df,listF *file_list) {
    posF p;
    for (p = firstF(*file_list); p != NULL; p = nextF(p,*file_list)) {
        if (df == p->data.descriptor) {
            strcpy(p->data.filename,"no usado");
            strcpy(p->data.flags,"");
            break;
        }
    }
}

void freeListF(listF *file_list) {
    posF current = *file_list;
    posF next;
    while (current != LNULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *file_list = LNULL;
}



