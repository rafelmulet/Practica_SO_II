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
 //Calculamos los bits totales de metadatos y los pasamos a bytes
    int tam= (tamMB()+tamAI()+tamSB)/8;
    //realizamos el modulo del tamaño total para saber el numero de 1's
    //del último bloque
    int mod= tam % 8;

    //declaramos el buffer
    unsigned char bufferMB[BLOCKSIZE];

    //iteramos desde i=0 a i= tam haciendo la asignación de 1's
    for(int i =posSB;i<tam;i++){
        buffer[i]=255;
    }
    //para el último byte asignamos 1's solo a los bits que corresponden
    for (int j = 0; j<mod; j++){
        buffer[tam]+=2^(7-j)
    }
    //ponemos los bytes restantes del bloque a 0
    for(int x=tam+1;x<BLOCKSIZE;x++){
        buffer[x]=0;
    }
}

int initAI() {

}
