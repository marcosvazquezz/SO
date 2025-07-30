//Marcos Vázquez Pereira  marcos.vazquez.pereira@udc.es                 Grupo 2.1      27/09/2024
//Jorge Luaces Roura  jorge.roura@udc.es

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "comandos.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include "listaMemoria.h"
#include "listaDirectorios.h"
#include "listaProcesos.h"
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/resource.h>
#define TAMANO 2048
#define MAXVAR 1000
#define MAXPATH 2048
#define MAXNAME 2048

int global = 0, global1 = 0, global2 = 0;
int global3,global4,global5;
extern char **environ;


void authors(char *cadena[],int trozos) {
    if (trozos==1) {
        printf("Marcos Vazquez Pereira : marcos.vazquez.pereira@udc.es\n");
        printf("Jorge Luaces Roura : jorge.roura@udc.es\n");
    } else if (trozos == 2) {
        if (strcmp(cadena[1],"-l")==0) {
            printf("marcos.vazquez.pereira@udc.es\n");
            printf("jorge.roura@udc.es\n");
        }
        else if (strcmp(cadena[1],"-n")==0) {
            printf("Marcos Vazquez Pereira\n");
            printf("Jorge Luaces Roura\n");
        }else{
            printf("Argument not valid.\n");
        }
    }
    else {
        printf("Argument not valid.\n");
    }
}



void quit(bool *ended){
    *ended = true;
}

void imprimirPrompt() {
    printf("$");
}

void pid(char *cadena[], int trozos) {
if(trozos == 1 && strcmp(cadena[0],"pid")==0) {
    printf("Pid del shell: %d\n",getpid());
}else {
        perror("Argument not valid.\n");
    }
}

void ppid(char *cadena[], int trozos) {
    if(trozos == 1 && strcmp(cadena[0],"ppid")==0) {
        printf("Pid del padre del shell: %d\n",getppid());
    }
    else {
        printf("Argument not valid.\n");
    }
}

void date(char *cadena[],  int trozos) {
    time_t date;
    time(&date);
    localtime(&date);
    struct tm *localtime1 = localtime(&date);



    if(trozos == 1 && (strcmp(cadena[0],"date")==0)) {
        printf("%02d:%02d:%02d\n",localtime1->tm_hour,localtime1->tm_min,localtime1->tm_sec);
        printf("%02d/%02d/%02d\n",localtime1->tm_mday,localtime1->tm_mon+1,localtime1->tm_year+1900);
    } else if(trozos == 2) {
        if(strcmp(cadena[1],"-d")==0){
            printf("%02d/%02d/%02d\n",localtime1->tm_mday,localtime1->tm_mon+1,localtime1->tm_year+1900);
        } else if (strcmp(cadena[1], "-t") == 0){
            printf("%02d:%02d:%02d\n",localtime1->tm_hour,localtime1->tm_min,localtime1->tm_sec);
        }
        else {
            printf("Argument not valid.\n");
        }
    }
    else {
        printf("Argument not valid.\n");
    }
}

