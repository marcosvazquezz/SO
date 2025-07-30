#ifndef COMANDOS_H
#define COMANDOS_H
#include "stdbool.h"
#include "listas.h"
#include "ficheros.h"
#include "listaMemoria.h"
#include "listaDirectorios.h"
#include "listaProcesos.h"

void quit(bool *ended);
void imprimirPrompt();
int trocearCadena(char * cadena, char * trozos[]);
void leerEntrada(char *cadena,int tamano);
void procesarEntrada (char *cadena,bool *ended,list *historico,listF *ficheros,listM *memList,listP *procList, listD *dirList,bool insert);
void authors(char *cadena[],int trozos);
void pid(char *cadena[], int trozos);
void ppid(char *cadena[], int trozos);
void date(char *cadena[],  int trozos);
void infosys();
void cd(char *comand[]);
void historic(char *cadena[], list *historico,listF *Ficheros,listM *memory,listP *procList,listD *dirList);
void Cmd_open (char * tr[],listF *file_list);
void Cmd_close (char *tr[],listF *file_list);
void makedir(char *cadena[],int trozos);
void Cmd_dup (char * tr[],listF * file_list);
void makedir (char *cadena[], int trozos);
void cwd ();
void makefile(char *cadena[],int trozos);
void listfile(char *cadena[]);
void listdir(char *cadena[]);
void erase(char *cadena[],int trozos);
void delrec(char *ruta);
void help(int trozos, char *comando);
void revlist(char *tr[],bool hid,bool extended,bool acc,bool link);
void reclist(char *tr[],bool hid,bool extended,bool acc,bool link);
//P2
int memory(int num_trozos, char *trozos[],listM L);
void allocate(char *trozos[],int num_trozos,listM *L,listF *ficheros);
void do_allocateMalloc(char *tr[], listM *L);
void do_AllocateMmap(char *arg[],listM *L,listF *fileList);
void imprimirLista(listM *L);
void * ObtenerMemoriaShmget (key_t clave, size_t tam, listM *memList);
void *cadtop(char *cadena);
void * MapearFichero (char *cadena[], int protection, listF *l, listM *memlist);
void do_AllocateCreateshared (char *tr[],listM *L);
void do_AllocateShared (char *tr[],listM *L);
void Do_pmap (void);
void do_DeallocateDelkey (char *args[]);
void deallocate(char *args[], listM *memList);
void Cmd_Read(char *ar[]);
void Cmd_ReadFile (char *ar[]);
ssize_t Leerdescriptor(char *f, void *p, size_t cont);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void memfill(char *tr[], listM * memList);
void Recursiva (int n);
void recursiva (char *cadena[]);
void memdump (char *args[]);
ssize_t EscribirFichero(int df, void *p, size_t cont);
ssize_t EscribirFichero2(char *f, void *p,size_t cont, int sobreescritura);
void Cmd_Write(char *ar[]);
void Cmd_WriteFile(char *ar[]);
/*/P3/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////*/
void Cmd_getuid();
void Cmd_setuid(char *tr[], int num_trozos);
void mostrarEntorno(char **entorno, char * nombre_entorno);
void Cmd_fork (char *tr[]);
int BuscarVariable (char * var, char *e[]);
void showvar(char *tr[], char *entorno[]);
void changevar(char *tr[],int num_trozos,char **env);
void subsvar(char *tr[], int num_trozos, char **env);
void Cmd_search(char *tr[],listD *rutaBusqueda);
void inicializarSearchList(listD *rutaBusqueda);
void printearPath(listD *rutaBusqueda);
void exec(char *tr[], listD d);
void execpri(char *tr[], listD d);
void back(char *tr[],listP *procList, listD *dirList);
void backpri(char *tr[],listP *procList, listD *dirList);
char * Ejecutable (char *s, listD d);
int CambiarVariable2(char * v1, char *v2,char * valor, char *e[]);
int CambiarVariable(char * var, char * valor, char *e[]);
void Cmd_environ(char *tr[],int num_trozos, char **env);
void listjobs(listP *procList);
void printearProcesos(posP p, listP *procList);
void estadosSenal(posP p, listP *procList);
void fgpri(char *tr[], listD d, listP procesos);
void fg(char *tr[], listD d, listP procesos);
int Execpve(char *tr[], char **NewEnv, int * pprio, listD d, char *argv[]);

#endif //COMANDOS_H