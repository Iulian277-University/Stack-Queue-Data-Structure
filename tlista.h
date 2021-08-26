/* Taiatu Iulian Marius - 312CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{
    void *info;
    struct celula *urm;

}TCelula, *TLista;


TLista alocSerial(void *info_serial);
TLista alocaSezon(void *info_sezon);
TLista alocaEpisod(void *info_episod);

int InsLista(TLista *L, void *ae, FILE *output_file);
int InsListaTop10(TLista *L, void *ae, FILE *output_file);

TLista CautaSerialInListe(char *nume_serial, TLista tendinte, TLista documentare, TLista tutoriale, TLista top10);
int EliminaSerialDinListe(char *nume_serial, TLista *tendinte, TLista *documentare, TLista *tutoriale);
int EliminaSerialDinTop10(char *nume_serial, TLista *top10);


#endif