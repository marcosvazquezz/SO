#include "listaProcesos.h"
#define PNULL NULL
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
struct SEN {
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={
    {"HUP", SIGHUP},
    {"INT", SIGINT},
    {"QUIT", SIGQUIT},
    {"ILL", SIGILL},
    {"TRAP", SIGTRAP},
    {"ABRT", SIGABRT},
    {"IOT", SIGIOT},
    {"BUS", SIGBUS},
    {"FPE", SIGFPE},
    {"KILL", SIGKILL},
    {"USR1", SIGUSR1},
    {"SEGV", SIGSEGV},
    {"USR2", SIGUSR2},
    {"PIPE", SIGPIPE},
    {"ALRM", SIGALRM},
    {"TERM", SIGTERM},
    {"CHLD", SIGCHLD},
    {"CONT", SIGCONT},
    {"STOP", SIGSTOP},
    {"TSTP", SIGTSTP},
    {"TTIN", SIGTTIN},
    {"TTOU", SIGTTOU},
    {"URG", SIGURG},
    {"XCPU", SIGXCPU},
    {"XFSZ", SIGXFSZ},
    {"VTALRM", SIGVTALRM},
    {"PROF", SIGPROF},
    {"WINCH", SIGWINCH},
    {"IO", SIGIO},
    {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
    {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
    {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
    {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
    {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
    {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
    {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
    {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
    {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
    {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
    {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
    {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
    {"WAITING", SIGWAITING},
#endif
     {NULL,-1},
    };    /*fin array sigstrnum */


int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/
{
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}


char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/
{			/* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

void createListP(listP *l) {
    *l = PNULL;
}
bool isEmptyListP(listP *L) {
    return *L == PNULL;
}
bool createNodeP(posP *p) {
    *p = malloc(sizeof(struct pnode));
    return *p != PNULL;
}
void freeListP(listP *memory_list) {
    posP current = *memory_list;
    posP next;
    while (current != PNULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *memory_list = PNULL;
}
bool insertItemP (struct tItemP c, posP p, listP *L) {
    posP q, r;
    if(!createNodeP(&q)){
        return false;
    }
    q->data = c;
    q->next = PNULL;
    if(*L ==PNULL){
        *L=q;
    }else {
        if (p == PNULL){
            for (r = *L; r->next !=PNULL; r = r->next){}
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

posP firstP(listP L) {
    return L;
}

posP lastP(listP L) {
    posP p;
    for(p = L;  p != PNULL; p=p->next){}
    return p;
}

posP findItemP(struct tItemP c, listP *L) {
    posP p;
    for(p=*L; (p!=PNULL) && (p->data.pid != c.pid); p = p->next){}
    return p;
}
void deleteAtPositionP(posP p, listP *L) {
    posP q;
    if (p == firstP(*L))
        *L = (*L) -> next;
    else {
        if (p->next == PNULL) {
            for (q = *L; q->next != p; q = q->next){}
            q->next = PNULL;
        } else {
            q = p->next;
            p->data = q->data;
            p->next = q->next;
            p = q;
        }
    }
    free(p);
}

struct tItemP getItemP(posP p, listP *L) {
    return p->data;
}

void updateItemP(struct tItemP item, posP p, listP *L) {
    p->data = item;
}

posP nextP(posP p, listP *L) {
    return p->next;
}

/*void VaciarListaP(listP *L) {
    posP i;
    for (i = firstP(*L); i != PNULL; i = i->next) {
        deleteAtPositionP(i,L);
    }

}*/
