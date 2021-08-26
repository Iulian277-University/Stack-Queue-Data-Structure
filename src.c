/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tlista.h"
#include "tserial.h"
#include "tcoada.h"
#include "tstiva.h"
#include "tcomenzi.h"

int main(int argc, char *argv[])
{
    char *input_file_name = argv[1];
    char *output_file_name = argv[2];

    FILE *input_file = NULL;
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL) // Nu s-a putut deschide fisierul de intrare
        return -1;

    FILE *output_file = NULL;
    output_file = fopen(output_file_name, "w");
    if(output_file == NULL) // Nu s-a putut deschide fisierul de iesire
    {
        fclose(input_file);
        return -1;
    }

    char *buffer = NULL;
    buffer = (char *) calloc(LINE_LENGTH, sizeof(char));
    if(buffer == NULL) // Eroare de alocare
    {
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

    char *command = NULL;
    command = (char *) calloc(LINE_LENGTH, sizeof(char));
    if(command == NULL) // Eroare de alocare
    {
        fclose(input_file);
        fclose(output_file);
        free(buffer);
        return -1;
    }

    TLista tendinte = NULL;
    TLista documentare = NULL;
    TLista tutoriale = NULL;

    TLista top10 = NULL;

    void *watch_laterQ; // Coada 'watch_later'
    int nr_el_q_later = 0; // Numarul de elemente din coada watch_later
    watch_laterQ = (AQ) InitQ(sizeof(TSerial));
    if(watch_laterQ == NULL)
    {
        fprintf(output_file, "alloc err\n");
        return -1;
    }

    void *currently_watchingS; // Stiva 'currently_watching'
    currently_watchingS = (AS) InitS(sizeof(TSerial));
    if(currently_watchingS == NULL)
    {
        fprintf(output_file, "alloc err\n");
        return -1;
    }

    void *historyS; // Stiva 'history'
    historyS = InitS(sizeof(TSerial));
    if(historyS == NULL)
    {
        fprintf(output_file, "alloc err\n");
        return -1;
    }

    // Citeste din fisierul de input linie cu linie
    while(!feof(input_file))
    {
        int res_of_fscanf = fscanf(input_file, "%s", buffer);
        if(res_of_fscanf == 0 || buffer[0] == '\n')
            break;
        if(buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        char *token = strtok(buffer, " ");
        strcpy(command, token);
        
        if(strcmp(command, "add") == 0)
        {
            TSerial *info_serial = (TSerial *) malloc(sizeof(TSerial));
            info_serial->nume = (char *) malloc(100 * sizeof(char));

            addCommand(info_serial, &tendinte, &documentare, &tutoriale, input_file, output_file); 
        }
        else if(strcmp(command, "add_sez") == 0)
        {
            char *nume_serial_de_cautat = (char *) malloc(LINE_LENGTH * sizeof(char));
            fscanf(input_file, "%s", nume_serial_de_cautat);

            int info_sezon_nr_episoade = -1;
            int info_episod_durata = -1;
            fscanf(input_file, "%d", &info_sezon_nr_episoade);

            // Aloc informatie pentru un sezon nou
            TSezon *info_sezon = (TSezon *) malloc(sizeof(TSezon));
            info_sezon->nr_episoade = info_sezon_nr_episoade;

            CautaSerialSiAdaugaSezoane(nume_serial_de_cautat, tendinte, documentare, tutoriale, top10, 
                    watch_laterQ, currently_watchingS, output_file, info_sezon, info_episod_durata, input_file);

        }
        else if(strcmp(command, "add_top") == 0)
        {
            TSerial *info_serial = (TSerial *) malloc(sizeof(TSerial));
            info_serial->nume = (char *) malloc(100 * sizeof(char));

            addSerialTop10(info_serial, &top10, input_file, output_file);

            fprintf(output_file, "Categoria top10: ");
            AfisareMesajLaOutput(top10, output_file);
        }
        else if(strcmp(command, "later") == 0)
        {
            char *nume_serial_de_cautat = (char *) malloc(LINE_LENGTH * sizeof(char));
            fscanf(input_file, "%s", nume_serial_de_cautat);

            // Cauta serialul intr-una din cele 4 categorii
            TLista serial_gasit = NULL;
            serial_gasit = CautaSerialInListe(nume_serial_de_cautat, tendinte, documentare, tutoriale, top10);
            if(serial_gasit != NULL) // Este in liste
            {
                IntrSerialQ((void *) (watch_laterQ), (void *) (((TSerial *)serial_gasit->info)), &nr_el_q_later);
                fprintf(output_file, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", 
                    ((TSerial *)serial_gasit->info)->nume, nr_el_q_later);

                // Rupere legaturi pentru serialul 'serial_gasit' din lista
                int res_of_elim_3lists = EliminaSerialDinListe(nume_serial_de_cautat, &tendinte, &documentare, &tutoriale);
                if(res_of_elim_3lists == 0)
                    EliminaSerialDinTop10(nume_serial_de_cautat, &top10);
            }
        }
        else if(strcmp(command, "watch") == 0)
        {
            char *nume_serial_de_cautat = (char *) malloc(LINE_LENGTH * sizeof(char));
            fscanf(input_file, "%s", nume_serial_de_cautat);

            int durata = -1;
            fscanf(input_file, "%d", &durata);

            watchCommand(&tendinte, &documentare, &tutoriale, &top10, nume_serial_de_cautat, watch_laterQ, &nr_el_q_later, 
                            currently_watchingS, historyS, durata, input_file, output_file);
    
        }
        else if(strcmp(command, "show") == 0)
        {
            char *ID = (char *) malloc(LINE_LENGTH * sizeof(char));
            fscanf(input_file, "%s", ID);
            
            if(strcmp(ID, "1") == 0) // Tendinte
            {
                fprintf(output_file, "Categoria %s: ", ID);
                AfisareMesajLaOutput(tendinte, output_file);
            }
            else if(strcmp(ID, "2") == 0) // Documentare
            {
                fprintf(output_file, "Categoria %s: ", ID);
                AfisareMesajLaOutput(documentare, output_file);
            }
            else if(strcmp(ID, "3") == 0) // Tutoriale
            {
                fprintf(output_file, "Categoria %s: ", ID);
                AfisareMesajLaOutput(tutoriale, output_file);
            }
            else if(strcmp(ID, "top10") == 0) // Top10
            {
                fprintf(output_file, "Categoria %s: ", ID);
                AfisareMesajLaOutput(top10, output_file);
            }
            else if(strcmp(ID, "later") == 0) // Coada watch_later
            {
                fprintf(output_file, "Categoria %s: ", ID);
                fprintf(output_file, "[");
                AfiQSerial(watch_laterQ, output_file);
                fprintf(output_file, "].\n");
            }
            else if(strcmp(ID, "watching") == 0) // Stiva currently_watching
            {
                fprintf(output_file, "Categoria %s: ", ID);
                fprintf(output_file, "[");
                AfiSSerial(currently_watchingS, output_file);
                fprintf(output_file, "].\n");
            }
            else if(strcmp(ID, "history") == 0) // Stiva history
            {
                fprintf(output_file, "Categoria %s: ", ID);
                fprintf(output_file, "[");
                AfiSSerial(historyS, output_file);
                fprintf(output_file, "].\n");
            }
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
