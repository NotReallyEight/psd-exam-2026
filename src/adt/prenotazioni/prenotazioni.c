#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prenotazioni.h"

#define NUM_FASCE 5

typedef struct prenotazioneInterna { // definizione struttura nodo per la lista delle prenotazioni
    char *matricola; // stringa per la matricola dello studente prenotato
    char *data; // stringa per la data della prenotazione
    int fasciaOraria; // intero che rappresenta l'indice della fascia oraria
    int posto; // numero del posto assegnato in aula
    struct prenotazioneInterna *next; // puntatore al prossimo nodo della lista
} prenotazioneInterna;

typedef struct listaPrenotazioni { // struttura di testata per la gestione della lista
    prenotazioneInterna *testa; // puntatore al primo elemento della lista
    int dimensione; // contatore del numero totale di prenotazioni attive
} listaPrenotazioni;

static listaPrenotazioni *lista = NULL; // riferimento statico alla lista prenotazioni

static char *duplicaStringa(const char *src) { // funzione locale per clonare stringhe
    char *dst; // puntatore destinazione
    if (!src) 
        return NULL;

    dst = (char *)malloc(strlen(src) + 1); // allocazione memoria per copia stringa
    if (!dst) 
        return NULL;

    strcpy(dst, src); // copia caratteri sorgente in destinazione
    return dst; // ritorna indirizzo nuova stringa
}

static prenotazioneInterna *creaNodo(const char *matricola, const char *data, int fasciaOraria, int posto) { // creazione nuovo nodo

    prenotazioneInterna *nodo = (prenotazioneInterna *)malloc(sizeof(prenotazioneInterna)); // alloca memoria nodo
    if (!nodo) 
        return NULL;

    nodo->matricola = duplicaStringa(matricola); // alloca e duplica matricola
    nodo->data = duplicaStringa(data); // alloca e duplica data
    nodo->fasciaOraria = fasciaOraria; // assegna fascia oraria
    nodo->posto = posto; // assegna numero posto
    nodo->next = NULL; // inizializza puntatore prossimo a null

    if (!nodo->matricola || !nodo->data) { // gestione errore allocazione stringhe
        free(nodo->matricola); // deallocazione parziale matricola
        free(nodo->data); // deallocazione parziale data
        free(nodo); // deallocazione nodo
        return NULL;
    }

    return nodo; // ritorna puntatore al nodo creato
}

static void liberaNodo(prenotazioneInterna *nodo) { // rimozione fisica di un nodo
    if (!nodo) 
        return;

    free(nodo->matricola); // libera memoria stringa matricola
    free(nodo->data); // libera memoria stringa data
    free(nodo); // libera struttura nodo
}

int creaListaPrenotazioni() { // inizializzazione modulo prenotazioni

    lista = (listaPrenotazioni *)malloc(sizeof(listaPrenotazioni)); // alloca testata lista
    if (!lista) 
        return -1;

    lista->testa = NULL; // lista inizialmente vuota
    lista->dimensione = 0; // dimensione iniziale zero
    return 0;
}

void distruggiListaPrenotazioni() { // deallocazione totale della lista
    prenotazioneInterna *curr, *next;

    if (!lista) 
        return;

    curr = lista->testa; // parte dalla testa
    while (curr) { // scorre tutta la lista
        next = curr->next; // salva riferimento successivo
        liberaNodo(curr); // libera nodo corrente
        curr = next; // passa al prossimo
    }

    free(lista); // libera testata lista
    lista = NULL; // azzera riferimento globale
}

int creaPrenotazione(const char *matricola, const char *data, int fasciaOraria, int posto) { // inserimento nuova prenotazione

    prenotazioneInterna *nodo, *curr;

    if (!lista || !matricola || !data) 
        return -1; // validazione puntatori

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) 
        return -1; // validazione range fascia

    if (posto < 0) 
        return -1; // validazione numero posto


    curr = lista->testa; // verifica duplicati nella lista
    while (curr) { // scansione per ricerca corrispondenze
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) 
            return -1; // gia prenotato

        curr = curr->next;
    }

    nodo = creaNodo(matricola, data, fasciaOraria, posto); // creazione fisica del nodo

    if (!nodo) 
        return -1;

    nodo->next = lista->testa; // inserimento in testa alla lista
    lista->testa = nodo; // aggiornamento puntatore testa
    lista->dimensione++; // incremento contatore prenotazioni

    return 0;
}

