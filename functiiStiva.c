/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tstiva.h"
#include "tserial.h"

size_t DimES(void *a)
{
    return DIMES(a);   
}

int VidaS(void *a)
{
    return VF(a) == NULL;
}

void *InitS(size_t dime)
{
    AS stiva = (AS) malloc(sizeof(TStiva));
    if(stiva == NULL)
        return NULL;

    stiva->dime = dime;
    stiva->vf = NULL;

    return (void *) stiva;
}

int PopSerial(void *stiva, void *ae)
{
    TLista aux = VF(stiva);
    if(aux == NULL)
        return 0;

    memcpy(ae, aux->info, DimES(stiva));
    VF(stiva) = aux->urm;

    return 1;
}

int PushSerial(void *stiva, void *ae)
{
    TLista aux = NULL;
    aux = alocSerial(ae);

    memcpy(aux->info, ae, DimES(stiva));

    aux->urm = VF(stiva);
    VF(stiva) = aux;

    return 1;
}

int RastoarnaStive(void *dest, void *sursa)
{
    if(DimES(dest) != DimES(sursa))
        return -1;
    if(VidaS(sursa))
        return 0;
    
    TLista vf = VF(sursa);
    TLista urm;
    while(vf)
    {
        urm = vf->urm;
        vf->urm = VF(dest);
        VF(dest) = vf;
        vf = urm;
    }
    VF(sursa) = NULL;
    
    return 1;
}

int AfiSSerial(void *stiva, FILE *output_file)
{
    if(VIDAS(stiva))
        return 1;

    void *elem = NULL; // Elementul la care se face extragerea din stiva
    elem = malloc(DimES(stiva));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return 0;
    }

    void *stiva_aux = InitS(DimES(stiva));
    if(stiva_aux == NULL)
    {
        fprintf(output_file, "stiva aloc err\n");
        return 0;
    }   
    
    while(!VidaS(stiva))
    {
        PopSerial(stiva, elem);
        if(!VidaS(stiva))
            AfiSerialQ((TSerial *)elem, output_file);
        else
            AfiUltimSerialQ((TSerial *)elem, output_file);
        PushSerial(stiva_aux, elem);
    }

    RastoarnaStive(stiva, stiva_aux);

    return 1;
}

int CautaSerialS(AS *stiva, char *nume_serial, int durata, FILE *output_file)
{
    void *elem = NULL; // Elementul la care se face extragerea din stiva
    elem = malloc(DimES(*stiva));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return 0;
    }

    void *stiva_aux = InitS(DimES(*stiva));
    if(stiva_aux == NULL)
    {
        fprintf(output_file, "stiva aloc err\n");
        return 0;
    }   
    
    int gasit = 0;
    while(!VidaS(*stiva))
    {
        PopSerial(*stiva, elem);
        
        if(strcmp(((TSerial *)elem)->nume, nume_serial) == 0) // L-am gasit in stiva
        {
            gasit = 1;
            ((TSerial *)elem)->nr_minute_vizionate += durata;
        }
        PushSerial(stiva_aux, elem);
    }

    // Refacere stiva initiala
    RastoarnaStive(*stiva, stiva_aux);

    return gasit;
}

void *EliminaSerialS(AS *stiva, char *nume_serial, int durata, void *historyS, FILE *output_file)
{
    void *elem = NULL; // Elementul la care se face extragerea din stiva
    elem = malloc(DimES(*stiva));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return NULL;
    }

    void *stiva_aux = InitS(DimES(*stiva));
    if(stiva_aux == NULL)
    {
        fprintf(output_file, "stiva aloc err\n");
        return NULL;
    }

    void *stiva_aux2 = InitS(DimES(*stiva));
    if(stiva_aux2 == NULL)
    {
        fprintf(output_file, "stiva aloc err\n");
        return NULL;
    }    

    while(!VidaS(*stiva))
    {
        PopSerial(*stiva, elem);
        if((strcmp(((TSerial *)elem)->nume, nume_serial) == 0) && (((TSerial *)elem)->nr_minute_vizionate >= ((TSerial *)elem)->durata_serial))
        {
            // Il omit doar daca a fost vizionat integral
            fprintf(output_file, "Serialul %s a fost vizionat integral.\n", ((TSerial *)elem)->nume);
            PushSerial(historyS, elem);
        }
        else if(strcmp(((TSerial *)elem)->nume, nume_serial) == 0)
            PushSerial(stiva_aux2, elem); // Pentru a-l adauga la final in varful stivei
        else
            PushSerial(stiva_aux, elem);
    }

    // Refacere stiva initiala
    RastoarnaStive(*stiva, stiva_aux);
    RastoarnaStive(*stiva, stiva_aux2);
        
    return (void *) (*stiva);
}

void *AdaugaSezoaneSerialS(AS *stiva, char *nume_serial, void *info_sezon, int durata_adaugata, FILE *output_file)
{
    void *elem = NULL; // Elementul la care se face extragerea din stiva
    elem = malloc(DimES(*stiva));
    if(elem == NULL)
    {
        fprintf(output_file, "el aloc err\n");
        return NULL;
    }

    void *stiva_aux = InitS(DimES(*stiva));
    if(stiva_aux == NULL)
    {
        fprintf(output_file, "stiva aloc err\n");
        return NULL;
    }    

    while(!VidaS(*stiva))
    {
        PopSerial(*stiva, elem);
        if((strcmp(((TSerial *)elem)->nume, nume_serial) == 0))
        {
            IntrSezonQ(((TSerial *)elem)->sezoane, info_sezon);
            ((TSerial *)elem)->durata_serial += durata_adaugata;
            fprintf(output_file, "Serialul %s are un sezon nou.\n", ((TSerial *)elem)->nume);
        }
        PushSerial(stiva_aux, elem);
    }

    // Refacere stiva initiala
    RastoarnaStive(*stiva, stiva_aux);

    return (void *) (*stiva);
}
