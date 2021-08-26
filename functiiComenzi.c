/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tlista.h"
#include "tserial.h"
#include "tcoada.h"
#include "tstiva.h"
#include "tcomenzi.h"

int addCommand(TSerial *info_serial, TLista *tendinte, TLista *documentare, TLista *tutoriale, FILE *input_file, FILE *output_file)
{
    fscanf(input_file, "%d", &info_serial->ID);
    fscanf(input_file, "%s", info_serial->nume);
    fscanf(input_file, "%f", &info_serial->rating);
    fscanf(input_file, "%d", &info_serial->nr_sezoane);

    if(info_serial->ID != 4) // Daca nu face parte din categoria top10
        info_serial->nr_de_ordine = -1;
    
    info_serial->sezoane = (AQ) InitQ(sizeof(TSezon));
    if(info_serial->sezoane == NULL)
        return -1;

    int info_sezon_nr_episoade = -1;
    int info_episod_durata = -1;
    int durata_totala_vizionare_serial = 0;
    int i =0;
    for(i=0; i<info_serial->nr_sezoane; ++i) // Pentru fiecare sezon
    {
        fscanf(input_file, "%d", &info_sezon_nr_episoade);

        // Aloc informatie pentru un sezon
        TSezon *info_sezon = (TSezon *) malloc(sizeof(TSezon));
        info_sezon->nr_episoade = info_sezon_nr_episoade;

        // Introduc un nou sezon in coada de sezoane
        IntrSezonQ((void *) (info_serial->sezoane), (void *) (info_sezon));
        
        // Initializez o coada de episoade
        info_sezon->episoade = (AQ) InitQ(sizeof(TEpisod));
        if(info_sezon->episoade == NULL)
            return -1;

        int j = 0;
        for(j=0; j<info_sezon->nr_episoade; ++j) // Pentru fiecare episod
        {
            fscanf(input_file, "%d", &info_episod_durata);
            durata_totala_vizionare_serial += info_episod_durata;

            // Aloc informatie pentru un episod
            TEpisod *info_episod = (TEpisod *) malloc(sizeof(TEpisod));
            info_episod->nr_minute = info_episod_durata;

            IntrEpisodQ((void *) (info_sezon->episoade), (void *) (info_episod));
        }
        info_serial->nr_minute_vizionate = 0;
        info_serial->durata_serial = durata_totala_vizionare_serial;
    }

    if(info_serial->ID == 1) // Tendinte
        InsLista(tendinte, (void *) info_serial, output_file);
    else if(info_serial->ID == 2) // Documentare
        InsLista(documentare, (void *) info_serial, output_file);
    else if(info_serial->ID == 3) // Tutoriale
        InsLista(tutoriale, (void *) info_serial, output_file);

    return 1;
}

int CautaSerialSiAdaugaSezoane(char *nume_serial_de_cautat, TLista tendinte, TLista documentare, TLista tutoriale, TLista top10, AQ watch_laterQ, 
        AS currently_watchingS, FILE *output_file, TSezon *info_sezon, int info_episod_durata, FILE *input_file)
{
    // Initializez o coada de episoade
    info_sezon->episoade = (AQ) InitQ(sizeof(TEpisod));
    if(info_sezon->episoade == NULL)
        return -1;

    int durata_totala_adaugata = 0;
    int j = 0;
    for(j=0; j<info_sezon->nr_episoade; ++j) // Pentru fiecare episod
    {
        fscanf(input_file, "%d", &info_episod_durata);
        durata_totala_adaugata += info_episod_durata;

        // Aloc informatie pentru un episod
        TEpisod *info_episod = (TEpisod *) malloc(sizeof(TEpisod));
        info_episod->nr_minute = info_episod_durata;

        IntrEpisodQ((void *) (info_sezon->episoade), (void *) (info_episod));
    }

    TLista serial_gasit = NULL;
    serial_gasit = CautaSerialInListe(nume_serial_de_cautat, tendinte, documentare, tutoriale, top10);
    if(serial_gasit != NULL) // Serialul este in liste
    {       
        // Introduc un nou sezon in coada de sezoane
        IntrSezonQ((void *) (((TSerial *)(serial_gasit->info))->sezoane), (void *) (info_sezon));
    
        ((TSerial *)serial_gasit->info)->durata_serial += durata_totala_adaugata;
        fprintf(output_file, "Serialul %s are un sezon nou.\n", ((TSerial *)serial_gasit->info)->nume);

    }
    else // Serialul nu exista in liste
    {
        // Cautare in coada 'watch_later'
        int serial_gasit_in_coada = CautaSerialQ(&watch_laterQ, nume_serial_de_cautat, 0, output_file);

        if(serial_gasit_in_coada == 1) // Este in coada 'watch_later'
        {
            watch_laterQ = (AQ) AdaugaSezoaneSerialQ(&watch_laterQ, nume_serial_de_cautat, 
                                                    (void *) (info_sezon), durata_totala_adaugata, output_file);
        }
        else
        {
            // Cautare si in stiva 'currently_watching'
            int serial_gasit_in_stiva = CautaSerialS(&currently_watchingS, nume_serial_de_cautat, 0, output_file);

            if(serial_gasit_in_stiva == 1) // Este in stiva 'currently_watching'
            {
                currently_watchingS = (AS) AdaugaSezoaneSerialS(&currently_watchingS, nume_serial_de_cautat, 
                                                                (void *) (info_sezon), durata_totala_adaugata, output_file);
            }
        }
    }
    return 0;
}

