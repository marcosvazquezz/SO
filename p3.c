#define MAX 100
#include <stdbool.h>
#include "comandos.h"
#include "listaMemoria.h"
#include "listaProcesos.h"
#include "listaDirectorios.h"

int main(int argc, char *argv[], char *envp[]){

    char entrada[MAX];
    bool ended = false;
    listF fileList;
    listM memList;
    list historic;
    listP procList;
    listD dirList;
    createList(&historic);
    createListF(&fileList);
    createListM(&memList);
    createListP(&procList);
    createListD(&dirList);
    while(!ended) {
        imprimirPrompt();
        leerEntrada(entrada, MAX);
        procesarEntrada(entrada,&ended,&historic,&fileList,&memList,&procList,&dirList,true);
    }
    freeList(&historic);
    freeListF(&fileList);
    freeListM(&memList);
    return 0;
}
