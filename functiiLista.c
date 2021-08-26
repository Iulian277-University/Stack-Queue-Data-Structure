/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tlista.h"
#include "tserial.h"
#include "tcoada.h"

TLista alocSerial(void *info_serial)
{
    // Aloca o celula de lista 'serial'
    TLista serial = malloc(sizeof(TCelula));
    
    // Aloca informatia din celula 'serial'
    serial->info = (TSerial *) malloc(sizeof(TSerial));
    (*(TSerial *)serial->info).nume = malloc(LINE_LENGTH * sizeof(char));
    serial->urm = NULL;

    (*(TSerial *)serial->info).ID = (*(TSerial *)info_serial).ID;
    ((TSerial *)(serial->info))->nr_de_ordine = (*(TSerial *)info_serial).nr_de_ordine;

    ((TSerial *)(serial->info))->durata_serial = (*(TSerial *)info_serial).durata_serial;
    ((TSerial *)(serial->info))->nr_minute_vizionate = (*(TSerial *)info_serial).nr_minute_vizionate;


    strcpy(((TSerial *)serial->info)->nume, (*(TSerial *)info_serial).nume);
    (*(TSerial *)serial->info).rating = (*(TSerial *)info_serial).rating;
    (*(TSerial *)serial->info).nr_sezoane = (*(TSerial *)info_serial).nr_sezoane;
    
    (*(TSerial *)serial->info).sezoane = (AQ) ((*(TSerial *)info_serial).sezoane);

    return serial;
}

TLista alocaSezon(void *info_sezon)
{
    // Aloca o celula de lista 'sezon'
    TLista sezon = malloc(sizeof(TCelula));
    
    sezon->info = (TSezon *) malloc(sizeof(TSezon));
    (*(TSezon *)sezon->info).nr_episoade = (*(TSezon *)info_sezon).nr_episoade;
    sezon->urm = NULL;

    (*(TSezon *)sezon->info).episoade = (AQ) ((*(TSezon *)info_sezon).episoade);
    
    return sezon;
}

TLista alocaEpisod(void *info_episod)
{
    // Aloca o celula de lista 'episod'
    TLista episod = malloc(sizeof(TCelula));
    
    episod->info = (TEpisod *) malloc(sizeof(TEpisod));
    (*(TEpisod *)episod->info).nr_minute = (*(TEpisod *)info_episod).nr_minute;
    episod->urm = NULL;

    return episod;
}

int InsLista(TLista *L, void *ae, FILE *output_file)
{
    // Insereaza descrescator dupa rating: rating egal => crescator dupa nume

    TLista aux = NULL;
    aux = alocSerial(ae);
    if(aux == NULL)
        return 0;
    aux->info = (void *) ae;

    int count_position = 1;
    TLista p = *L, ant = NULL;
    while(p != NULL)
    {
        float rating_p = ((TSerial *)p->info)->rating;
        float rating_ae = ((TSerial *)ae)->rating;

        char *nume_p = ((TSerial *)p->info)->nume;
        char *nume_ae = ((TSerial *)ae)->nume;

        if((rating_p > rating_ae) || ((rating_p == rating_ae) && (strcmp(nume_p, nume_ae) < 0)))
        {
            ant = p;
            p = p->urm;
            count_position++;
        }
        else
            break;
    }
	
	if(ant == NULL) // Inserare la inceputul listei
	{
		aux->urm = p;
		*L = aux;
	}
	else // Inserare in interiorul listei
	{
		ant->urm = aux;
		aux->urm = p;
	}

    if(((TSerial *)aux->info)->ID != 4)
        fprintf(output_file, "Serialul %s a fost adaugat la pozitia %d.\n", ((TSerial *)aux->info)->nume, count_position);

    return 1;
}

