/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tcoada.h"
#include "tstiva.h"
#include "tserial.h"

size_t DimEQ(void *a)
{
    return DIME(a);
}

int VidaQ(void *a)
{
    return (VIDAQ(a));
}

void *InitQ(size_t dime)
{
    AQ coada = (AQ) malloc(sizeof(TCoada));
    if(!coada)
        return NULL;

    coada->dime = dime;

    IC(coada) = NULL;
    SC(coada) = NULL;

    return (void *) coada;
}

int ExtrQ(void *coada, void *ae)
{   
    TLista aux = IC(coada);
    if(aux == NULL)
        return 0;

    IC(coada) = aux->urm;

    memcpy(ae, aux->info, DimEQ(coada));

    // Test daca am avut un singur element in coada
    if(IC(coada) == NULL)
        SC(coada) = NULL;
    
    return 1;
}

int IntrSerialQ(void *coada, void *ae, int *nr_el_q_later)
{
    if(VIDAQ(coada)) // Inserare in coada vida
    {
        TLista aux = NULL;
        aux = alocSerial(ae);
        if(!aux)
            return -1;

        memcpy(aux->info, ae, DimEQ(coada));
        aux->urm = NULL;
        
        IC(coada) = aux;
        SC(coada) = aux;

        (*nr_el_q_later)++;

        return 1;
    }
    else // Inserare la sfarsitul cozii
    {
        TLista aux = NULL;
        aux = alocSerial(ae);
        if(!aux)
            return -1;

        memcpy(aux->info, ae, DimEQ(coada));
        aux->urm = NULL;
        
        SC(coada)->urm = aux; // Legare la sfarsitul cozii
        SC(coada) = aux; // Muta sfarsitul cozii la ultima celula (celula inserata anterior)

        (*nr_el_q_later)++;

        return 1;
    }

    return -1;
}

int IntrSerialQPentruMutare(void *coada, void *ae)
{
    if(VIDAQ(coada)) // Inserare in coada vida
    {
        TLista aux = NULL;
        aux = alocSerial(ae);
        if(!aux)
            return -1;
        
        memcpy(aux->info, ae, DimEQ(coada));
        aux->urm = NULL;
        
        IC(coada) = aux;
        SC(coada) = aux;

        return 1;
    }
    else // Inserare la sfarsitul cozii
    {
        // fprintf(output_file, "aici2\n");
        TLista aux = NULL;
        aux = alocSerial(ae);
        if(!aux)
            return -1;

        memcpy(aux->info, ae, DimEQ(coada));
        aux->urm = NULL;
        
        SC(coada)->urm = aux; // Legare la sfarsitul cozii
        SC(coada) = aux; // Muta sfarsitul cozii la ultima celula (celula inserata anterior)

        return 1;
    }

    return -1;
}

int IntrSezonQ(void *coada, void *ae)
{
    if(VIDAQ(coada)) // Inserare in coada vida
    {
        TLista aux = NULL;
        aux = alocaSezon(ae);
        if(!aux)
            return -1;

        aux->info = ae; // ae - Alocat in prealabil
        aux->urm = NULL;
        
        IC(coada) = aux;
        SC(coada) = aux;

        return 1;
    }
    else // Inserare la sfarsitul cozii
    {
        TLista aux = NULL;
        aux = alocaSezon(ae);
        if(!aux)
            return -1;

        aux->info = ae; // ae - Alocat in prealabil
        aux->urm = NULL;
        
        SC(coada)->urm = aux; // Legare la sfarsitul cozii
        SC(coada) = aux; // Muta sfarsitul cozii la ultima celula (celula inserata anterior)

        return 1;
    }

    return 1;
}

int IntrEpisodQ(void *coada, void *ae)
{
    if(VIDAQ(coada)) // Inserare in coada vida
    {
        TLista aux = NULL;
        aux = alocaEpisod(ae);
        if(!aux)
            return -1;

        memcpy(aux->info, ae, DimEQ(coada)); // ae - Alocat in prealabil
        aux->urm = NULL;
        
        IC(coada) = aux;
        SC(coada) = aux;

        return 1;
    }
    else // Inserare la sfarsitul cozii
    {
        TLista aux = NULL;
        aux = alocaEpisod(ae);
        if(!aux)
            return -1;

        memcpy(aux->info, ae, DimEQ(coada)); // ae - Alocat in prealabil
        aux->urm = NULL;
        
        SC(coada)->urm = aux; // Legare la sfarsitul cozii
        SC(coada) = aux; // Muta sfarsitul cozii la ultima celula

        return 1;
    }

    return 1;
}

