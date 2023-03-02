/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Coppola, Valentino
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

//INCLUDES DE LIBRERIAS
#include <stdio.h>                  // printf(), fprintf(), stderr, stdout, stdin
#include <fcntl.h>                  // O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h>               // S_IRUSR, S_IWUSR
#include <stdlib.h>                 // exit(), EXIT_SUCCESS, EXIT_FAILURE, atoi()
#include <unistd.h>                 // SEEK_SET, read(), write(), open(), close(), lseek()
#include <errno.h>                  // errno
#include <string.h>                 // strerror()

//DECLARACIONES CONSTANTES
#define BLOCKSIZE 1024              // bytes
#define EXITO 0                     // gestión errores
#define FALLO -1                    // gestión errores

//DECLARACION FUNCIONES
int bmount(const char *camino);
int bumount();
int bwrite(unsigned int nbloque, const void *buf);
int bread(unsigned int nbloque, void *buf);