int InsListaTop10(TLista *L, void *ae, FILE *output_file)
{
    // Inserare in lista 'top10'
    TLista aux = NULL;
    aux = alocSerial(ae);
    aux->info = (void *) ae;

    int poz_de_inserat = ((TSerial *)ae)->nr_de_ordine;

    TLista p = *L, ant = NULL;
    if(poz_de_inserat == 1) // Inserare pe locul 1 in 'top10' (inserare la inceput de lista)
    {
		aux->urm = p;
		*L = aux;
    }
    else // Daca se cere o pozitie de inserare > 1
    {
        int cnt = 1;
        while(p != NULL)
        {
            ant = p;
            p = p->urm;
            cnt++;
            
            if(cnt == poz_de_inserat) // Inserez intre ant si p
            {
                ant->urm = aux;
                aux->urm = p;
            }
        }

    }

    // Modifica pozitia in 'top10' a serialelor de mai jos
    p = (*L)->urm; // Incepand de la pozitia 2
    for(; p != NULL; p = p->urm)
        ((TSerial *)p->info)->nr_de_ordine += 1;

    
    // Pastrare doar primele 10 pozitii din lista 'top10'
    int cnt = 1;
    p = *L;
    ant = NULL;
    while(p != NULL && cnt <= 10)
    {
        ant = p;
        p = p->urm;
        cnt ++;
    }
    
    if(p == NULL) // Exact 10 pozitii in lista 'top10'
        return 1;

    ant->urm = NULL; // Mai mult de 10 pozitii

    return 1;
}

// Intoarce un pointer la celula de lista - celula unde se afla serialul cautata
TLista CautaSerialInListe(char *nume_serial, TLista tendinte, TLista documentare, TLista tutoriale, TLista top10)
{
    // Itereaza prin cele 4 liste (tendinte, documentare, tutoriale, top10)
    TLista p = tendinte;
    for(; p != NULL; p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
            return p;
    }

    p = documentare;
    for(; p != NULL; p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
            return p;
    }

    p = tutoriale;
    for(; p != NULL; p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
            return p;
    }

    p = top10;
    for(; p != NULL; p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
            return p;
    }

    return NULL; // Serialul nu a fost gasit
}

int EliminaSerialDinListe(char *nume_serial, TLista *tendinte, TLista *documentare, TLista *tutoriale)
{
    // Itereaza prin cele 4 liste (tendinte, documentare, tutoriale, top10)
    TLista p, ant;
    for(p = *tendinte, ant = NULL; p != NULL; ant = p, p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
        {
            if(ant == NULL) // Eliminare la inceput de lista
                *tendinte = p->urm;
            else // Eliminare in interiorul listei
                ant->urm = p->urm;
            return 1;
        }
    }

    for(p = *documentare, ant = NULL; p != NULL; ant = p, p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
        {
            if(ant == NULL) // Eliminare la inceput de lista
                *documentare = p->urm;
            else // Eliminare in interiorul listei
                ant->urm = p->urm;
            return 1;
        }
    }

    for(p = *tutoriale, ant = NULL; p != NULL; ant = p, p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
        {
            if(ant == NULL) // Eliminare la inceput de lista
                *tutoriale = p->urm;
            else // Eliminare in interiorul listei
                ant->urm = p->urm;
            return 1;
        }
    }

    return 0; // Serialul nu a putut fi eliminat
}

int EliminaSerialDinTop10(char *nume_serial, TLista *top10)
{
    // Itereaza prin lista 'top10'
    TLista p, ant;
    for(p = *top10, ant = NULL; p != NULL; ant = p, p = p->urm)
    {
        if(strcmp(nume_serial, ((TSerial *)(p->info))->nume) == 0)
        {
            if(ant == NULL) // Eliminare la inceput de lista
            {
                *top10 = p->urm;

                p = *top10;
                for(; p != NULL; p = p->urm)
                    ((TSerial *)p->info)->nr_de_ordine -= 1;

                return 1;
            }
            else // Eliminare in interiorul listei
            {
                ant->urm = p->urm;

                p = ant->urm;
                for(; p != NULL; p = p->urm)
                    ((TSerial *)p->info)->nr_de_ordine -= 1;

                return 1;
            }
        }
    }

    return 0; // Serialul nu a putut fi eliminat
}
