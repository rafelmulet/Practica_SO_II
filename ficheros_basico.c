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

    //Posición del primer bloque del mapa de bits
    SB.posPrimerBloqueMB = posSB + tamSB;
    //Posición del último bloque del mapa de bits
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
    //Posición del primer bloque del array de inodos
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
    //Posición del último bloque del array de inodos
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
    //Posición del primer bloque de datos
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
    //Posición del último bloque de datos
    SB.posUltimoBloqueDatos = nbloques - 1;
    //Posición del inodo del directorio raíz en el array de inodos
    SB.posInodoRaiz = 0;
    //Posición del primer inodo libre en el array de inodos
    SB.posPrimerInodoLibre = 0;
    //Cantidad de bloques libres en el SF
    SB.cantBloquesLibres = nbloques;
    //Cantidad de inodos libres en el array de inodos
    SB.cantInodosLibres = ninodos;
    //Cantidad total de bloques
    SB.totBloques = nbloques;
    //Cantidad total de inodos
    SB.totInodos = ninodos;
    
    //Para finalizar, escribimos la estructura en el bloque posSB con bwrite()
    bwrite(posSB, &SB);
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
