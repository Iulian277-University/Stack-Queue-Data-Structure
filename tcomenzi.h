/* Taiatu Iulian Marius - 312CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _COMENZI_
#define _COMENZI_

int addCommand(TSerial *info_serial, TLista *tendinte, TLista *documentare, TLista *tutoriale, FILE *input_file, FILE *output_file);
int addSerialTop10(TSerial *info_serial, TLista *top10, FILE *input_file, FILE *output_file);

int CautaSerialSiAdaugaSezoane(char *nume_serial_de_cautat, TLista tendinte, TLista documentare, TLista tutoriale, TLista top10, AQ watch_laterQ, 
        AS currently_watchingS, FILE *output_file, TSezon *info_sezon, int info_episod_durata, FILE *input_file);

int watchCommand(TLista *tendinte, TLista *documentare, TLista *tutoriale, TLista *top10, char *nume_serial_de_cautat,
                AQ watch_laterQ, int *nr_el_q_later, AS currently_watchingS, AS historyS, int durata, FILE *input_file, FILE *output_file);

void AfisareMesajLaOutput(TLista lista, FILE *output_file);


#endif