int trocearCadena(char *cadena, char * trozos[]) {
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void infosys() {
    struct utsname unamedata;
    uname(&unamedata);
    printf("%s (%s), OS: %s %s %s\n",unamedata.nodename,unamedata.machine,unamedata.sysname,unamedata.release,unamedata.version);
}

void cd(char *comand[]) {
    char buffer[1024];
    size_t sizeBuffer = sizeof(buffer);
    if (comand[1] == NULL) {
        printf("You are in:\n%s\n",getcwd(buffer,sizeBuffer));
        return;
    }
    if (chdir(comand[1]) !=0) {
        perror("Error changing directory.\n");

    }
}


void leerEntrada(char *cadena,int tamano) {
    fgets(cadena,tamano,stdin);
}


void help(int trozos, char *comando) {
    if (trozos == 1) {
        printf("Comandos disponibles:\n");
        printf("%10s", "authors");
        printf("%10s", "pid");
        printf("%10s", "ppid");
        printf("%10s", "cd");
        printf("%10s", "date");
        printf("%10s", "historic");
        printf("%10s", "open");
        printf("%10s", "close");
        printf("%10s", "dup");
        printf("%10s", "infosys");
        printf("%10s", "help");
        printf("%10s", "quit");
        printf("%10s", "exit");
        printf("%10s", "bye");
        printf("%10s", "makefile");
        printf("%10s", "makedir");
        printf("%10s", "listfile");
        printf("%10s", "cwd");
        printf("%10s", "listdir");
        printf("%10s", "reclist");
        printf("%10s", "revlist");
        printf("%10s", "erase");
        printf("%10s", "delrec");
        printf("\n");
    } else if (trozos == 2) {
        if (strcmp(comando, "authors") == 0) {
            printf("authors [-n|-l]  Muestra los nombres y/o logins de los autores\n");
        } else if (strcmp(comando, "pid") == 0) {
            printf("pid [-p]  Muestra el pid del shell o de su proceso padre\n");
        } else if (strcmp(comando, "ppid") == 0) {
            printf("ppid  Muestra el pid del proceso padre del shell\n");
        } else if (strcmp(comando, "cd") == 0) {
            printf("cd [dir]  Cambia (o muestra) el directorio actual del shell\n");
        } else if (strcmp(comando, "date") == 0) {
            printf("date [-d|-t]  Muestra la fecha y/o la hora actual\n");
        } else if (strcmp(comando, "historic") == 0) {
            printf("historic [-c|-N|N]  Muestra (o borra) el historico de comandos\n");
            printf("  -N: muestra los N primeros\n");
            printf("  -c: borra el historico\n");
            printf("  N: repite el comando N\n");
        } else if (strcmp(comando, "open") == 0) {
            printf("open fich m1 m2...  Abre el fichero fich\n");
            printf("  y lo anade a la lista de ficheros abiertos del shell\n");
            printf("  m1, m2... son modos de apertura (or bit a bit):\n");
            printf("  cr: O_CREAT, ap: O_APPEND, ex: O_EXCL\n");
            printf("  ro: O_RDONLY, rw: O_RDWR, wo: O_WRONLY, tr: O_TRUNC\n");
        } else if (strcmp(comando, "close") == 0) {
            printf("close df  Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n");
        } else if (strcmp(comando, "dup") == 0) {
            printf("dup df  Duplica el descriptor de fichero df y anade una nueva entrada a la lista de ficheros abiertos\n");
        } else if (strcmp(comando, "infosys") == 0) {
            printf("infosys  Muestra informacion de la maquina donde corre el shell\n");
        } else if (strcmp(comando, "help") == 0) {
            printf("help [cmd|-lt|-T|-all]  Muestra ayuda sobre los comandos\n");
            printf("  -lt: lista topics de ayuda\n");
            printf("  -T topic: lista comandos sobre ese topic\n");
            printf("  cmd: info sobre el comando cmd\n");
            printf("  -all: lista todos los topics con sus comandos\n");
        } else if (strcmp(comando, "quit") == 0) {
            printf("quit  Termina la ejecucion del shell\n");
        } else if (strcmp(comando, "exit") == 0) {
            printf("exit  Termina la ejecucion del shell\n");
        } else if (strcmp(comando, "bye") == 0) {
            printf("bye  Termina la ejecucion del shell\n");
        } else if (strcmp(comando, "makefile") == 0) {
            printf("makefile [name]  Crea un fichero de nombre name\n");
        } else if (strcmp(comando, "makedir") == 0) {
            printf("makedir [name]  Crea un directorio de nombre name\n");
        } else if (strcmp(comando, "listfile") == 0) {
            printf("listfile [-long][-link][-acc] name1 name2 ..  lista ficheros;\n");
            printf("  -long: listado largo\n");
            printf("  -acc: acesstime\n");
            printf("  -link: si es enlace simbolico, el path contenido\n");
        } else if (strcmp(comando, "cwd") == 0) {
            printf("cwd  Muestra el directorio actual del shell\n");
        } else if (strcmp(comando, "listdir") == 0) {
            printf("listdir [-hid][-long][-link][-acc] n1 n2 ..  lista contenidos de directorios\n");
            printf("  -long: listado largo\n");
            printf("  -hid: incluye los ficheros ocultos\n");
            printf("  -acc: acesstime\n");
            printf("  -link: si es enlace simbolico, el path contenido\n");
        } else if (strcmp(comando, "reclist") == 0) {
            printf("reclist [-hid][-long][-link][-acc] n1 n2 ..  lista recursivamente contenidos de directorios (subdirs despues)\n");
            printf("  -hid: incluye los ficheros ocultos\n");
            printf("  -long: listado largo\n");
            printf("  -acc: acesstime\n");
            printf("  -link: si es enlace simbolico, el path contenido\n");
        } else if (strcmp(comando, "revlist") == 0) {
            printf("revlist [-hid][-long][-link][-acc] n1 n2 ..  lista recursivamente contenidos de directorios (subdirs antes)\n");
            printf("  -hid: incluye los ficheros ocultos\n");
            printf("  -long: listado largo\n");
            printf("  -acc: acesstime\n");
            printf("  -link: si es enlace simbolico, el path contenido\n");
        } else if (strcmp(comando, "erase") == 0) {
            printf("erase [name1 name2 ..]  Borra ficheros o directorios vacios\n");
        } else if (strcmp(comando, "delrec") == 0) {
            printf("delrec [name1 name2 ..]  Borra ficheros o directorios no vacios recursivamente\n");
        }else if (strcmp(comando, "allocate") == 0) {
            printf("allocate [-malloc|-shared|-createshared|-mmap]... Asigna un bloque de memoria\n");
            printf("-malloc tam : asigna un bloque malloc de tamano tam\n");
            printf("-createshared cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n");
            printf("-shared cl: asigna el bloque de memoria compartida (ya existente) de clave cl\n");
            printf("-mmap fich perm: mapea el fichero fich, perm son los permisos\n");
        }else if (strcmp(comando, "deallocate") == 0) {
            printf("deallocate [-malloc|-shared|-delkey|-mmap|addr]... Desasigna un bloque de memoria\n");
            printf("-malloc tam : desasigna el bloque malloc de tamano tam\n");
            printf("-shared cl: desasigna (desmapea) el bloque de memoria compartida de clave cl\n");
            printf("-delkey cl: elimina del sistema (sin desmapear) la clave de memoria cl\n");
            printf("-mmap fich: desmapea el fichero mapeado fich\n");
            printf("addr: desasigna el bloque de memoria en la direccion addr\n");
        }else if (strcmp(comando, "memfill") == 0) {
            printf("memfill addr cont byte Llena la memoria a partir de addr con byte\n");
        }else if (strcmp(comando, "memdump") == 0) {
            printf("memdump addr cont  Vuelca en pantallas los contenidos(cont bytes) de la posicion de memoria addr\n");
        }else if (strcmp(comando, "memory") == 0) {
            printf("memory [-blocks|-funcs|-vars|-all|-pmap]... Muestra muestra detalles de la memoria del proceso\n");
            printf("-blocks: los bloques de memoria asignados\n");
            printf("-funcs: las direcciones de las funciones\n");
            printf("-vars: las direcciones de las variables\n");
            printf("-all: todo\n");
            printf("-pmap: muestra la salida del comando pmap(o similar)");
        }else if (strcmp(comando, "readfile") == 0) {
            printf("readfile fiche addr cont   Lee cont bytes desde fich a la direccion addr\n");
        }else if (strcmp(comando, "writefile") == 0) {
            printf("writefile [-o] fiche addr cont Escribe cont bytes desde la direccion addr a fich (-o sobreescribe\n");
        }else if (strcmp(comando, "read") == 0) {
            printf("read df addr cont   Transfiere cont bytes del fichero descrito por df a la direccion addr\n");
        }else if (strcmp(comando, "write") == 0) {
            printf("write df addr cont  Transfiere cont bytes desde la direccion addr al fichero descrito por df\n");
        }else if (strcmp(comando,"getuid")==0) {
            printf("getuid Muestra las credenciales del proceso que ejecuta el shell\n");
        }else if (strcmp(comando,"setuid")==0) {
            printf("setuid [-l] id Cambia las credenciales del proceso que ejecuta el shell\n");
            printf("id : establece la credencial al valor numerico id\n");
            printf("- l id: establece la credencial a login id\n");
        }else if (strcmp(comando,"showvar")==0) {
            printf("showvar v1 v2 Muestra el valor y las direcciones de la variable de entorno var\n");
        }else if (strcmp(comando,"changevar")==0) {
            printf("changevar [-a|-e|-p] var valor Cambia el valor de una variable de entorno\n");
            printf("-a: accede por el tercer arg de main\n");
            printf("-e: accede mediante environ\n");
            printf("-p: accede mediante putenv\n");
        }else if (strcmp(comando,"subsvar")==0) {
            printf("subsvar [-a|-e] var1 var2 valor Sustituye la variable de entorno var1\n");
            printf("con var2 = valor\n");
            printf("-a: accede por el tercer arg de main\n");
            printf("-e: accede mediante environ\n");
        }else if (strcmp(comando,"environ")==0) {
            printf("environ [-environ|-addr]   Muestra el entorno del proceso\n");
            printf("-environ: accede usando environ (en lugar del tercer arg de main\n");
            printf("-addr: muestra el valor y donde se almacenan environ y el 3er arg main\n");
        }else if (strcmp(comando,"fork")==0) {
            printf("fork  El shell hace fork y queda en espera a que su hijo termine\n");
        }else if (strcmp(comando,"search")==0) {
            printf("search [-add|-del|-clear|-path]..  Manipula o muestra la ruta de busqueda del shell (path)\n");
            printf("-add dir: aniade dir a la ruta de busqueda(equiv +dir\n");
            printf("-del dir: elimina dir de la ruta de busqueda (equiv -dir)\n");
            printf("-clear: vacia la ruta de busqueda\n");
            printf("-path: importa el PATH en la ruta de busqueda\n");
        }else if (strcmp(comando,"exec")==0) {
            printf("exec VAR1 VAR2 ..prog args....[@pri] Ejecuta,sin crear proceso, prog con argumentos en un entorno que contiene solo las variables VAR1,VAR2...\n");
        }else if (strcmp(comando,"execpri")==0) {
            printf("execpri prio prog args....   Ejecuta, sin crear proceso, prog con argumentos con la prioridad cambiada a prio\n");
        }else if (strcmp(comando,"fg")==0) {
            printf("fg prog args... Crea un proceso que ejecuta en primer plano prog con argumentos\n");
        }else if (strcmp(comando,"fgpri")==0) {
            printf("fgpri prio prog args... Crea un proceso que ejecuta en primer plano prog con argumentos con la prioridad cambiada a prio\n");
        }else if (strcmp(comando,"back")==0) {
            printf("back prog args... Crea un proceso que ejecutae en segundo plano prog con argumentos\n");
        }else if (strcmp(comando,"backpri")==0) {
            printf("backpri prio prog args...   Crea un proceso que ejecuta en segundo plano prog con argumentos con la prioridad cambiada a prio\n");
        }else if (strcmp(comando,"listjobs")==0) {
            printf("listjobs  Lista los procesos en segundo plano\n");
        }else if (strcmp(comando,"deljobs")==0) {
            printf("deljobs [-term][-sig]  Elimina los procesos de la lista procesos en sp\n");
            printf("-term: los terminados\n");
            printf("-sig: los terminados por senal\n");
        }else if (strcmp(comando, "recurse") == 0) {
            printf("recurse [n] Invoca a la función recursiva n veces\n");
        }else {
            printf("El comando %s no esta recogido en el terminal.\n", comando);

        }
    }
}

void Cmd_open (char * tr[],listF *file_list) {
    int i,df, mode=0;

    if (tr[1]==NULL) {
        listarficherosabiertos(file_list);
        return;
    }
    if(tr[2] != NULL) {
        for (i=2; tr[i]!=NULL; i++)
        if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC;
        else break;
    }


    if ((df=open(tr[1],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        AnadirFicherosAbiertos(df,mode,file_list,tr);
        printf ("Anadida entrada %d a la tabla ficheros abiertos.\n",df);
    }
}

void Cmd_close (char *tr[],listF *file_list){
    int df;

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
       listarficherosabiertos(file_list);
          return;
    }


    if (close(df)==-1) {
        perror("Inposible cerrar descriptor");
    }else {
        EliminarFicherosAbiertos(df,file_list);
        printf("Eliminada la entrada %d de la tabla de ficheros abiertos\n",df);
    }
 }


void historic(char *cadena[], list *historico,listF *Ficheros,listM *memory,listP *procList,listD *dirList) {
    struct tItemC c;
    pos p;
    int contador = 0;
    int n;

    strcpy(c.comando, cadena[0]);
    if(cadena[1] == NULL) {
        for (p =first(*historico); p != NULL; p = next(p,*historico)) {
            printf("%d -> %s\n", contador, p->data.comando);
            contador++;
        }
    } else {
        n  = atoi(cadena[1]);


        if (n >= 0) {


            for (p =first(*historico);contador < n && p != NULL; p = next(p,*historico)) {
                contador++;
            }
            if(p == NULL) {
                printf("No hay elemento %d en el historico\n",n);
            }else {
                printf("%d -> Ejecutando (%s)\n", contador, p->data.comando);
                procesarEntrada(p->data.comando,false,historico,Ficheros,memory,procList,dirList,false);

            }



        } else {
            p = first(*historico);

            while (p!=NULL){ p = next(p,*historico);contador++;}
            int limite = contador + n;
            for (p =previous(last(*historico),*historico); contador != limite; p = previous(p,*historico)) {

                printf("%d -> %s\n", contador, p->data.comando);
                contador--;

            }
        }

    }
}

void cwd () {
    char buffer[1024];
    size_t sizeBuffer = sizeof(buffer);
    printf("You are in:\n%s\n",getcwd(buffer,sizeBuffer));

}

void Cmd_dup (char * tr[],listF * file_list){

    int df,duplicado;
    char p[20],aux[100];

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
        listarficherosabiertos(file_list);
        return;
    }
    struct tItemF f;
    posF q;
    f.descriptor = df;
    q = findItemF(f,file_list);
    if(q == NULL || strcmp(q->data.filename,"No usado") == 0) {
        printf("No hay ningun descriptor abierto en la entrada %d\n",df);
        return;
    }
    strcpy(p,q->data.filename);
    duplicado = dup(df);

    sprintf(aux,"dup %d (%s)",duplicado,p);
    strcpy(tr[1],aux);
    int flags = fcntl(df, F_GETFL);
    if (flags == -1) {
        perror("Error obteniendo los flags del descriptor");
        close(duplicado);
        return;
    }
    AnadirFicherosAbiertos(duplicado,fcntl(df,F_GETFL),file_list,tr);
}


char LetraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

bool isEmptyDir(const char *directorio) {
    struct dirent *entradaDirectorio;
    int contadorEntradas = 0;
    DIR *d=opendir(directorio);
    if(d == NULL) return false;
    while ((entradaDirectorio=readdir(d))!=NULL) {
        if (strcmp(entradaDirectorio->d_name,".")==0 || strcmp(entradaDirectorio->d_name,"..")==0) continue;
        contadorEntradas++;
    }
    closedir(d);
    return contadorEntradas==0;
}


void makedir (char *cadena[], int trozos) {

    if (trozos == 1) {
        if (strcmp(cadena[0],"makedir")==0) {
            cd(cadena);
        }
        return;
    }

    if (trozos > 1) {
        if (mkdir(cadena[1],0777) == -1) {
            printf("No ha sido posible crear el directorio %s\n",strerror(errno));
        } else printf("Directorio creado\n");
    }
}



void makefile(char *cadena[],int trozos) {
    int df = 0;
    if (trozos == 1 ) {
        cd(cadena);
    }else if (trozos == 2) {
            df = open(cadena[1],O_CREAT,0777);
            close(df);
        }
    if (df == -1) {
        perror("Error creating file.\n");

    }
}

void listfile(char *cadena[]) {
    int n;
    bool flagLong =false, flagLink=false, flagAcc=false;
    struct stat info;
    struct passwd *pw;
    struct group *gr;
    char fechaCreacion[24],linkString[1024] = "", permisos[256] = "",usuario[256] = "",grupo[256] = "";
    ssize_t longEntrada;
    char buffer[512];
    char inodo[255];
    char hardLinks[255];
    char cwd[1024];


    if(cadena[1]==NULL) {
        cd(cadena);
        return;
    }
    n = 1;
    while(cadena[n]!=NULL) {
        if (strcmp(cadena[n], "-long") == 0) flagLong = true;
        else if (strcmp(cadena[n], "-acc") == 0) flagAcc = true;
        else if (strcmp(cadena[n], "-link") == 0) flagLink = true;
        else break;
        n++;
    }

    if(cadena[n]==NULL) {
        printf("You are in:\n%s\n",getcwd(cwd,sizeof(cwd)));
        return;
    }

    while(cadena[n]!=NULL) {
        printf("---------------{%s}---------------\n",cadena[n]);
        sprintf(fechaCreacion,"%s","");
        sprintf(inodo,"%s","");
        sprintf(hardLinks,"%s","");
        sprintf(permisos,"%s","");
        sprintf(usuario,"%s","");
        strcpy(grupo,"");
        strcpy(linkString,"");



        if (lstat(cadena[n],&info) != 0) {
            perror("Error obteniendo la informacion del archivo.");
            return;
        }

        if (flagLong) {
            gr = getgrgid(info.st_gid);
            pw = getpwuid(info.st_uid);
            if (gr == NULL || pw == NULL) {
                perror("Error obteniendo los permisos de grupo y usuario.");
                return;
            }

            sprintf(usuario,"%s",pw->pw_name);
            sprintf(grupo,"%s",gr->gr_name);
            sprintf(permisos,"%s",ConvierteModo2(info.st_mode));
            sprintf(hardLinks,"%ld",info.st_nlink);
            sprintf(inodo,"( %lu )",info.st_ino);

            struct tm *fechaModificacion = localtime(&info.st_mtime);
            strftime(fechaCreacion,sizeof(fechaCreacion),"%Y/%m/%d-%H:%M",fechaModificacion);

        }

        if(flagAcc){
            struct tm *fechaAcceso = localtime(&info.st_atime);
            strftime(fechaCreacion,sizeof(fechaCreacion),"%Y/%m/%d-%H:%M",fechaAcceso);
        }


        if(flagLink) {
            memset(buffer,0,sizeof(buffer));
            longEntrada = readlink(cadena[n],buffer,sizeof(buffer));
            if(longEntrada != -1) {
                buffer[longEntrada] = '\0';
                sprintf(linkString,"-> %s",buffer);
            }
        }
        printf ("%1s %2s %2s %4s %2s %2s %2ld %2s %2s\n",fechaCreacion,hardLinks,inodo,grupo,usuario,permisos,info.st_size,
                 cadena[n],linkString);
        n++;
    }

}



void listdir(char *cadena[]) {
    int n;
    bool flagLong = false, flagHid = false, flagAcc = false, flagLink = false;
    struct dirent *entradaDirectorio;
    DIR *directorio;
    char cwd[1024],linkString[256];
    struct stat info;
    char fechaCreacion[24];
    struct tm *fechaMod;
    char buffer[sizeof(entradaDirectorio)];
    ssize_t longEntrada;
    char inodo[255];
    char hardLinks[255];
    char permisos [255];
    char usuario[255];
    char grupo[255];
    char *aux;
    struct group *gr;
    struct passwd *pw;


    if(cadena[1]==NULL) {
       cd(cadena);
        return;
    }

    n = 1;
    while(cadena[n]!=NULL) {
        if (strcmp(cadena[n], "-long") == 0) flagLong = true;
        else if (strcmp(cadena[n], "-hid") == 0) flagHid = true;
        else if (strcmp(cadena[n], "-acc") == 0) flagAcc = true;
        else if (strcmp(cadena[n], "-link") == 0) flagLink = true;
        else break;
        n++;
    }

    if(cadena[n]==NULL) {
        printf("You are in:\n%s\n",getcwd(cwd,sizeof(cwd)));
        return;
    }

    while(cadena[n]!=NULL) {
        printf("---------------{%s}---------------\n",cadena[n]);
        sprintf(fechaCreacion,"%s","");
        sprintf(inodo,"%s","");
        sprintf(hardLinks,"%s","");
        sprintf(permisos,"%s","");
        sprintf(usuario,"%s","");
        strcpy(grupo,"");
        strcpy(linkString,"");

        directorio = opendir(cadena[n]);
        if (directorio == NULL) {
            perror("Error opening directory.\n");
            return;
        }

        while((entradaDirectorio = readdir(directorio)) != NULL) {
            if(!flagHid && entradaDirectorio->d_name[0] == '.') {
                continue;
            }
            aux = malloc(strlen(entradaDirectorio->d_name) + strlen(cadena[n]) + 2);
            sprintf(aux,"%s/%s",cadena[n],entradaDirectorio->d_name);
            if(lstat(aux,&info) != 0) {
                perror("Error al obtener informacion del directorio.");
                continue;
            }
            free(aux);

            if(flagLong) {
                gr = getgrgid(info.st_gid);
                pw = getpwuid(info.st_uid);
                if(gr == NULL || pw == NULL) {
                    perror("Error al obtener el grupo y el usuario.");
                }
                sprintf(usuario, "%s",pw->pw_name);
                sprintf(grupo, "%s",gr->gr_name);
                sprintf(permisos, "%s",ConvierteModo2(info.st_mode));
                sprintf(hardLinks,"%ld",info.st_nlink);
                sprintf(inodo,"( %lu )",info.st_ino);
                fechaMod = localtime(&info.st_mtime);
                strftime(fechaCreacion,sizeof(fechaCreacion),"%Y/%m/%d-%H:%M",fechaMod);
            }

            if(flagAcc) {
                struct tm *fechaAcceso = localtime(&info.st_atime);
                strftime(fechaCreacion,sizeof(fechaCreacion),"%Y/%m/%d-%H:%M",fechaAcceso);

            }

            if(flagLink) {
                memset(buffer,0,sizeof(buffer));
                longEntrada = readlink(entradaDirectorio->d_name,buffer,sizeof(buffer));
                if(longEntrada == -1) {
                    strcpy(linkString,"");
                }else sprintf(linkString,"-> %s",buffer);
            }
            printf ("%1s %2s %2s %4s %2s %2s %2ld %2s %2s\n",fechaCreacion,hardLinks,inodo,grupo,usuario,permisos,info.st_size,
                entradaDirectorio->d_name,linkString);
        }
        closedir(directorio);
        n++;
    }
}


void printextend(struct stat info) {
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y/%m/%d-%H:%M", localtime(&info.st_ctime));
    struct passwd *userinfo =getpwuid(info.st_uid);
    struct group *groupinfo=getgrgid(info.st_gid);
    printf("%s %lu (%lu) %s %s %s  ",time_str,info.st_nlink,info.st_ino, userinfo->pw_name, groupinfo->gr_name , ConvierteModo2(info.st_mode));
}

void printextendedacc (struct stat info) {

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y/%m/%d-%H:%M", localtime(&info.st_atime));
    struct passwd *userinfo =getpwuid(info.st_uid);
    struct group *groupinfo=getgrgid(info.st_gid);
    printf("%s %lu (%lu) %s %s %s  ",time_str,info.st_nlink,info.st_ino, userinfo->pw_name, groupinfo->gr_name , ConvierteModo2(info.st_mode));
}


void revlist(char *tr[],bool hid,bool extended,bool acc,bool link) {
    int i, contador= 1;
    if (tr[1] == NULL) {
        cwd();
        return;
    }

    for (i = 1; tr[i] != NULL && i <= 4; i++) {
        if (strcmp(tr[i],"-hid") == 0){hid = true;contador++;}
        else if (strcmp(tr[i],"-long" ) == 0 ) {extended =true;contador++;}
        else if (strcmp(tr[i],"-acc") == 0 ) {acc = true;contador++;}
        else if (strcmp(tr[i],"-link") == 0 ) {link =true;contador++;}
    }

    struct dirent *entrada;
    DIR * dr = opendir(tr[contador]);
    int contadordr = 0;
    char **directorios = malloc(1024 * sizeof(char *));
    if (directorios == NULL) {
        perror("malloc error");
        closedir(dr);
        return;
    }


    while ((entrada = readdir(dr))!= NULL) {

        struct stat info;
        char fullPath[2048];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", tr[contador], entrada->d_name);
        if (lstat(fullPath, &info) == -1) {
            perror("lstat");
            continue;
        }
        if (!hid && S_ISDIR(info.st_mode) && entrada->d_name[0] != '.' && strcmp(entrada->d_name,".") != 0 && strcmp(entrada->d_name,"..") != 0) {
            directorios[contadordr] = malloc(strlen(fullPath) + 1);
            strcpy(directorios[contadordr],fullPath);
            contadordr++;
        }
        if (hid && S_ISDIR(info.st_mode) && strcmp(entrada->d_name,".") != 0 && strcmp(entrada->d_name,"..") != 0) {

            directorios[contadordr] = malloc(strlen(fullPath) + 1);
            strcpy(directorios[contadordr],fullPath);
            contadordr++;
        }

    }
    for (i = 0; i < contadordr; i++) {
        char aux[100];
        strcpy(aux,tr[contador]);
        strcpy(tr[contador],directorios[i]);
        revlist(tr,hid,extended,acc,link);
        strcpy(tr[contador],aux);
        free(directorios[i]);
    }
    if (i <= contadordr) {
        free(directorios);
        closedir(dr);
    }
    dr = opendir(tr[contador]);
    if(dr == NULL) {
        printf("**error al acceder a %s:No such file or directory\n",tr[contador]);
        return;
    }
    printf("***%s\n",tr[contador]);


    while ((entrada = readdir(dr))!= NULL) {
        struct stat info;
        char fullPath[2048];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", tr[contador], entrada->d_name);

        if (lstat(fullPath, &info) == -1) {
            perror("lstat");
            continue;
        }

        if(extended && entrada->d_name[0] != '.' && !acc && !hid) {
            printextend(info);

        } else if(extended && acc && entrada->d_name[0] != '.' && !hid) {
            printextendedacc(info);
        }

        if (!hid  && entrada->d_name[0] != '.' && !link ) {
            printf("%8ld %s\n",info.st_size, entrada->d_name);

        } else if (!hid && link && entrada->d_name[0] != '.') {
            if (S_ISLNK(info.st_mode)) {
                printf("%8ld %s",info.st_size, entrada->d_name);
            } else printf("%8ld %s\n",info.st_size, entrada->d_name);

        }


        if (hid) {
            if (extended && !acc) {
                printextend(info);
                printf("%8ld %s\n",info.st_size, entrada->d_name);

            } else if (extended && acc ) {

                printextendedacc(info);
                printf("%8ld %s\n",info.st_size, entrada->d_name);
            }else {
                if (link && S_ISLNK(info.st_mode)) {
                    printf("%8ld %s",info.st_size, entrada->d_name);
                } else printf("%8ld %s\n",info.st_size, entrada->d_name);
            }

            if (link && S_ISLNK(info.st_mode)) {

                char link[1024];
                ssize_t len = readlink(fullPath, link, sizeof(link) - 1);
                if (len != -1) {
                    link[len] = '\0';
                    printf(" -> %s\n", link);
                }
            }

        }

    }
    closedir(dr);
}

void reclist(char *tr[],bool hid,bool extended,bool acc,bool link) {
    int i, contador= 1;

    if (tr[1] == NULL) {
        cwd();
        return;
    }
    for (i = 1; tr[i] != NULL && i <= 4; i++) {
        if (strcmp(tr[i],"-hid") == 0){hid = true;contador++;}
        else if (strcmp(tr[i],"-long" ) == 0 ) {extended =true;contador++;}
        else if (strcmp(tr[i],"-acc") == 0 ) {acc = true;contador++;}
        else if (strcmp(tr[i],"-link") == 0 ) {link =true;contador++;}
    }

    struct dirent *entrada;
    DIR * dr = opendir(tr[contador]);

    if(dr == NULL) {
        printf("**error al acceder a %s:No such file or directory\n",tr[contador]);
        return;
    }
    printf("***%s\n",tr[contador]);
    char **directorios = malloc(1024 * sizeof(char *));
    if (directorios == NULL) {
        perror("malloc error");
        closedir(dr);
        return;
    }
    int contadordr = 0;
    while ((entrada = readdir(dr))!= NULL) {
        struct stat info;
        char fullPath[2048];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", tr[contador], entrada->d_name);

        if (lstat(fullPath, &info) == -1) {
            perror("lstat");
            continue;
        }

        if(extended && entrada->d_name[0] != '.' && !acc && !hid) {
            printextend(info);

        } else if(extended && acc && entrada->d_name[0] != '.' && !hid) {
            printextendedacc(info);
        }

        if (!hid  && entrada->d_name[0] != '.' && !link ) {
            printf("%8ld %s\n",info.st_size, entrada->d_name);

        } else if (!hid && link && entrada->d_name[0] != '.') {
            if (S_ISLNK(info.st_mode)) {
                printf("%8ld %s",info.st_size, entrada->d_name);
            } else printf("%8ld %s\n",info.st_size, entrada->d_name);

            }


        if (hid) {
            if (extended && !acc) {
                printextend(info);
                printf("%8ld %s\n",info.st_size, entrada->d_name);

            } else if (extended && acc ) {

                printextendedacc(info);
                printf("%8ld %s\n",info.st_size, entrada->d_name);
            }else {
                if (link && S_ISLNK(info.st_mode)) {
                    printf("%8ld %s",info.st_size, entrada->d_name);
                } else printf("%8ld %s\n",info.st_size, entrada->d_name);
            }



            if (S_ISDIR(info.st_mode) && strcmp(entrada->d_name,".") != 0 && strcmp(entrada->d_name,"..") != 0) {

                directorios[contadordr] = malloc(strlen(fullPath) + 1);
                strcpy(directorios[contadordr],fullPath);
                contadordr++;
            }
        }
        if (link && S_ISLNK(info.st_mode)) {
            char link[1024];
            ssize_t len = readlink(fullPath, link, sizeof(link) - 1);
            if (len != -1) {
                link[len] = '\0';
                printf(" -> %s\n", link);
            }
        }

        if (hid == false && S_ISDIR(info.st_mode) && entrada->d_name[0] != '.' && strcmp(entrada->d_name,".") != 0 && strcmp(entrada->d_name,"..") != 0) {

                directorios[contadordr] = malloc(strlen(fullPath) + 1);
                strcpy(directorios[contadordr],fullPath);
                contadordr++;
            }

        }
        closedir(dr);
        for (i = 0; i < contadordr; i++) {
            strcpy(tr[contador],directorios[i]);
            reclist(tr,hid,extended,acc,link);
            free(directorios[i]);
        }
    free(directorios);
}

void erase(char *cadena[],int trozos) {
    if(cadena[1] ==NULL) perror("No existe directorio.");
    else
        for(int i = 1; i < trozos;i++) {
            if(remove(cadena[i]) != 0) {
                perror("Error al borrar el directorio.");
            }
        }
}
void delrec(char *ruta) {
    struct stat infoDirectorio;
    struct dirent *archivo;
    DIR *directorio;
    char* directorioAuxiliar;
    size_t longitudRuta, longitudAux;
    longitudRuta = strlen(ruta);
    if (stat(ruta, &infoDirectorio) != 0) {
        perror("Error al obtener información del archivo/directorio");
        return;
    }
    if (LetraTF(infoDirectorio.st_mode)=='d') {
        if (remove(ruta) == 0) printf("Eliminado: %s\n", ruta);
        else {
            directorio = opendir(ruta);
            if (directorio == NULL) {
                perror("Error al abrir directorio");
                return;
            }
            while ((archivo = readdir(directorio))!=NULL) {
                if ((strcmp(archivo->d_name, ".") == 0 || strcmp(archivo->d_name, "..") == 0 )) continue;
                longitudAux = strlen(archivo->d_name);
                directorioAuxiliar = malloc(longitudRuta + longitudAux + 2);
                snprintf(directorioAuxiliar,longitudRuta + longitudAux + 2,"%s/%s",ruta,archivo->d_name);
                delrec(directorioAuxiliar);
                free(directorioAuxiliar);
            }
            closedir(directorio);
            delrec(ruta);
        }
    } else if(remove(ruta)==0){
        printf("Eliminado: %s\n", ruta);
        return;
    } else perror("Fichero o directorio invalido.");
}


void delrec_aux(char *cadena[]) {
    if (cadena[1]==NULL) {
        perror("Directorio no válido");
        return;
    }
    for (int i=1;cadena[i]!=NULL && strcmp(cadena[i],"\0")!=0;i++) delrec(cadena[i]);
}

/*---------------------------------------------------------------------P2----------------------------------------------------------------------*/

void *cadtop(char *cadena) {
    unsigned long int dir = strtoull(cadena, NULL, 16);
    return (void*)dir;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, listM *memList){
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); /* si no es crear, necesitamos el tamano, que es s.shm_segsz*/
    /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    struct tItemM node;
    strcpy(node.allocationtype,"shared");
    node.key = clave;
    node.memory_adress = p;
    node.size = s.shm_segsz;
    node.time = time(NULL);
    strcpy(node.name,"");
    node.filedescriptor = -1;
    insertItemM(node,lastM(*memList),memList);
    return (p);
}

