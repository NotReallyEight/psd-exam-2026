
/*
 * File: prenotazioni.c
 * ---
 * Implementazione dell'ADT prenotazioni
 * definito nel file prenotazioni.h.
 *
 * Il modulo gestisce una lista concatenata
 * di prenotazioni identificate dalla combinazione
 * univoca matricola + data + fasciaOraria.
 *
 * Le specifiche pubbliche sono descritte nel file
 * header; qui vengono documentati i dettagli interni.
 *
 * Autore: Raffaele Severino
 * Data inizio: 02/04/2026
 * Data ultima modifica: 13/05/2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prenotazioni.h"

#define NUM_FASCE 5

// --- STRUTTURE INTERNE ---

/*
 * Nodo della lista concatenata.
 * Ogni nodo rappresenta una singola prenotazione.
 */
typedef struct prenotazioneInterna {
    char *matricola; // puntatore alla stringa matricola
    char *data; // puntatore alla stringa data
    int fasciaOraria; // intero per la fascia oraria
    int posto; // intero per il posto assegnato
    struct prenotazioneInterna *next; // puntatore al nodo successivo
} prenotazioneInterna;



/*
 * Struttura della lista prenotazioni.
 * Mantiene testa e contatore per gestione efficiente.
 */
typedef struct listaPrenotazioni {
    prenotazioneInterna *testa; // puntatore al primo nodo
    int dimensione; // contatore elementi presenti
} listaPrenotazioni;

static listaPrenotazioni *lista = NULL; // Lista globale del modulo

// --- FUNZIONI INTERNE ---

/*
 * Funzione: duplicaStringa
 * ---
 * Crea una copia dinamica della stringa ricevuta.
 *
 * Viene usata per rendere autonoma la gestione
 * delle stringhe memorizzate nei nodi della lista.
 */
static char *duplicaStringa(const char *src) {
    char *dst;

    if (!src)
        return NULL;

    dst = (char *)malloc(strlen(src) + 1);

    if (!dst)
        return NULL;

    strcpy(dst, src);

    return dst;
}


/*
 * Funzione: creaNodo
 * ---
 * Alloca e inizializza un nuovo nodo della lista.
 *
 * Se una delle stringhe non viene allocata
 * correttamente, libera tutto e ritorna NULL.
 */
static prenotazioneInterna *creaNodo(const char *matricola, const char *data, int fasciaOraria,int posto) {
    prenotazioneInterna *nodo;

    nodo = (prenotazioneInterna *)malloc(sizeof(prenotazioneInterna));

    if (!nodo)
        return NULL;

    nodo->matricola = duplicaStringa(matricola);
    nodo->data = duplicaStringa(data);
    nodo->fasciaOraria = fasciaOraria;
    nodo->posto = posto;
    nodo->next = NULL;

    if (!nodo->matricola || !nodo->data) {

        free(nodo->matricola);
        free(nodo->data);
        free(nodo);
        return NULL;
    }

    return nodo;
}


/*
 * Funzione: liberaNodo
 * ---
 * Dealloca completamente un nodo.
 */
static void liberaNodo(prenotazioneInterna *nodo) {

    if (!nodo)
        return;

    free(nodo->matricola);
    free(nodo->data);
    free(nodo);
}

/*
 * Nomi descrittivi delle fasce orarie.
 * Utilizzati nei messaggi a schermo.
 */
static const char *nomiFasce[] = {
    "09:00 - 11:00",
    "11:00 - 13:00",
    "13:30 - 15:30",
    "15:30 - 17:30",
    "17:30 - 19:30"
};

// --- CICLO DI VITA ---

/*
 * Funzione: creaListaPrenotazioni
 * ---
 * Alloca e inizializza la struttura principale.
 */
int creaListaPrenotazioni() {

    lista = (listaPrenotazioni *)malloc(sizeof(listaPrenotazioni));

    if (!lista)
        return -1;

    lista->testa = NULL;
    lista->dimensione = 0;

    return 0;
}

/*
 * Funzione: distruggiListaPrenotazioni
 * ---
 * Libera tutta la memoria associata alla lista.
 */
void distruggiListaPrenotazioni() {
    prenotazioneInterna *curr, *next;

    if (!lista)
        return;

    curr = lista->testa;

    while (curr) {
        next = curr->next;
        liberaNodo(curr);
        curr = next;
    }

    free(lista);
    lista = NULL;
}

// --- FUNZIONI PER OPERAZIONI DI BASE ---

/*
 * Funzione: creaPrenotazione
 * ---
 * Inserisce una nuova prenotazione in testa alla lista.
 *
 * L'inserimento in testa è O(1) ed è sufficiente
 * perché la lista non richiede ordinamento.
 */
