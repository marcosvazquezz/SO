#ifndef LISTAPROCESOS_H
#define LISTAPROCESOS_H
#include <stdbool.h>
#include <time.h>
#include <string.h>


struct tItemP {
    pid_t pid;
    char  command_line[2048];
    char *status;
    int  priority;
    time_t date_and_time;
    char *senal;
};


struct pnode {
    struct tItemP data;
    struct pnode *next;
};

typedef struct pnode *listP;
typedef struct pnode *posP;



void createListP(listP *l);
bool isEmptyListP(listP *L);
bool createNodeP(posP *p);
void freeListP(listP *memory_list);
bool insertItemP (struct tItemP c, posP p, listP *L);
posP lastP(listP L);
posP findItemP(struct tItemP c, listP *L);
void deleteAtPositionP(posP p, listP *L);
posP firstP(listP L);
struct tItemP getItemP(posP p, listP *L);
char *NombreSenal(int sen);
void updateItemP(struct tItemP item, posP p, listP *L);
posP nextP(posP p, listP *L);
//void VaciarListaP(listP *L);



#endif //LISTAPROCESOS_H