int annullaPrenotazione(const char *matricola, const char *data, int fasciaOraria) { // cancellazione di una prenotazione

    prenotazioneInterna *curr, *prev;

    if (!lista || !matricola || !data) 
        return -1;

    curr = lista->testa; // ricerca elemento da eliminare
    prev = NULL; // tracciamento nodo precedente

    while (curr) { // scansione lista
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) {
            if (prev) prev->next = curr->next; // scavalca nodo corrente nel mezzo della lista
            else lista->testa = curr->next; // aggiorna testa se rimosso primo elemento
            liberaNodo(curr); // deallocazione fisica del nodo rimosso
            lista->dimensione--; // decremento contatore prenotazioni
            return 0;
        }

        prev = curr; // aggiorna precedente
        curr = curr->next; // passa al successivo
    }
    return -1; // prenotazione non trovata
}


prenotazione cercaPrenotazione(const char *matricola, const char *data, int fasciaOraria) { // ricerca di una prenotazione

    prenotazioneInterna *curr;
    if (!lista || !matricola || !data) 
        return NULL;

    curr = lista->testa; // ricerca lineare nella lista
    while (curr) { // scansione nodi
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) 
            return curr; // trovata

    
        curr = curr->next;
    }
    return NULL; // non trovata
}

static const char *nomiFasce[] = { // array stringhe per fascie orarie
    "09:00 - 11:00",
    "11:00 - 13:00",
    "13:30 - 15:30",
    "15:30 - 17:30",
    "17:30 - 19:30"
};

void visualizzaPrenotazioni() { // stampa a video di tutto l'elenco
    prenotazioneInterna *curr;

    if (!lista || !lista->testa) { // controllo lista vuota
        printf("Nessuna prenotazione attiva.\n");
        return;
    }

    printf("=== Prenotazioni attive (%d) ===\n", lista->dimensione);


    curr = lista->testa;
    while (curr) { // iterazione e stampa dettagli ogni nodo
        printf("Matricola: %s | Data: %s | Fascia: %s | Posto: %d\n", curr->matricola, curr->data, nomiFasce[curr->fasciaOraria], curr->posto);
        curr = curr->next;
    }
}

void visualizzaPrenotazioniPerFascia(int fasciaOraria) { // stampa filtrata per orario
    prenotazioneInterna *curr;

    int trovati = 0;

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) 
        return;


    printf("=== Prenotazioni fascia %s ===\n", nomiFasce[fasciaOraria]);

    curr = lista->testa;
    while (curr) { // scansione lista per filtrare fascia specifica
        if (curr->fasciaOraria == fasciaOraria) {
            printf("Matricola: %s | Data: %s | Posto: %d\n", curr->matricola, curr->data, curr->posto);
            trovati++;
        }


        curr = curr->next;
    }
    if (!trovati) 
        printf("Nessuna prenotazione per questa fascia.\n");
}





const char *getMatricolaPrenotazione(prenotazione p) { // accesso sicuro a matricola prenotazione
    if (p == NULL) 
        return NULL;

    return p->matricola; // ritorna indirizzo stringa matricola
}

const char *getDataPrenotazione(prenotazione p) { // accesso sicuro a data prenotazione
    if (p == NULL) 
        return NULL;

    return p->data; // ritorna indirizzo stringa data
}

int getFasciaOraria(prenotazione p) { // accesso sicuro a fascia oraria
    if (p == NULL) 
        return -1;
    return p->fasciaOraria; // ritorna valore intero fascia
}

int getPostoAssegnato(prenotazione p) { // accesso sicuro a numero posto
    
    if (p == NULL) 
        return -1;

    return p->posto; // ritorna valore intero posto
}