void imprimirLista(listM *L) {
    posM p;
    printf("***Lista de bloques asignados para el proceso %d\n",getpid());
    for (p = *L; p != MNULL; p = p->next) {
        struct tm tm = *localtime(&p->data.time);
        if(strcmp(p->data.allocationtype,"malloc")==0) {
            //printf("***Lista de bloques asignados malloc para el proceso %d\n",getpid());
            printf("%p\t%d\t%d %d %02d:%02d\t%s\n", p->data.memory_adress,p->data.size,tm.tm_mon+1,tm.tm_mday, tm.tm_hour,tm.tm_min,p->data.allocationtype);
        }else if(strcmp(p->data.allocationtype,"mmap")==0) {
            //printf("***Lista de bloques asignados mmap para el proceso %d\n",getpid());
            printf("%p\t%d\t%d %d %02d:%02d\t%s\t (descriptor %d)\n", p->data.memory_adress,p->data.size,tm.tm_mon+1,tm.tm_mday, tm.tm_hour,tm.tm_min,p->data.name,p->data.filedescriptor);
        }else if (strcmp(p->data.allocationtype,"createshared")==0) {
            //printf("***Lista de bloques asignados shared para el proceso %d\n",getpid());
            printf("%p\t%d\t%d %d %02d:%02d\t%s\t (key %d)\n", p->data.memory_adress,p->data.size,tm.tm_mon+1,tm.tm_mday, tm.tm_hour,tm.tm_min,p->data.allocationtype,p->data.key);
        }else if (strcmp(p->data.allocationtype,"shared")==0) {
            //printf("***Lista de bloques asignados shared para el proceso %d\n",getpid());
            printf("%p\t%d\t%d %d %02d:%02d\t%s\t (key %d)\n", p->data.memory_adress,p->data.size,tm.tm_mon+1,tm.tm_mday, tm.tm_hour,tm.tm_min,p->data.allocationtype,p->data.key);

        }
    }
}



