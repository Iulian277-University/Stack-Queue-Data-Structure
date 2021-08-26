/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include <stdlib.h>
#include "tlista.h"

#ifndef _COADA_
#define _COADA_

typedef struct
{
    size_t dime; // Dimensiunea unui element dintr-o celula din coada
    TLista ic; // Celula de lista pentru inceput coada
    TLista sc; // Celula de lista pentru sfarsit coada

}TCoada, *AQ;


#define IC(a) (((AQ)(a))->ic)
#define SC(a) (((AQ)(a))->sc)
#define DIME(a) (((AQ)(a))->dime)
#define DIMDIF(s,d) (DIME(s) != DIME(d))
#define VIDAQ(a) ((IC(a) == NULL) && (SC(a) == NULL))


size_t DimEQ(void *a);
int VidaQ(void *a);

void *InitQ(size_t dime);
int ExtrQ(void *coada, void *ae);

int IntrSerialQ(void *coada, void *ae, int *nr_el_q_later);
int IntrSerialQPentruMutare(void *coada, void *ae);
int IntrSezonQ(void *coada, void *ae);
int IntrEpisodQ(void *coada, void *ae);

int AfiQSerial(void *coada, FILE *output_file);

int CautaSerialQ(AQ *coada, char *nume_serial, int durata, FILE *output_file);
void *EliminaSerialQ(AQ *coada, int *nr_el_q_later, char *nume_serial, int durata, void *historyS, void *currentlyWatchingS, FILE *output_file);

void *AdaugaSezoaneSerialQ(AQ *coada, char *nume_serial, void *info_sezon, int durata_adaugata, FILE *output_file);


#endif