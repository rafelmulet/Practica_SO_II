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
    
    printf("\nDATOS DEL SUPERBLOQUE\n");
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
    printf ("sizeof struct inodo: %lu\n", sizeof(struct inodo));
    
    
    printf("\nRECORRIDO LISTA ENLAZADA DE INODOS LIBRES\nLEIL.txt\n");
    struct inodo inodos[BLOCKSIZE/INODOSIZE];
    FILE* fichero;
    fichero = fopen("LEIL.txt", "wt");
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++) {
        bread(i, inodos);

        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++) {
            fprintf(fichero, "%d\t", inodos[j].punterosDirectos[0]);
        }
        fprintf(fichero, "\n");
    }
    fclose(fichero);
    
    
    printf("\nRESERVAMOS UN BLOQUE Y LUEGO LO LIBERAMOS\n");
    int bloqueReservado = reservar_bloque();
    printf("Se ha reservado el bloque físico nº %d que era el 1º libre indicado por el MB\n", bloqueReservado);
    bread(posSB, &SB);
    printf("SB.cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    liberar_bloque(bloqueReservado);
    bread(posSB, &SB);
    printf("Liberamos ese bloque y después SB.cantBloquesLibres = %d\n", SB.cantBloquesLibres);
    
    
    printf("\nMAPA DE BITS CON BLOQUES DE METADATOS OCUPADOS\n");
    printf("posSB: %d -> leer_bit(%d) = %d\n", posSB, posSB, leer_bit(posSB));
    printf("SB.posPrimerBloqueMB: %d -> leer_bit(%d) = %d\n", SB.posPrimerBloqueMB, SB.posPrimerBloqueMB, leer_bit(SB.posPrimerBloqueMB));
    printf("SB.posUltimoBloqueMB: %d -> leer_bit(%d) = %d\n", SB.posUltimoBloqueMB, SB.posUltimoBloqueMB, leer_bit(SB.posUltimoBloqueMB));
    printf("SB.posPrimerBloqueAI: %d -> leer_bit(%d) = %d\n", SB.posPrimerBloqueAI, SB.posPrimerBloqueAI, leer_bit(SB.posPrimerBloqueAI));
    printf("SB.posUltimoBloqueAI: %d -> leer_bit(%d) = %d\n", SB.posUltimoBloqueAI, SB.posUltimoBloqueAI, leer_bit(SB.posUltimoBloqueAI));
    printf("SB.posPrimerBloqueDatos: %d  -> leer_bit(%d) = %d\n", SB.posPrimerBloqueDatos, SB.posPrimerBloqueDatos, leer_bit(SB.posPrimerBloqueDatos));
    printf("SB.posUltimoBloqueDatos: %d  -> leer_bit(%d) = %d\n", SB.posUltimoBloqueDatos, SB.posUltimoBloqueDatos, leer_bit(SB.posUltimoBloqueDatos));

    
    printf("\nDATOS DEL DIRECTORIO RAIZ\n");
    struct tm *ts;
    char atime[80];
    char mtime[80];
    char ctime[80];
    struct inodo inodo;
    int ninodo = 0;

    leer_inodo(ninodo, &inodo);
    printf("tipo: %c\npermisos: %d\n", inodo.tipo, inodo.permisos);

    ts = localtime(&inodo.atime);
    strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.mtime);
    strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.ctime);
    strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    printf("ID: %d\n ATIME: %s\n MTIME: %s\n CTIME: %s\n", ninodo, atime, mtime, ctime);

    bumount();
}