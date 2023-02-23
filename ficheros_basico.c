/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Coppola, Valentino
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

#include "ficheros_basico.h"

int tamMB(unsigned int nbloques) {
    int tam = (nbloques / 8) / BLOCKSIZE;
    if ((nbloques / 8) % BLOCKSIZE != 0) tam++;
    
    return tam;
}

int tamAI(unsigned int ninodos) {
    int tam = ninodos / (BLOCKSIZE / INODOSIZE);
    if ((ninodos * INODOSIZE) % BLOCKSIZE) tam++;

    return tam;
}

int initSB(unsigned int nbloques, unsigned int ninodos) {

}

int initMB() {

}

int initAI() {

}
