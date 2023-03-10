/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Coppola, Valentino
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

#include "bloques.h"

int descriptor;

int bmount(const char *camino) {
    umask(000);
    descriptor = open(camino, O_RDWR | O_CREAT, 0666);
    if (descriptor == FALLO) { //error
        perror("Error: bmount open");
    }

    return descriptor;
}

int bumount() {
    if (close(descriptor) == FALLO) { //error
        perror("Error: bumount close");
        return FALLO;
    } else {
        return EXITO;
    }
}

int bwrite(unsigned int nbloque, const void *buf) {
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == FALLO) { //error
        perror("Error: bwrite lseek");
        return FALLO;
    }
    if (write(descriptor, buf, BLOCKSIZE) == FALLO) { //error
        perror("Error: bwrite write");
        return FALLO;
    } else {
        return BLOCKSIZE;
    }
}

int bread(unsigned int nbloque, void *buf) {
    if (lseek(descriptor, nbloque * BLOCKSIZE, SEEK_SET) == FALLO) { //error
        perror("Error: bread lseek");
        return FALLO;
    }
    if (read(descriptor, buf, BLOCKSIZE) == FALLO) { //error
        perror("Error: bread read");
        return FALLO;
    } else {
        return BLOCKSIZE;
    }
}
