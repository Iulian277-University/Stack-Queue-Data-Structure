/* Taiatu Iulian Marius - 312CB */
#include <stdio.h>
#include "tlista.h"
#include "tserial.h"
#include "tcoada.h"

void AfiSerial(TSerial *serial, FILE *output_file)
{
    fprintf(output_file, "[(%s, %0.1f, )] ", serial->nume, serial->rating);
}

void AfiSerialQ(TSerial *serial, FILE *output_file)
{
    fprintf(output_file, "(%s, %0.1f), ", serial->nume, serial->rating);
}

void AfiUltimSerialQ(TSerial *serial, FILE *output_file)
{
    fprintf(output_file, "(%s, %0.1f)", serial->nume, serial->rating);
}