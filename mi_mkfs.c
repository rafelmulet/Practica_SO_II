/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Coppola, Valentino
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

#include "ficheros_basico.h"

int main(int argc, char **argv) {
    unsigned char buffer[BLOCKSIZE];
    memset(buffer, 0, sizeof(buffer));
    int nbloques;

    //Comprobar sintaxis
    if (argc != 3) {
        printf("./mi_mkfs <nombre_dispositivo> <nbloques>\n");
        return FALLO;
    }
    nbloques = atoi(argv[2]);
    if (nbloques == 0) {
        printf("./mi_mkfs <nombre_dispositivo> <nbloques>\n");
        return FALLO;
    }

    //Apertura i formateo del fichero
    bmount(argv[1]);
    for (int i = 0; i < nbloques; i++) {
        bwrite(i, buffer);
    }

    //Inicialización superbloque
    initSB(nbloques, nbloques/4);

    //Inicialización mapa de bits
    initMB();

    //Inicialización lista inodos libres
    initAI();

    //Creación el directorio raíz
    reservar_inodo('d', 7);

    //Cierre fichero
    bumount();

    return EXITO;
}
