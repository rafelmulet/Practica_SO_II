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
    int tam = (ninodos * INODOSIZE) / BLOCKSIZE;
    if ((ninodos * INODOSIZE) % BLOCKSIZE) tam++;

    return tam;
}

int initSB(unsigned int nbloques, unsigned int ninodos) {
    //Creacion del superbloque
    struct superbloque SB;

    //Damos el valor correspondientes a todo el contenido del superbloque
    SB.posPrimerBloqueMB = posSB + tamSB;
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
    SB.posUltimoBloqueDatos = nbloques - 1;
    SB.posInodoRaiz = 0;
    SB.posPrimerInodoLibre = 0;
    SB.cantBloquesLibres = nbloques;
    SB.cantInodosLibres = ninodos;
    SB.totBloques = nbloques;
    SB.totInodos = ninodos;

    //Para finalizar, escribimos la estructura en el bloque posSB con bwrite()
    bwrite(posSB, &SB);

    return EXITO;
}

int initMB() {
    //Declaraciones variables auxiliares
    int bloque;
    
    //Leemos el superbloque
    struct superbloque SB;
    bread(posSB, &SB);

    //Calculamos los bits totales de metadatos
    int tamBits = tamSB + tamMB(SB.totBloques) + tamAI(SB.totInodos);

    //Ponemos a 1 los bloques completos 
    unsigned char bufferBC[BLOCKSIZE];
    memset(bufferBC, 255, sizeof(bufferBC));
    for (bloque = 0; bloque < (tamBits / 8 / BLOCKSIZE); bloque++) {
        bwrite(SB.posPrimerBloqueMB + bloque, bufferBC);
    }

    //Declaramos un buffer en el que escribiremos el número de 1s
    //correspondiente
    unsigned char bufferMB[BLOCKSIZE];
    memset(bufferMB, 0, sizeof(bufferMB));

    //Escribimos a 1 los bytes ocupados
    for (int i = 0; i < ((tamBits / 8) - (BLOCKSIZE * bloque)); i++) {
        bufferMB[i] = 255;
    }
    //Escribimos los últimos bits del módulo
    for (int j = 0; j < (tamBits % 8); j++) {
        bufferMB[tamBits / 8] += potencia(2, 7 - j);
    }

    //Escribimos el buffer en el bloque correspondiente
    bwrite(SB.posPrimerBloqueMB + bloque, bufferMB);

    //Reescribimos el superbloque actualizado
    SB.cantBloquesLibres -= tamBits;
    bwrite(posSB, &SB);

    return EXITO;
}

int initAI() {
    //Declaramos los struct superbloque y el array de inodos
    struct superbloque SB;
    struct inodo inodos[BLOCKSIZE/INODOSIZE];

    //Leemos el superbloque
    bread(posSB, &SB);

    int contInodos = SB.posPrimerInodoLibre + 1;

    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) {
        bread(i, inodos);

        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++) {
            inodos[j].tipo = 'l';

            if (contInodos < SB.totInodos) {
                inodos[j].punterosDirectos[0] = contInodos;
                contInodos++;
            } else {
                inodos[j].punterosDirectos[0] = UINT_MAX;
                j = BLOCKSIZE / INODOSIZE;
            }
        }

        bwrite(i, inodos);
    }
    
    return EXITO;
}

int escribir_bit(unsigned int nbloque, unsigned int bit) {
    struct superbloque SB;
    unsigned char bufferMB[BLOCKSIZE];

    bread(posSB, &SB);
    
    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB;

    bread(nbloqueabs, bufferMB);
    posbyte = posbyte % BLOCKSIZE;

    unsigned char mascara = 128;
    mascara >>= posbit;

    if (bit == 1) {
        bufferMB[posbyte] |= mascara;
    } else {
        bufferMB[posbyte] &= ~mascara;
    }

    bwrite(nbloqueabs, bufferMB);

    return EXITO;
}

char leer_bit(unsigned int nbloque) {
    struct superbloque SB;
    unsigned char bufferMB[BLOCKSIZE];

    bread(posSB, &SB);
    
    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;
    int nbloqueabs = SB.posPrimerBloqueMB + nbloqueMB;

    bread(nbloqueabs, bufferMB);
    posbyte = posbyte % BLOCKSIZE;

    unsigned char mascara = 128;
    mascara >>= posbit;
    mascara &= bufferMB[posbyte];
    mascara >>= (7 - posbit);

    return mascara;
}

