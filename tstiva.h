/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include <stdlib.h>
#include "tlista.h"

#ifndef _STIVA_
#define _STIVA_

typedef struct
{
    size_t dime; // Dimensiunea unui element dintr-o celula din stiva
    TLista vf; // Celula de lista pentru varful stivei

}TStiva, *AS;


#define VF(a) (((AS)(a))->vf)
#define VIDAS(a) ((VF(a)) == NULL)
#define DIMES(a) (((AS)(a))->dime)


size_t DimES(void *a);
int VidaS(void *a);

void *InitS(size_t dime);
int PopSerial(void *stiva, void *ae);
int PushSerial(void *stiva, void *ae);

int RastoarnaStive(void *d, void *s);

int AfiSSerial(void *stiva, FILE *output_file);

int CautaSerialS(AS *stiva, char *nume_serial, int durata, FILE *output_file);
void *EliminaSerialS(AS *stiva, char *nume_serial, int durata, void *historyS, FILE *output_file);

void *AdaugaSezoaneSerialS(AS *stiva, char *nume_serial, void *info_sezon, int durata_adaugata, FILE *output_file);


#endif