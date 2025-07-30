# Nombre del ejecutable
TARGET = p3

# Archivos fuente
SRCS = ficheros.c comandos.c p3.c listas.c listaMemoria.c listaProcesos.c listaDirectorios.c
#Archivos Cabecera
HEADERS = ficheros.h listas.h comandos.h listaMemoria.h listaProcesos.h listaDirectorios.h
# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Compilador
CC = gcc

# Flags del compilador
CFLAGS = -Wall  -g

# Regla para generar el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar los archivos .c a .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)