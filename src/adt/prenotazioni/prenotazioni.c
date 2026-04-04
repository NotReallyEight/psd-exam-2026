#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prenotazioni.h"


typedef struct prenotazioneInterna {
    char *matricola;
    char *data;
    int   fasciaOraria;
    int   posto;
    struct prenotazioneInterna *next;
} prenotazioneInterna;

typedef struct listaPrenotazioni {
    prenotazioneInterna *testa;
    int dimensione;
} listaPrenotazioni;