void * MapearFichero (char *cadena[], int protection, listF *l, listM *memlist)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(cadena[2],&s)==-1 || (df=open(cadena[2], modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
    AnadirFicherosAbiertos(df,modo,l, cadena);
    struct tItemM node;
    strcpy(node.allocationtype,"mmap");
    node.memory_adress = p;
    strcpy(node.name,cadena[2]);
    node.filedescriptor = df;
    node.size = s.st_size;
    node.time = time(NULL);
    node.key = -1;

    insertItemM(node,lastM(*memlist),memlist);


    return p;
}

void do_AllocateMmap(char *arg[],listM *L,listF *fileList){
    char *perm;
    void *p;
    int protection=0;

    if (arg[1]==NULL || arg[2]==NULL){
        imprimirLista(L);
        return;
    }
    if ((perm=arg[1])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MapearFichero(arg,protection,fileList,L))==NULL)
        perror ("Imposible mapear fichero");
    else
        printf ("fichero %s mapeado en %p\n", arg[2], p);
}

void do_AllocateCreateshared (char *tr[],listM *L){
    key_t cl;
    size_t tam;
    void *p;

    if (tr[1]==NULL || tr[2]==NULL) {
        imprimirLista(L);
        return;
    }

    cl=(key_t)  strtoul(tr[2],NULL,10);
    tam=(size_t) strtoul(tr[3],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam,L))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


