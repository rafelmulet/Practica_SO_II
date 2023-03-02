/*
INTEGRANTES DEL GRUPO QUE HAN PARTICIPADO
- Coppola, Valentino
- Frau Gordiola, Antoni
- Mulet Serna, Rafel
*/

#include "ficheros_basico.h"

int main(int argc, char **argv) {
    //Comprobar sintaxis
    if (argc != 2) {
        printf("./leer_sf <nombre_dispositivo>\n");
        return FALLO;
    }
    
    bmount(argv[1]);
    
    struct superbloque SB;
    bread(posSB, &SB);
    
    printf("DATOS DEL SUPERBLOQUE\n");
    printf("posPrimerBloqueMB = %d\n", SB.posPrimerBloqueMB);
    printf("posUltimoBloqueMB = %d\n", SB.posUltimoBloqueMB);
    printf("posPrimerBloqueAI = %d\n", SB.posPrimerBloqueAI);
    printf("posUltimoBloqueAI = %d\n", SB.posUltimoBloqueAI);
    printf("posPrimerBloqueDatos = %d\n", SB.posPrimerBloqueDatos);
    printf("posUltimoBloqueDatos = %d\n", SB.posUltimoBloqueDatos);
    printf("posInodoRaiz = %d\n", SB.posInodoRaiz);
    printf("posPrimerInodoLibre = %d\n", SB.posPrimerInodoLibre);
    printf("cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    printf("cantInodosLibres = %d\n", SB.cantInodosLibres);
    printf("totBloques = %d\n", SB.totBloques);
    printf("totInodos = %d\n\n", SB.totInodos);

    printf ("sizeof struct superbloque: %lu\n", sizeof(struct superbloque));
    printf ("sizeof struct inodo: %lu\n\n", sizeof(struct inodo));

    printf("RECORRIDO LISTA ENLAZADA DE INODOS LIBRES\n");
    //**************************************************//

    bumount();
}