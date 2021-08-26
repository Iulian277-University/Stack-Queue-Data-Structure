/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tcoada.h"

#define LINE_LENGTH 50

#ifndef _SERIAL_
#define _SERIAL_

typedef struct serial
{
    int ID;
    char *nume;
    float rating;
    int nr_sezoane;
    int nr_de_ordine; // Pentru Top10
    int nr_minute_vizionate;
    int durata_serial;
    AQ sezoane; // Inceput coada de sezoane
}TSerial; // Tendinte (1), Documentare (2), Tutoriale (3), Top10 (4)

typedef struct sezon
{
    int nr_episoade;
    AQ episoade; // Inceput coada de episoade
}TSezon;

typedef struct episod
{
    int nr_minute;
}TEpisod;


void AfiSerial(TSerial *serial, FILE *output_file);
void AfiEpisod(TEpisod *episod, FILE *output_file);
void AfiSerialQ(TSerial *serial, FILE *output_file);
void AfiUltimSerialQ(TSerial *serial, FILE *output_file);


#endif