void do_AllocateShared (char *tr[],listM *L){
    key_t cl;
    void *p;


    if (tr[1]==NULL || tr[2] == NULL) {
        imprimirLista(L);
        return;
    }

    cl=(key_t)  strtoul(tr[2],NULL,10);

    if ((p=ObtenerMemoriaShmget(cl,0,L))!=NULL)
        printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


void do_allocateMalloc(char *tr[], listM *L) {
    if (tr[1] == NULL || tr[2] == NULL) {
        imprimirLista(L);
        return;
    }
    int size = atoi(tr[2]);
    if (size < 0) {
        printf("Imposible obtener memoria con malloc: Cannot allocate memory\n");
    } else if (size == 0) {
        printf("No se asignan bloques de 0 bytes\n");
    } else {
        void *puntero = malloc(size);
        if (puntero != NULL) {
            struct tItemM node;
            node.memory_adress = puntero;
            node.size = size;
            strcpy(node.allocationtype, "malloc");
            node.time = time(NULL);
            node.filedescriptor = -1;
            strcpy(node.name,"");
            node.key = -1;

            if (insertItemM(node, lastM(*L), L)) {
                printf("Asignados %d bytes en %p\n", size, puntero);
            } else {
                free(puntero);
                printf("Error al insertar en la lista de memoria\n");
            }
        } else {
            printf("Error en malloc\n");
        }
    }
}


void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid (pid,NULL,0);
 }