int creaPrenotazione(const char *matricola, const char *data, int fasciaOraria, int posto) {

    prenotazioneInterna *nodo, *curr;

    if (!lista || !matricola || !data) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida.\n Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    if (posto < 0) {
        printf("Errore: posto non valido.\n");
        return -1;
    }

    /*
     * La chiave è composta da tre campi: matricola,
     * data e fasciaOraria. Tutti e tre devono coincidere
     * per rilevare un duplicato.
     */
    curr = lista->testa;

    while (curr) {
        if (strcmp(curr->matricola, matricola) == 0 &&
            strcmp(curr->data, data) == 0 &&
            curr->fasciaOraria == fasciaOraria) {
            printf("Errore: prenotazione gia' esistente per questa fascia.\n");
            return -1;
        }

        curr = curr->next;
    }

    nodo = creaNodo(matricola, data, fasciaOraria, posto);

    if (!nodo) {
        printf("Errore: allocazione memoria fallita.\n");
        return -1;
    }

    nodo->next = lista->testa;
    lista->testa = nodo;
    lista->dimensione++;

    return 0;
}

/*
 * Funzione: annullaPrenotazione
 * ---
 * Rimuove la prenotazione corrispondente alla
 * combinazione matricola + data + fasciaOraria.
 */
int annullaPrenotazione(const char *matricola, const char *data, int fasciaOraria) {
    prenotazioneInterna *curr, *prev;

    if (!lista || !matricola || !data) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida.\n Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    curr = lista->testa;
    prev = NULL;

    while (curr) {
        if (strcmp(curr->matricola, matricola) == 0 &&
            strcmp(curr->data, data) == 0 &&
            curr->fasciaOraria == fasciaOraria) {

            // Se esiste un precedente collega al successivo
            if (prev)
                prev->next = curr->next;
            else
                lista->testa = curr->next;

            liberaNodo(curr);
            lista->dimensione--;

            return 0;
        }

        prev = curr;
        curr = curr->next;
    }

    printf("Errore: prenotazione non trovata.\n");
    return -1;
}

/*
 * Funzione: cercaPrenotazione
 * ---
 * Ricerca lineare sulla lista tramite chiave composta.
 */
prenotazione cercaPrenotazione(const char *matricola, const char *data, int fasciaOraria) {
    prenotazioneInterna *curr;

    if (!lista || !matricola || !data) {
        printf("Errore: parametri non validi.\n");
        return NULL;
    }

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida.\n Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return NULL;
    }

    curr = lista->testa;

    while (curr) {
        if (strcmp(curr->matricola, matricola) == 0 &&
            strcmp(curr->data, data) == 0 &&
            curr->fasciaOraria == fasciaOraria)
            return curr;

        curr = curr->next;
    }

    return NULL;
}

// --- VISUALIZZAZIONI ---

/*
 * Funzione: visualizzaPrenotazioni
 * ---
 * Mostra a video tutte le prenotazioni presenti.
 */
void visualizzaPrenotazioni() {
    prenotazioneInterna *curr;

    if (!lista || !lista->testa) {
        printf("Nessuna prenotazione attiva.\n");
        return;
    }

    printf("=== Prenotazioni attive (%d) ===\n", lista->dimensione);

    curr = lista->testa;

    while (curr) {
        printf("Matricola: %s | Data: %s | Fascia: %s | Posto: %d\n",
                curr->matricola, curr->data, nomiFasce[curr->fasciaOraria], curr->posto);

        curr = curr->next;
    }
}

/*
 * Funzione: visualizzaPrenotazioniPerFascia
 * ---
 * Mostra a video le prenotazioni della fascia indicata.
 */
void visualizzaPrenotazioniPerFascia(int fasciaOraria) {
    prenotazioneInterna *curr;
    int trovati = 0;

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida.\n Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return;
    }

    printf("=== Prenotazioni fascia %s ===\n", nomiFasce[fasciaOraria]);

    curr = lista->testa;

    while (curr) {
        if (curr->fasciaOraria == fasciaOraria) {
            printf("Matricola: %s | Data: %s | Posto: %d\n", curr->matricola, curr->data, curr->posto);

            trovati++;
        }

        curr = curr->next;
    }

    if (!trovati)
        printf("Nessuna prenotazione per questa fascia.\n");
}

// --- GETTER ---

/*
 * Funzione: getMatricolaPrenotazione
 * ---
 * Restituisce la matricola della prenotazione.
 */
const char *getMatricolaPrenotazione(prenotazione p) {
    if (p != NULL)
        return p->matricola;
    else
        return NULL;
}

/*
 * Funzione: getDataPrenotazione
 * ---
 * Restituisce la data della prenotazione.
 */
const char *getDataPrenotazione(prenotazione p) {
    if (p != NULL)
        return p->data;
    else
        return NULL;
}

/*
 * Funzione: getFasciaOraria
 * ---
 * Restituisce la fascia oraria della prenotazione.
 */
int getFasciaOraria(prenotazione p) {
    if (p != NULL)
        return p->fasciaOraria;
    else
        return -1;
}

/*
 * Funzione: getPostoAssegnato
 * ---
 * Restituisce il posto assegnato alla prenotazione.
 */
int getPostoAssegnato(prenotazione p) {
    if (p != NULL)
        return p->posto;
    else
        return -1;
}