int reservar_bloque() {
    struct superbloque SB;
    unsigned char bufferMB[BLOCKSIZE];

    bread(posSB, &SB);

    if (SB.cantBloquesLibres == 0) return FALLO;
    
    unsigned char bufferAux[BLOCKSIZE];
    memset(bufferAux, 255, BLOCKSIZE);

    int nbloqueabs = SB.posPrimerBloqueMB;
    bread(nbloqueabs, bufferMB);
    while (memcmp(bufferAux, bufferMB, sizeof(bufferAux)) == 0) {
        nbloqueabs++;
        bread(nbloqueabs, bufferMB);
    }

    int posbyte;
    for (posbyte = 0; bufferMB[posbyte] == 255; posbyte++);

    unsigned char mascara = 128;
    int posbit;
    for (posbit = 0; bufferMB[posbyte] & mascara; posbit++) {
        bufferMB[posbyte] <<= 1;
    }

    int nbloque = ((nbloqueabs - SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte) * 8 + posbit;

    escribir_bit(nbloque, 1);
    SB.cantBloquesLibres--;
    bwrite(posSB, &SB);

    unsigned char bufferDT[BLOCKSIZE];
    memset(bufferDT, 0, BLOCKSIZE);
    bwrite(nbloque, bufferDT);

    return nbloque;
}

int liberar_bloque(unsigned int nbloque) {
    struct superbloque SB;
    bread(posSB, &SB);
    
    escribir_bit(nbloque, 0);
    SB.cantBloquesLibres++;
    bwrite(posSB, &SB);
    
    return nbloque;
}

int escribir_inodo(unsigned int ninodo, struct inodo *inodo) {
    struct superbloque SB;
    bread(posSB, &SB);
    
    int bloqueInodo = ninodo / (BLOCKSIZE / INODOSIZE);
    struct inodo inodos[BLOCKSIZE/INODOSIZE];
    bread(bloqueInodo, inodos);

    inodos[ninodo % (BLOCKSIZE / INODOSIZE)] = *inodo;

    bwrite(bloqueInodo, inodos);

    return EXITO;
}

int leer_inodo(unsigned int ninodo, struct inodo *inodo) {
    struct superbloque SB;
    bread(posSB, &SB);

    int bloqueInodo = ninodo / (BLOCKSIZE / INODOSIZE);
    struct inodo inodos[BLOCKSIZE/INODOSIZE];
    bread(SB.posPrimerBloqueAI + bloqueInodo, inodos);

    *inodo = inodos[ninodo % (BLOCKSIZE / INODOSIZE)];

    return EXITO;
}

int reservar_inodo(unsigned char tipo, unsigned char permisos) {
    struct superbloque SB;
    bread(posSB, &SB);
    
    if (SB.cantInodosLibres == 0) return FALLO;

    struct inodo inodoAux;
    int posInodoReservado = SB.posPrimerInodoLibre;
    leer_inodo(posInodoReservado, &inodoAux);
    
    SB.posPrimerInodoLibre = inodoAux.punterosDirectos[0];

    inodoAux.tipo = tipo;
    inodoAux.permisos = permisos;
    inodoAux.nlinks = 1;
    inodoAux.tamEnBytesLog = 0;
    inodoAux.atime = time(NULL);
    inodoAux.ctime = time(NULL);
    inodoAux.mtime = time(NULL);
    inodoAux.numBloquesOcupados = 0;
    
    for (int i = 0; i < (sizeof(inodoAux.punterosDirectos) / sizeof(int)); i++) {
        inodoAux.punterosDirectos[i] = 0;
    }

    for (int i = 0; i < (sizeof(inodoAux.punterosIndirectos) / sizeof(int)); i++) {
        inodoAux.punterosIndirectos[i] = 0;
    }

    escribir_inodo(posInodoReservado, &inodoAux);
    SB.cantInodosLibres--;
    bwrite(posSB, &SB);

    return posInodoReservado;
}

//FUNCION EXTRA
int potencia(int base, int exponente) {
    if (exponente <= 0) return 1;
    return base * potencia(base, exponente - 1);
}