void Recursiva (int n){
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

void recursiva (char *cadena[]){

    if (cadena[1]==NULL) {
        return;
    }
    int n = atoi(cadena[1]);
    if (n >= 2048) {
        printf("Introduce un valor menor a 2048\n");
        return;
    }
    if (n<0) {
        printf("Introduce un valor mayor o igual a 0\n");
    }
    Recursiva(n);


}


void allocate(char *trozos[],int num_trozos,listM *L,listF *ficheros) {
    if (num_trozos == 1) {
        imprimirLista(L);
        return;
    }
    if (strcmp(trozos[1],"-malloc")==0){
            do_allocateMalloc(trozos,L);

    }else if (strcmp(trozos[1],"-shared")==0) {
            do_AllocateShared(trozos,L);
    }else if (strcmp(trozos[1],"-createshared")==0) {
            do_AllocateCreateshared(trozos,L);
    }else if (strcmp(trozos[1],"-mmap")==0) {
            do_AllocateMmap(trozos,L,ficheros);
    }
}


void do_DeallocateDelkey (char *args[]){
    key_t clave;
    int id;
    char *key=args[2];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}



void deallocate(char *args[], listM *memList) {
    if (args[1] == NULL) {
        imprimirLista(memList);
        return;
    }
    struct tItemM c = {0};

    if (strcmp(args[1], "-malloc") == 0) {
        if (args[2] == NULL) {
            imprimirLista(memList);
            return;
        }
        strcpy(c.allocationtype, "malloc");
        c.size = atoi(args[2]);
    } else if (strcmp(args[1], "-mmap") == 0) {
        if (args[2] == NULL) {
            imprimirLista(memList);
            return;
        }
        strcpy(c.allocationtype, "mmap");
        strncpy(c.name, args[2], sizeof(c.name) - 1);
        c.name[sizeof(c.name) - 1] = '\0';
    } else if (strcmp(args[1], "-shared") == 0) {
        if (args[2] == NULL) {
            imprimirLista(memList);
            return;
        }
        strcpy(c.allocationtype, "shared");
        c.key = atoi(args[2]);
    } else if (strcmp(args[1], "-delkey") == 0) {
        if (args[2] == NULL) {
            imprimirLista(memList);
            return;
        }
        do_DeallocateDelkey(args);
        return;
    } else {
        c.memory_adress = cadtop(args[1]);
    }

    posM p = findItemM(c, memList);
    if (p != NULL) {
        if (strcmp(p->data.allocationtype, "malloc") == 0) {
            free(p->data.memory_adress);
        } else if (strcmp(p->data.allocationtype, "mmap") == 0) {
            if (munmap(p->data.memory_adress, p->data.size) == -1) {
                perror("Error al hacer unmap del fichero");
                return;
            }
        } else if (strcmp(p->data.allocationtype, "shared") == 0 ||
                   strcmp(p->data.allocationtype, "createshared") == 0) {
            if (shmdt(p->data.memory_adress) == -1) {
                perror("Error al desasociar memoria compartida");
                return;
            }
        }
        deleteAtPositionM(p, memList);
        printf("Memoria liberada correctamente\n");
    } else {
        printf("No se encontró el bloque de memoria especificado\n");
    }
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void memdump (char *args[]) {
    if (args[1] == NULL) {
        return;
    }
    void* p = cadtop(args[1]);
    unsigned char *arr=(unsigned char *) p;
    size_t cont;
    if (args[2]==NULL) {
        cont = 25;
    } else { cont = atoi(args[2]);}

    printf("Volcando %zu bytes desde la direccion %p\n",cont,p);
    size_t i = 0;char c;
    for ( i = 0; i < cont; i++) {
        c = arr[i];
        if (c >= 32 && c <= 126) {
            printf("%2c ", arr[i]);
        }else printf(" ");

    }
    printf("\n");

    for ( i = 0; i < cont; i++) {
        printf("%02x ", arr[i]);
    }
    printf("\n");

}

void memfill(char *tr[], listM * memList) {
    size_t size;
    unsigned char caracter;
    struct tItemM elementoM;
    posM posicion;
    if (tr[0]!= NULL && tr[1]!=NULL && tr[2]!=NULL && tr[3]!=NULL) {
        void *direccion = (void*)cadtop(tr[1]);
        size = (size_t) atoi (tr[2]);
        if (tr[3][0] == '\'' && tr[3][2] == '\'' && tr[3][3] == '\0') {
            caracter = tr[3][1];
        }else {
            caracter = (unsigned char) atoi(tr[3]);
        }

        if (size <= 0) {
            perror("Imposible llenar la memoria");
            return;
        }
        elementoM.memory_adress = direccion;
        posicion = findItemM(elementoM,memList);
        if (posicion==NULL) {
            printf("La direccion de memoria no esta en la lista\n");
            return;
        }

        LlenarMemoria(direccion,size,caracter);
        printf("Llenando %zu bytes de memoria con el byte  %c (%02X) a partir de la direccion %p\n",
                size,caracter,caracter,direccion);
    }

}




int memory(int num_trozos, char *trozos[],listM L) {
    int x1=0,y1=0,z1=0;
    static int x2 = 0,y2=0,z2=0;
    static int x3,y3,z3;

    if(num_trozos > 1 && strcmp(trozos[1],"-funcs")==0) {
        printf("Funciones de programa: \t%p, %p, %p\n",pid,ppid,date);
        printf("Funciones de libreria: \t%p, %p, %p\n",strcmp,strlen,malloc);
        return 0;
    }
    if (num_trozos > 1 && strcmp(trozos[1],"-vars")==0) {
        printf("Variables locales: \t%p, %p, %p\n",&x1,&y1,&z1);
        printf("Variables globales: \t%p, %p, %p\n",&global,&global1,&global2);
        printf("Var (N.I.) globales :\t%p, %p, %p\n",&global3,&global4,&global5);
        printf("Variables estaticas: \t%p, %p, %p\n",&x2,&y2,&z2);
        printf("Var (N.I.) estaticas: t%p, %p, %p\n",&x3,&y3,&z3);
       return 0;
    }
    if (num_trozos > 1 && strcmp(trozos[1],"-blocks")==0) {
        imprimirLista(&L);
        return 0;
    }
    if(num_trozos > 1 && strcmp(trozos[1],"-all")==0) {
        printf("Variables locales: \t%p, %p, %p\n",&x1,&y1,&z1);
        printf("Variables globales: \t%p, %p, %p\n",&global,&global1,&global2);
        printf("Var (N.I.) globales :\t%p, %p, %p\n",&global3,&global4,&global5);
        printf("Variables estaticas: \t%p, %p, %p\n",&x2,&y2,&z2);
        printf("Var (N.I.) estaticas: t%p, %p, %p\n",&x3,&y3,&z3);
        printf("Funciones de programa: \t%p, %p, %p\n",pid,ppid,date);
        printf("Funciones de libreria: \t%p, %p, %p\n",strcmp,strlen,malloc);
        imprimirLista(&L);
        return 0;
    }if (num_trozos > 1 && strcmp(trozos[1],"-pmap")==0) {
        Do_pmap();
    }
    if (num_trozos == 1) {
        printf("Variables locales: \t%p, %p, %p\n",&x1,&y1,&z1);
        printf("Variables globales: \t%p, %p, %p\n",&global,&global1,&global2);
        printf("Var (N.I.) globales :\t%p, %p, %p\n",&global3,&global4,&global5);
        printf("Variables estaticas: \t%p, %p, %p\n",&x2,&y2,&z2);
        printf("Var (N.I.) estaticas: \t%p, %p, %p\n",&x3,&y3,&z3);
        printf("Funciones de programa: \t%p, %p, %p\n",pid,ppid,date);
        printf("Funciones de libreria: \t%p, %p, %p\n",strcmp,strlen,malloc);
        imprimirLista(&L);
        return 0;
    }
    return 0;
}

ssize_t LeerFichero (char *f, void *p, size_t cont){
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

ssize_t EscribirFichero(int df, void *p, size_t cont) {
    ssize_t n;
    int aux;
    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    return n;
}

ssize_t EscribirFichero2(char *f, void *p,size_t cont, int sobreescritura) {
    ssize_t n;
    int df,aux,flag;

    if (sobreescritura) flag = O_CREAT|O_WRONLY|O_TRUNC;
    else flag = O_CREAT|O_WRONLY|O_APPEND;

    if ((df=open(f,flag,0777)) == -1) return -1;
    if ((n = write(df,p,cont))==-1) {
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close(df);
    return n;
}


ssize_t Leerdescriptor(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t  n;
    int df = atoi(f),aux;

    if (fstat (df,&s)==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void Cmd_ReadFile (char *ar[]){
    void *p;
    size_t cont=-1;  /*si no pasamos tamano se lee entero */
    ssize_t n;
    if (ar[1]==NULL || ar[2]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[2]);
    if (ar[3]!=NULL)
        cont=(size_t) atoll(ar[3]);

    if ((n=LeerFichero(ar[1],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void Cmd_WriteFile(char *ar[]) {
    void *p;
    size_t cont;
    ssize_t n;
    char *fichero;
    int sobreescritura=0,posicion = 1;

    if (ar[1]==NULL) {
        printf ("faltan parametros\n");
        return;
    }
    if (strcmp(ar[1],"-o")==0) {
        sobreescritura = 1; posicion = 2;
    }

    if (ar[posicion]==NULL || ar[posicion+1]==NULL || ar[posicion+2]==NULL) {
        printf ("faltan parametros\n");
        return;
    }

    fichero=ar[posicion];
    p = cadtop(ar[posicion+1]);
    cont = (size_t) atoll(ar[posicion+2]);

    if ((n=EscribirFichero2(fichero,p,cont,sobreescritura))==-1) perror("Imposible la escritura del fichero");
    else printf("escritos %lld bytes en %s desde %p\n",(long long)n,fichero,p);

}

void Cmd_Read(char *ar[]) {
    void *p;
    size_t cont=-1;  /*si no pasamos tamano se lee entero */
    ssize_t n;
    if (ar[1]==NULL || ar[2]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[2]);
    if (ar[3]!=NULL)
        cont=(size_t) atoll(ar[3]);

    if ((n=Leerdescriptor(ar[1],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void Cmd_Write(char *ar[]) {
    void *p;
    size_t cont;
    ssize_t n;
    int df;

    if (ar[1]==NULL || ar[2]==NULL || ar[3]==NULL) {
        printf ("faltan parametros\n");
        return;
    }
    df = atoi(ar[1]);
    p = cadtop(ar[2]);
    cont = (size_t) atoll(ar[3]);

    if ((n = EscribirFichero(df,p,cont))==-1) perror("Imposible la escritura en el fichero");
    else printf("escritos %lld bytes en %d desde %p\n",(long long)n,df,p);

}

/*-----------------------------------------------------------------P3----------------------------------------------------------------------------*/

char *cambiarLogin(uid_t uid) {
    struct passwd *login;
    if ((login = getpwuid(uid))==NULL) {
        return "inexistente";
    }
     return login->pw_name;
}

uid_t cambiarUID(char *id) {
    struct passwd *login;
    if ((login = getpwnam(id)) == NULL) {
        return (uid_t) -1;
    }
    return login->pw_uid;
}

void Cmd_getuid() {
    printf("Credencial real: %d , (%s)\n",getuid(),cambiarLogin(getuid()));
    printf("Credencial efectiva: %d, (%s)\n",geteuid(),cambiarLogin(geteuid()));
}


void Cmd_setuid(char *tr[], int num_trozos){
    uid_t id;
    char *userLogin;

    if (num_trozos < 2) {
        Cmd_getuid();
        return;
    }
    if (strcmp(tr[1],"-l")==0) {
        if (tr[2] != NULL) {
            userLogin = tr[2];
            id = cambiarUID(userLogin);
            if (id == -1) printf("Usuario no existente %s\n",userLogin);
            else if (setuid(id)==-1)
                perror("Imposible cambiar credencial: Operation not permitted");
        }
        else Cmd_getuid();
    }
}


void Cmd_fork (char *tr[]){
    pid_t pid;

    if ((pid=fork())==0){
        printf ("ejecutando proceso %d\n", getpid());
    }
    else if (pid!=-1)
        waitpid (pid,NULL,0);
}


int BuscarVariable (char * var, char *e[])  {
    int pos=0;
    char aux[MAXVAR];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;
    return(-1);
}

void mostrarEntorno(char **entorno, char * nombre_entorno) {
    int i = 0;

    while (entorno[i] != NULL) {
        printf("%p -> %s[%d] = (%p) %s\n",entorno[i],nombre_entorno,i,entorno[i],entorno[i]);
        i++;
    }
}

void Cmd_environ(char *tr[],int num_trozos, char **env) {
    if (num_trozos == 1) {
        mostrarEntorno(env,"environ");
        return;
    }
    if (strcmp(tr[1],"-environ") == 0) {
        mostrarEntorno(env,"environ");
    }else if (strcmp(tr[1],"-addr") == 0) {
        printf("environ: %p (almacenado en %p)\n",environ,&environ);
        printf("main arg3: %p (almacenado en %p)\n",env,&env);
    }
}

void showvar(char *tr[], char *entorno[]) {
    int p;
    if (tr[1]==NULL) {
        mostrarEntorno(entorno,"main arg3");
    }else {
        p = BuscarVariable(tr[1],entorno);
        if (p==-1 ) {
            perror("Error : La variable no existe");
        }
        else {
            printf("Con arg3 main %s %p (@%p)\n", entorno[p], entorno[p], &entorno[p]);
            printf("Con environ %s %p (@%p)\n", entorno[p], entorno[p], &entorno[p]);
            printf("Con getenv %s (%p)\n", getenv(tr[1]), getenv(tr[1]));
        }
        if (tr[2] != NULL) {
            p = BuscarVariable(tr[2],entorno);
            if (p==-1 ) {
                perror("Error : La segunda variable no existe");
            }else {
                printf("Con arg3 main %s %p (@%p)\n", entorno[p], entorno[p], &entorno[p]);
                printf("Con environ %s %p (@%p)\n", entorno[p], entorno[p], &entorno[p]);
                printf("Con getenv %s (%p)\n", getenv(tr[2]), getenv(tr[2]));

            }
        }
    }
}

int CambiarVariable(char * var, char * valor, char *e[]) {
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);

    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

int CambiarVariable2(char * v1, char *v2,char * valor, char *e[]) {
    int pos;
    char *aux;

    if ((pos=BuscarVariable(v1,e))==-1)
        return(-1);
    if (BuscarVariable(v2,e) != -1) {
        errno = EEXIST;
        return (-1);
    }

    if ((aux=(char *)malloc(strlen(v1)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,v2);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}


void changevar(char *tr[],int num_trozos,char **env) {
    if (num_trozos < 3) {
        printf("Uso: changevar [-a|-e|-p] var valor\n");
        return;
    }
    char *var = tr[2]; char *valor = tr[3];

    if (strcmp(tr[1],"-a")==0) {
       if (CambiarVariable(var,valor,env) == -1)
           perror("Imposible cambiar variable: No such file or directory");
    }else if (strcmp(tr[1],"-e")==0) {
        if (CambiarVariable(var,valor,environ) == -1)
            perror("Imposible cambiar variable: No such file or directory");
    }else if (strcmp(tr[1],"-p")==0) {
        char *aux=NULL;
        if ((aux = (char *) malloc(strlen(var) + strlen(valor) + 2)) == NULL) exit(0);
        strcpy(aux, var);
        strcat(aux, "=");
        strcat(aux, (valor));
        if (putenv(aux) != 0) perror("Imposible cambiar variable");
    }
    else {
        printf("Uso: changevar [-a|-e|-p] var valor\n");
    }

}

void subsvar(char *tr[], int num_trozos, char **env) {
    if (num_trozos < 3) {
        printf("Uso: subsvar [-a|-e] var valor\n");
        return;
    }else {
        if (strcmp(tr[1],"-a")==0) {
            if (CambiarVariable2(tr[2],tr[3],tr[4],env) == -1)
                printf("Imposible sustituir variable %s por %s: %s\n",tr[2],tr[3],strerror(errno));
        }else if (strcmp(tr[1],"-e")==0) {
            if (CambiarVariable2(tr[2],tr[3],tr[4],environ) == -1)
                printf("Imposible sustituir variable %s por %s: %s\n",tr[2],tr[3],strerror(errno));
        }
        else
            printf("Uso: subsvar [-a|-e] var valor\n");
    }
}


void inicializarRutaBusqueda(listD *rutaBusqueda) {
    if (rutaBusqueda != NULL) {
        createListD(rutaBusqueda);
    }
}

void printearPath(listD *rutaBusqueda) {
    char *path = getenv("PATH");
    if (path != NULL) {
        char *pathAux = strdup(path);
        char *trozo = strtok(pathAux,":");
        int contador = 0;

        while (trozo != NULL) {
            struct tItemD item;
            item.dir = strdup(trozo);
            insertItemD(item,NULL,rutaBusqueda);
            trozo = strtok(NULL,":");
            contador++;
        }
        free(pathAux);
        printf("Importados %d directorios en la ruta de busqueda\n",contador);
    }
    else {
        printf("Imposible obtener la ruta de busqueda\n");
    }
}

void Cmd_search(char *tr[],listD *rutaBusqueda) {
    if (rutaBusqueda == NULL) {
        perror("Error: Lista no inicializada");
        return;
    }
    if (tr[1] == NULL) {
        posD p;
        for (p = firstD(*rutaBusqueda); p != NULL; p = p->next) {
            printf("%s\n", p->data.dir);
        }
        return;
    }
    if (strcmp(tr[1], "-add") == 0 && tr[2] != NULL) {
        struct tItemD elemento;
        elemento.dir = strdup(tr[2]);
        if (elemento.dir != NULL) {
            insertItemD(elemento, NULL, rutaBusqueda);
            printf("Importado directorio %s en la ruta de busqueda.\n", tr[2]);
        }else {
            perror("Error: El directorio no existe");
        }
    } else if (strcmp(tr[1], "-del") == 0 && tr[2] != NULL) {
        posD p;
        for (p = firstD(*rutaBusqueda); p != NULL; p = p->next) {
            if (strcmp(p->data.dir, tr[2])==0) {
                deleteAtPositionD(p,rutaBusqueda);
                printf("Directorio %s eliminado en la ruta de busqueda\n",tr[2]);
                return;
            }
        }
        printf("El directorio %s no esta importado en la ruta de busqueda\n",tr[2]);
    } else if (strcmp(tr[1], "-clear") == 0) {
        freeListD(rutaBusqueda);
        createListD(rutaBusqueda);
        printf("Ruta de busqueda vacia\n");
    } else if (strcmp(tr[1], "-path") == 0) {
        printearPath(rutaBusqueda);
    } else {
            printf("No se puede obtener la ruta de busqueda\n");
        }
}


char * Ejecutable (char *s, listD d){
    static char path[MAXNAME];
    struct stat st;
    char *p;
    posD auxc;

    if (s==NULL || (auxc=firstD(d))==NULL)
        return s;
    if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;
    p =auxc->data.dir;
    strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);
    if (lstat(path,&st)!=-1)
        return path;
    while ((auxc=nextD(auxc)) !=NULL){
        p = auxc->data.dir;
        strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);

        if (lstat(path,&st)!=-1)
            return path;
    }
    return s;
}

int Execpve(char *tr[], char **NewEnv, int * pprio, listD d, char *argv[])
{
    char *p; /*NewEnv contains the address of the new environment*/
    /*pprio the address of the new priority*/
    /*NULL indicates no change in environment and/or priority*/
    if (tr[0]==NULL || (p=Ejecutable(tr[0],d))==NULL){
        errno=EFAULT;
        return-1;
    }
    if (pprio !=NULL && setpriority(PRIO_PROCESS,getpid(),*pprio)==-1 && errno){
        printf ("Imposible cambiar prioridad: %s\n",strerror(errno));
        return -1;
    }
    if (NewEnv==NULL && NewEnv == NULL)
        return execv (p,argv);
    else
        return execve (p, tr, NewEnv);
}



void execpri(char *tr[], listD d) {

    int i=1;int cntvariables = 0;
    char *argv[20];
    if (tr[2] ==NULL) {
        perror ("Imposible cambiar prioridad: %s\n");
        return;
    }

    for (i = 2; BuscarVariable(tr[i],environ) != -1; i++) {
        cntvariables ++;

    }
    char *newEnv[cntvariables+1];
    for (i = 0; i < cntvariables; i++){
        newEnv[i] = tr[i+2];
    }
    newEnv[i] = NULL;

    for (i = 0; tr[i+cntvariables] != NULL; i++){
        argv[i] = tr[i+cntvariables+2];

    }
    argv[i] = NULL;
    int pprio = atoi(tr[1]);
    if (cntvariables >= 1){

        Execpve(argv,newEnv, &pprio, d,argv);
    }else Execpve(argv,NULL, &pprio, d,argv);



}



void exec(char *tr[], listD d){
    int i=1;int cntvariables = 0;
    char *argv[20];


    for (i = 1; BuscarVariable(tr[i],environ) != -1; i++) {
        cntvariables ++;

    }

    char *newEnv[cntvariables+1];
    for (i = 0; i < cntvariables; i++) {
        newEnv[i] = tr[i+1];
    }
    newEnv[i] = NULL;

    for (i = 0; tr[i+cntvariables+1] != NULL; i++){
        argv[i] = tr[i+cntvariables+1];

    }
    argv[i] = NULL;

    if (tr[2] != NULL && tr[3] != NULL) {

        Execpve(argv,newEnv, NULL, d,argv);
    }else if (tr[2] != NULL && tr[3] == NULL){
        Execpve(argv,newEnv, NULL, d,argv);
    } else Execpve(argv,NULL, NULL, d,argv);


}

void fg(char *tr[], listD d, listP procesos) {

    pid_t pid = fork();

    if (pid==0){
        char *aux[20];
        aux[0] = "exec";
        int i;
        for (i = 1; tr[i]!=NULL ; i++) {
            aux[i] = tr[i];
        }
        aux[i] = NULL;
        printf ("ejecutando proceso %d\n", getpid());
        exec(aux,d);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    else if (pid!=-1) {
        waitpid (pid,NULL,0);

    }
}

void fgpri(char *tr[], listD d, listP procesos) {

    pid_t pid = fork();
    char *aux[20];

    if (pid==0){
        aux[0] = "execpri";
        int i;
        for (i = 1; tr[i]!=NULL ; i++) {
            aux[i] = tr[i];
        }
        aux[i] = NULL;
        printf ("ejecutando proceso %d\n", getpid());
        execpri(aux,d);
        perror("exec failed");
        exit(EXIT_FAILURE);
    }
    else if (pid!=-1) {
        waitpid (pid,NULL,0);
    }
}


void back(char *tr[],listP *procList, listD *dirList){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        if (Execpve(&tr[1],NULL,NULL,*dirList,&tr[1]) == -1) {
            perror("execpve");
            exit(1);
        }
    }
    if (pid > 0) {
        struct tItemP proceso;
        proceso.pid = pid;
        strncpy(proceso.command_line,tr[1],MAXNAME-1);
        proceso.command_line[MAXNAME-1]='\0';
        proceso.status = "RUNNING";

        insertItemP(proceso,lastP(*procList),procList);
        }
}

void backpri(char *tr[],listP *procList, listD *dirList) {
    int prioridad;
    pid_t pid;

    prioridad = atoi(tr[1]);
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        if (setpriority(PRIO_PROCESS,0,prioridad) == -1) {
            perror("setpriority");
            exit(1);
        }

        if (Execpve(&tr[2],NULL,NULL,*dirList,&tr[2]) == -1) {
            perror("execpve");
            exit(1);
        }
    }

    if (pid > 0) {
        struct tItemP proceso;
        proceso.pid = pid;
        snprintf(proceso.command_line,MAXNAME,"%s %s", tr[1],tr[2]);
        proceso.status = "RUNNING";
        insertItemP(proceso,lastP(*procList),procList);
    }

}

/*void estadosSenal(posP p, listP *procList) {
    int estado = 0;
    struct tItemP item = getItemP(p,procList);

    if (waitpid(item.pid,&estado,WNOHANG|WUNTRACED|WCONTINUED) == item.pid) {
        if (WIFEXITED(estado)) {
            strcpy (item.status,"TERMINADO");
            strcpy(item.senal,"000");
        }else if (WIFSIGNALED(estado)) {
            strcpy (item.status,"SENALADO");
            strcpy(item.senal,NombreSenal(WTERMSIG(estado)));
        }else if (WIFSTOPPED(estado)) {
            strcpy (item.status,"PARADO");
            strcpy(item.senal,NombreSenal(WSTOPSIG(estado)));
        }else if (WIFCONTINUED(estado)) {
            strcpy (item.status,"ACTIVO");
            strcpy(item.senal,"000");
        }
    }
    updateItemP(item,p,procList);
}*/


/*void printearProcesos(posP p, listP *procList) {
    estadosSenal(p,procList);
    struct tItemP item = getItemP(p,procList);

    printf("%d %s p =%d %ld %s (%s) %s\n",item.pid,cambiarLogin(getuid()),getpriority(PRIO_PROCESS,item.pid),item.date_and_time,item.status,item.senal,item.command_line);

}

void listjobs(listP *procList) {
    posP p;

    for (p = firstP(*procList); p != NULL; p = nextP(p,procList))
        printearProcesos(p,procList);
}*/



void procesarEntrada (char *cadena,bool *ended,list *historico,listF *ficheros,listM *memList,listP *procList, listD *dirList,bool insert) {
    char *trozos[20];
    struct tItemC comando;
    strcpy(comando.comando,cadena);
    int cant_trozos = trocearCadena(cadena,trozos);
    if (insert) {
        insertItem(comando,last(*historico),historico);

    }
    insert = true;

    if (cant_trozos >0) {
        if (strcmp(trozos[0], "authors") == 0) {
            authors(trozos,cant_trozos);
        }else if (strcmp(trozos[0], "pid") == 0) {
            pid(trozos,cant_trozos);
        }else if (strcmp(trozos[0],"ppid")==0) {
            ppid(trozos,cant_trozos);
        } else if(strcmp(trozos[0],"quit") == 0 || strcmp(trozos[0],"bye") == 0 || strcmp(trozos[0],"exit") == 0) {
            quit(ended);
        }else if (strcmp(trozos[0],"date") == 0) {
            date(trozos,cant_trozos);
        }else if(strcmp(trozos[0], "infosys") == 0) {
            infosys();
        }else if(strcmp(trozos[0],"cd") == 0) {
            cd(trozos);
        }else if (strcmp(trozos[0],"historic")==0) {
            historic(trozos,historico,ficheros, memList,procList,dirList);
        }else if (strcmp(trozos[0],"open") == 0) {
            Cmd_open(trozos,ficheros);
        }else if (strcmp(trozos[0],"close") == 0) {
            Cmd_close(trozos,ficheros);
        } else if (strcmp(trozos[0],"dup") == 0) {
            Cmd_dup(trozos,ficheros);
        } else if (strcmp(trozos[0],"makedir") == 0){
            makedir(trozos,cant_trozos);
        } else if(strcmp(trozos[0],"cwd") == 0) {
            cwd();
        }else if (strcmp(trozos[0],"makefile")==0) {
            makefile(trozos,cant_trozos);
        }else if (strcmp(trozos[0],"listdir") == 0) {
            listdir(trozos);
        }else if (strcmp(trozos[0],"erase") == 0) {
            erase(trozos,cant_trozos);
        }else if (strcmp(trozos[0],"write")==0) {
            Cmd_Write(trozos);
        }else if (strcmp(trozos[0],"delrec") == 0) {
            delrec_aux(trozos);
        }else if (strcmp(trozos[0],"listfile") == 0) {
            listfile (trozos);
        }else if (strcmp(trozos[0],"reclist") == 0) {
            reclist(trozos,false,false,false,false);
        }else if (strcmp(trozos[0],"revlist") == 0) {
            revlist(trozos,false,false,false,false);
        }else if (strcmp(trozos[0],"writefile")==0) {
            Cmd_WriteFile(trozos);
        }else if(strcmp(trozos[0], "readfile") == 0) {
            Cmd_ReadFile(trozos);
        }else if(strcmp(trozos[0], "allocate") == 0) {
            allocate(trozos,cant_trozos,memList, ficheros);
        }else if(strcmp(trozos[0], "deallocate") == 0) {
            deallocate(trozos,memList);
        }else if (strcmp(trozos[0],"write")==0) {
            Cmd_Write(trozos);
        }else if(strcmp(trozos[0], "read") == 0) {
            Cmd_Read(trozos);
        }else if (strcmp(trozos[0],"memfill")==0) {
            memfill(trozos,memList);
        }else if (strcmp(trozos[0],"memory")==0) {
            memory(cant_trozos,trozos,*memList);
        }else if (strcmp(trozos[0],"memdump")==0) {
            memdump(trozos);
        }else if (strcmp(trozos[0],"getuid")==0) {
            Cmd_getuid();
        }else if (strcmp(trozos[0],"setuid")==0) {
            Cmd_setuid(trozos,cant_trozos);
        }else if (strcmp(trozos[0],"fork")==0) {
            Cmd_fork(trozos);
        }else if (strcmp(trozos[0],"search")==0) {
            Cmd_search(trozos,dirList);
        }else if (strcmp(trozos[0],"changevar")==0) {
            changevar(trozos,cant_trozos,environ);
        }else if (strcmp(trozos[0],"subsvar")==0) {
            subsvar(trozos,cant_trozos,environ);
        }else if (strcmp(trozos[0],"showvar")==0) {
            showvar(trozos,environ);
        }else if (strcmp(trozos[0],"back")==0) {
            back(trozos,procList,dirList);
        }else if (strcmp(trozos[0],"backpri")==0) {
            backpri(trozos,procList,dirList);
        }else if (strcmp(trozos[0],"exec")==0) {
            exec(trozos,*dirList);
        }else if (strcmp(trozos[0],"environ")==0) {
            Cmd_environ(trozos,cant_trozos,environ);
        }else if (strcmp(trozos[0],"listjobs")==0) {
            //listjobs(procList);
        }else if (strcmp(trozos[0],"execpri")==0) {
            execpri(trozos,*dirList);
        }else if (strcmp(trozos[0],"fg")==0) {
            fg(trozos,*dirList,*procList);
        }else if (strcmp(trozos[0],"fgpri")==0) {
            fgpri(trozos,*dirList,*procList);
        }else if (strcmp(trozos[0],"recurse")==0) {
            recursiva(trozos);
        }else if (strcmp(trozos[0],"help") == 0) {
            if(cant_trozos > 1) {
                help (cant_trozos,trozos[1]);
            }else {
                help (cant_trozos,NULL);
            }
        }else printf("Comando no válido");
    }
}









