#ifndef FICHERO_H
#define FICHERO_H

#include <stdbool.h>

struct tItemF {
    int descriptor;
    char flags[1024];
    char filename[2048];
};

struct tnodeF{
    struct tItemF data;
    struct tnodeF *next;
};



typedef struct tnodeF *listF;
typedef struct tnodeF *posF;



void createListF(listF *l);
bool insertItemF (struct tItemF c,posF p, listF *L);
void updateItemF(struct tItemF c,posF p, listF *L);
posF firstF(listF L);
posF lastF(listF L);
posF nextF(posF p, listF L);
posF previousF(posF p, listF L);
void deleteAtPositionF(posF p, listF *L);
posF findItemF(struct tItemF c, listF *L);
bool is_eolF(listF l, posF p);
bool isEmptyListF(listF *L);
void AnadirFicherosAbiertos(int df, int mode,listF *file_list,char *cadena[]);
void listarficherosabiertos(listF *l);
void EliminarFicherosAbiertos(int df,listF *file_list);
char NombreFicheroAbierto(int df,listF *file_list);
void freeListF(listF *file_list);

#endif //FICHERO_H