int addSerialTop10(TSerial *info_serial, TLista *top10, FILE *input_file, FILE *output_file)
{
    fscanf(input_file, "%d", &info_serial->nr_de_ordine);
    fscanf(input_file, "%s", info_serial->nume);
    fscanf(input_file, "%f", &info_serial->rating);
    info_serial->ID = 4; // Setez categoria 'top10'
    fscanf(input_file, "%d", &info_serial->nr_sezoane);

    info_serial->sezoane = (AQ) InitQ(sizeof(TSezon));
    if(info_serial->sezoane == NULL)
        return -1;

    int info_sezon_nr_episoade = -1;
    int info_episod_durata = -1;
    int durata_totala_vizionare_serial = 0;
    int i = 0;
    for(i=0; i<info_serial->nr_sezoane; ++i) // Pentru fiecare sezon
    {
        fscanf(input_file, "%d", &info_sezon_nr_episoade);

        // Aloc informatie pentru un sezon
        TSezon *info_sezon = (TSezon *) malloc(sizeof(TSezon));
        info_sezon->nr_episoade = info_sezon_nr_episoade;

        // Introduc un nou sezon in coada de sezoane
        IntrSezonQ((void *) (info_serial->sezoane), (void *) (info_sezon));
        
        // Initializez o coada de episoade
        info_sezon->episoade = (AQ) InitQ(sizeof(TEpisod));
        if(info_sezon->episoade == NULL)
            return -1;

        int j = 0;
        for(j=0; j<info_sezon->nr_episoade; ++j) // Pentru fiecare episod
        {
            char *info_ep_durata_string = (char *) malloc(LINE_LENGTH * sizeof(char));
            fscanf(input_file, "%s ", info_ep_durata_string);

            info_episod_durata = atoi(info_ep_durata_string);
            durata_totala_vizionare_serial += info_episod_durata;

            // Aloc informatie pentru un episod
            TEpisod *info_episod = (TEpisod *) malloc(sizeof(TEpisod));
            info_episod->nr_minute = info_episod_durata;

            IntrEpisodQ((void *) (info_sezon->episoade), (void *) (info_episod));
        }
        info_serial->nr_minute_vizionate = 0;
        info_serial->durata_serial = durata_totala_vizionare_serial;
    }

    InsListaTop10(top10, (void *) info_serial, output_file);

    return 1;
}

int watchCommand(TLista *tendinte, TLista *documentare, TLista *tutoriale, TLista *top10, char *nume_serial_de_cautat,
                AQ watch_laterQ, int *nr_el_q_later, AS currently_watchingS, AS historyS, int durata, FILE *input_file, FILE *output_file)
{
    TLista serial_gasit = NULL;
    serial_gasit = CautaSerialInListe(nume_serial_de_cautat, *tendinte, *documentare, *tutoriale, *top10);
    if(serial_gasit != NULL) // Serialul este intr-una dintre cele 4 liste de seriale
    {
        ((TSerial *)serial_gasit->info)->nr_minute_vizionate += durata;

        if(((TSerial *)serial_gasit->info)->nr_minute_vizionate >= ((TSerial *)serial_gasit->info)->durata_serial)
        {
            fprintf(output_file, "Serialul %s a fost vizionat integral.\n", ((TSerial *)serial_gasit->info)->nume);
            PushSerial((void *) historyS, (void *) (((TSerial *)serial_gasit->info)));
            
            // Rupere legaturi pentru serialul 'serial_gasit' din lista
            int res_elim = EliminaSerialDinListe(nume_serial_de_cautat, tendinte, documentare, tutoriale);
            if(res_elim == 0)
                EliminaSerialDinTop10(nume_serial_de_cautat, top10);
        }
        else
        {
            // Introducere serial in stiva 'currently_watchingS'
            PushSerial((void *) currently_watchingS, (void *) (((TSerial *)serial_gasit->info)));

            // Rupere legaturi pentru serialul 'serial_gasit' din lista
            int res_elim = EliminaSerialDinListe(nume_serial_de_cautat, tendinte, documentare, tutoriale);
            if(res_elim == 0)
                EliminaSerialDinTop10(nume_serial_de_cautat, top10);
        }
    }
    else // Serialul nu se afla in liste
    {
        // Cauta si in stiva 'currently_watching'
        int serial_gasit_in_stiva = CautaSerialS(&currently_watchingS, nume_serial_de_cautat, durata, output_file);

        if(serial_gasit_in_stiva == 1) // Serialul se afla in stiva 'currently_watching'
            currently_watchingS = (AS) EliminaSerialS(&currently_watchingS, nume_serial_de_cautat, durata, historyS, output_file);
        else // Serialul nu se afla in stiva 'currently_watching'
        {
            // Cauta si in coada 'watch_later'
            int serial_gasit_in_coada = CautaSerialQ(&watch_laterQ, nume_serial_de_cautat, durata, output_file);
        
            if(serial_gasit_in_coada == 1) // Serialul se afla in coada 'watch_later'
            {   
                watch_laterQ = (AQ) EliminaSerialQ(&watch_laterQ, nr_el_q_later, nume_serial_de_cautat, durata, 
                                                    historyS, currently_watchingS, output_file);
            }
        }
    }

    return 1;
}

void AfisareMesajLaOutput(TLista lista, FILE *output_file)
{
    TLista p = lista;
    fprintf(output_file, "[");
    for(; p != NULL; p = p->urm)
    {
        
        if(p->urm == NULL)
            fprintf(output_file, "(%s, %0.1f)", ((TSerial *)p->info)->nume, ((TSerial *)p->info)->rating);
        else
            fprintf(output_file, "(%s, %0.1f), ", ((TSerial *)p->info)->nume, ((TSerial *)p->info)->rating);
    }
    fprintf(output_file, "].\n");
}
