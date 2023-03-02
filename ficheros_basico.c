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
    unsigned char bufferMB[BLOCKSIZE];
    
    //Leemos el superbloque
    struct superbloque SB;
    bread(posSB, &SB);

    //Calculamos los bits totales de metadatos
    int tamBits = tamSB + tamMB(SB.totBloques) + tamAI(SB.totInodos);
    
    //Ponemos a 1 los bloques completos    
    memset(bufferMB, 255, sizeof(bufferMB));
    for (bloque = 0; bloque < ((tamBits / 8) / BLOCKSIZE); bloque++) {
        bwrite(SB.posPrimerBloqueMB + bloque, bufferMB);
    }

    //Declaramos un buffer en el que escribiremos el número de 1s
    //correspondiente
    memset(bufferMB, 0, sizeof(bufferMB));

    //Escribimos a 1 los bytes ocupados
    for (int i = 0; i < (tamBits / 8); i++) {
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
    //Leemos el superbloque
    struct superbloque SB;
    bread(posSB, &SB);

    //**************************************************************//
    
    return EXITO;
}

int potencia(int base, int exponente) {
    if (exponente <= 0) return 1;
    return base * potencia(base, exponente - 1);
}
