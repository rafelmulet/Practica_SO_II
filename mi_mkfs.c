/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

#include "bloques.h"

int main(int argc, char **argv){
    unsigned char *buffer[BLOCKSIZE];
    memset(buffer, 0, sizeof(buffer));
    int nbloques;

    //sintaxis
    if (argc != 3) {
        printf("./mi_mkfs <nombre_dispositivo> <nbloques>\n");
        return FALLO;
    }
    nbloques = atoi(argv[2]);
    if (nbloques  == 0){
        printf("./mi_mkfs <nombre_dispositivo> <nbloques>\n");
        return FALLO;
    }
    
    bmount(argv[1]);
    for (int i = 0; i < nbloques; i++){
        bwrite(i, buffer);
    }
    bumount();

    return EXITO;
}