int AfiQSerial(void *coada, FILE *output_file)
{
    
    if(VIDAQ(coada))
        return 1;

    void *elem = NULL; // Elementul la care se face extragerea din coada
    elem = malloc(DimEQ(coada));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return 0;
    }

    void *coada_aux = InitQ(DimEQ(coada)); // Coada auxiliara
    if(coada_aux == NULL)
    {
        free(elem);
        fprintf(output_file, "coada aloc err\n");
        return 0;
    }

    while(!VIDAQ(coada))
    {
        ExtrQ(coada, elem);
        if(!VIDAQ(coada))
            AfiSerialQ(((TSerial *)elem), output_file);
        else
            AfiUltimSerialQ(((TSerial *)elem), output_file);
        IntrSerialQPentruMutare(coada_aux, elem);
    }

    // Refacere coada initiala
    IC(coada) = IC(coada_aux);
    SC(coada) = SC(coada_aux);

    return 1;
}

int CautaSerialQ(AQ *coada, char *nume_serial, int durata, FILE *output_file)
{   
    void *elem = NULL; // Elementul la care se face extragerea din coada
    elem = malloc(DimEQ(*coada));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return 0;
    }

    void *coada_aux = InitQ(DimEQ(*coada));
    if(coada_aux == NULL)
    {
        fprintf(output_file, "coada aloc err\n");
        return 0;
    }   

    int gasit = 0;
    while(!VidaQ(*coada))
    {
        ExtrQ(*coada, elem);

        if(strcmp(((TSerial *)elem)->nume, nume_serial) == 0) // L-am gasit in coada
        {
            gasit = 1;
            ((TSerial *)elem)->nr_minute_vizionate += durata;
        }
        IntrSerialQPentruMutare(coada_aux, elem);
    }
    // Refacere coada initiala
    IC(*coada) = IC(coada_aux);
    SC(*coada) = SC(coada_aux);

    return gasit;
}

void *EliminaSerialQ(AQ *coada, int *nr_el_q_later, char *nume_serial, int durata, void *historyS, void *currentlyWatchingS, FILE *output_file)
{
    void *elem = NULL; // Elementul la care se face extragerea din coada
    elem = malloc(DimEQ(*coada));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return NULL;
    }

    void *coada_aux = InitQ(DimEQ(*coada));
    if(coada_aux == NULL)
    {
        fprintf(output_file, "coada aloc err\n");
        return NULL;
    }   


    while(!VidaQ(*coada))
    {
        ExtrQ(*coada, elem);

        if((strcmp(((TSerial *)elem)->nume, nume_serial) == 0))
        {
            (*nr_el_q_later)--;
            if((((TSerial *)elem)->nr_minute_vizionate >= ((TSerial *)elem)->durata_serial))
            {
                // Il omit doar daca a fost vizionat integral
                fprintf(output_file, "Serialul %s a fost vizionat integral.\n", ((TSerial *)elem)->nume); 
                PushSerial(historyS, elem);
            }
            else
                PushSerial(currentlyWatchingS, elem);
        }
        else
            IntrSerialQPentruMutare(coada_aux, elem);
    }

    // Refacere coada initiala
    IC(*coada) = IC(coada_aux);
    SC(*coada) = SC(coada_aux);

    return (void *) (*coada);
}

void *AdaugaSezoaneSerialQ(AQ *coada, char *nume_serial, void *info_sezon, int durata_adaugata, FILE *output_file)
{
    void *elem = NULL; // Elementul la care se face extragerea din coada
    elem = malloc(DimEQ(*coada));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return NULL;
    }

    void *coada_aux = InitQ(DimEQ(*coada));
    if(coada_aux == NULL)
    {
        fprintf(output_file, "coada aloc err\n");
        return NULL;
    }   

    while(!VidaQ(*coada))
    {
        ExtrQ(*coada, elem);
        if((strcmp(((TSerial *)elem)->nume, nume_serial) == 0))
        {
            IntrSezonQ(((TSerial *)elem)->sezoane, info_sezon);
            ((TSerial *)elem)->durata_serial += durata_adaugata;
            fprintf(output_file, "Serialul %s are un sezon nou.\n", ((TSerial *)elem)->nume);
        }
        IntrSerialQPentruMutare(coada_aux, elem);
    }

    // Refacere coada initiala
    IC(*coada) = IC(coada_aux);
    SC(*coada) = SC(coada_aux);

    return (void *) (*coada);
}
