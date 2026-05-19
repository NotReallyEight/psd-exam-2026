/*
* File: listaAttesa.c
* ---
* Implementazione dell'ADT listaAttesa definito nel file listaAttesa.h.
*
* Questo modulo gestisce una lista d'attesa suddivisa in più fasce
* orarie indipendenti, ciascuna realizzata tramite code FIFO.
*
* Le specifiche pubbliche delle funzioni sono già documentate nel file
* header; in questo file vengono descritti i dettagli implementativi.
*
* Autore: Raffaele Severino
* Data inizio: 02/04/2026
* Data ultima modifica: 18/05/2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaAttesa.h"

// --- STRUTTURE INTERNE ---

/*
* Nodo della lista concatenata usata per rappresentare la coda.
* Ogni nodo identifica uno studente in attesa.
*/
typedef struct elementoCodaInterno {
    char *matricola; // Copia dinamica della matricola
    int fasciaOraria; // Fascia richiesta dallo studente
    struct elementoCodaInterno *next; // Collegamento al nodo successivo
} elementoCodaInterno;

/*
* Struttura interna di una singola coda FIFO.
* Mantiene testa e coda per inserimenti efficienti.
*/
typedef struct codaInterna {
    elementoCodaInterno *testa; // Primo elemento in attesa
    elementoCodaInterno *coda; // Ultimo elemento in attesa
    int dimensione; // Numero elementi presenti
} codaInterna;

static codaInterna code[NUM_FASCE]; // Una coda per ogni fascia oraria
static int inizializzato = 0; // Flag di inizializzazione

// --- FUNZIONI INTERNE ---

/*
* Funzione: duplicaStringa
* ---
* Crea una copia dinamica della stringa ricevuta.
*
* Viene usata per rendere autonoma la gestione della matricola
* memorizzata nei nodi della coda.
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
* Alloca e inizializza un nuovo nodo della coda.
*/
static elementoCodaInterno *creaNodo(const char *matricola,
                                     int fasciaOraria) {
    elementoCodaInterno *nodo;

    nodo = (elementoCodaInterno *)
           malloc(sizeof(elementoCodaInterno));

    if (!nodo)
        return NULL;

    nodo->matricola = duplicaStringa(matricola);

    if (!nodo->matricola) {
        free(nodo);
        return NULL;
    }

    nodo->fasciaOraria = fasciaOraria;
    nodo->next = NULL;

    return nodo;
}

/*
* Funzione: liberaNodo
* ---
* Dealloca completamente un nodo.
*/
static void liberaNodo(elementoCodaInterno *nodo) {
    if (!nodo)
        return;

    free(nodo->matricola);
    free(nodo);
}

/*
* Funzione: inizializzaCoda
* ---
* Inizializza tutte le code come vuote.
*
* L'operazione viene eseguita solo al primo utilizzo.
*/
static void inizializzaCoda() {
    int i;

    for (i = 0; i < NUM_FASCE; i++) {
        code[i].testa = NULL;
        code[i].coda = NULL;
        code[i].dimensione = 0;
    }

    inizializzato = 1;
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

// --- OPERAZIONI PRINCIPALI ---

/*
* Funzione: aggiungi_inCoda
* ---
* Inserisce uno studente in fondo alla coda selezionata.
*
* Prima dell'inserimento viene controllata l'assenza
* di duplicati nella stessa fascia.
*/
int aggiungi_inCoda(const char *matricola, int fasciaOraria) {
    elementoCodaInterno *nodo, *curr;

    if (!inizializzato)
        inizializzaCoda();

    if (!matricola) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    curr = code[fasciaOraria].testa;

    while (curr) {
        if (strcmp(curr->matricola, matricola) == 0) {
            printf("Errore: studente gia' in attesa per questa fascia.\n");
            return -1;
        }

        curr = curr->next;
    }

    nodo = creaNodo(matricola, fasciaOraria);

    if (!nodo) {
        printf("Errore: allocazione memoria fallita.\n");
        return -1;
    }

    // Se la coda è vuota il nodo diventa testa e coda
    if (code[fasciaOraria].coda == NULL) {
        code[fasciaOraria].testa = nodo;
        code[fasciaOraria].coda = nodo;
    } else {
        code[fasciaOraria].coda->next = nodo;
        code[fasciaOraria].coda = nodo;
    }

    code[fasciaOraria].dimensione++;

    return 0;
}

/*
* Funzione: estraiDaCoda
* ---
* Rimuove e restituisce il primo elemento della coda.
*
* Il nodo restituito dovrà essere liberato dal chiamante.
*/
elementoCoda estraiDaCoda(int fasciaOraria) {
    elementoCodaInterno *nodo;

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return NULL;
    }

    if (code[fasciaOraria].testa == NULL) {
        printf("Errore: nessuno studente in attesa per la fascia %s.\n", nomiFasce[fasciaOraria]);
        return NULL;
    }

    nodo = code[fasciaOraria].testa;
    code[fasciaOraria].testa = nodo->next;

    // Se la coda diventa vuota aggiorna anche il fondo
    if (code[fasciaOraria].testa == NULL)
        code[fasciaOraria].coda = NULL;

    nodo->next = NULL;
    code[fasciaOraria].dimensione--;

    return nodo;
}

/*
* Funzione: testa
* ---
* Restituisce il primo elemento senza rimuoverlo.
*/
elementoCoda testa(int fasciaOraria) {
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return NULL;
    }

    if (code[fasciaOraria].testa == NULL) {
        printf("Errore: nessuno studente in attesa per la fascia %s.\n", nomiFasce[fasciaOraria]);
        return NULL;
    }

    return code[fasciaOraria].testa;
}

/*
* Funzione: rimuoviDaCoda
* ---
* Cerca una matricola nella coda e la rimuove.
*/
int rimuoviDaCoda(const char *matricola, int fasciaOraria) {
    elementoCodaInterno *curr, *prev;

    if (!matricola) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    curr = code[fasciaOraria].testa;
    prev = NULL;

    while (curr) {
        if (strcmp(curr->matricola, matricola) == 0) {

            // Se esiste un precedente collega al successivo
            if (prev)
                prev->next = curr->next;
            else
                code[fasciaOraria].testa = curr->next;

            // Se viene rimosso l'ultimo aggiorna la coda
            if (curr->next == NULL)
                code[fasciaOraria].coda = prev;

            liberaNodo(curr);
            code[fasciaOraria].dimensione--;

            return 0;
        }

        prev = curr;
        curr = curr->next;
    }

    printf("Errore: studente non trovato in coda per la fascia %s.\n",nomiFasce[fasciaOraria]);

    return -1;
}

// --- VISUALIZZAZIONE ---

/*
* Funzione: visualizzaCoda
* ---
* Mostra a video tutti gli elementi presenti
* nella fascia selezionata.
*/
void visualizzaCoda(int fasciaOraria) {
    elementoCodaInterno *curr;
    int posizione = 1;

    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return;
    }

    if (code[fasciaOraria].testa == NULL) {
        printf("Nessuno studente in attesa per la fascia %s.\n", nomiFasce[fasciaOraria]);
        return;
    }

    printf("=== Coda fascia %s (%d in attesa) ===\n",
           nomiFasce[fasciaOraria], code[fasciaOraria].dimensione);

    curr = code[fasciaOraria].testa;

    while (curr) {
        printf("%d. Matricola: %s\n", posizione, curr->matricola);

        posizione++;
        curr = curr->next;
    }
}

/*
* Funzione: visualizzaTutteLeCoda
* ---
* Mostra lo stato completo di tutte le code.
*/
void visualizzaTutteLeCoda() {
    int i;

    if (!inizializzato)
        inizializzaCoda();

    printf("=== Stato lista attesa tutte le fasce ===\n");

    for (i = 0; i < NUM_FASCE; i++)
        visualizzaCoda(i);
}

// --- UTILITA' ---

/*
* Funzione: codaVuota
* ---
* Verifica se una coda è priva di elementi.
*/
int codaVuota(int fasciaOraria) {
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    if (code[fasciaOraria].testa == NULL)
        return 1;

    return 0;
}

/*
* Funzione: dimensioneCoda
* ---
* Restituisce il numero di elementi presenti.
*/
int dimensioneCoda(int fasciaOraria) {
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) {
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1);
        return -1;
    }

    return code[fasciaOraria].dimensione;
}



/*
* Funzione: resetCoda
* ---
* Svuota tutte le code e reimposta lo stato iniziale.
*/
void resetCoda() {
    int i;
    elementoCodaInterno *curr, *next;

    for (i = 0; i < NUM_FASCE; i++) {
        curr = code[i].testa;

        while (curr) {
            next = curr->next;
            liberaNodo(curr);
            curr = next;
        }

        code[i].testa = NULL;
        code[i].coda = NULL;
        code[i].dimensione = 0;
    }

    inizializzato = 0;
}

// --- GETTER ---

/*
* Funzione: getMatricolaCoda
* ---
* Restituisce la matricola contenuta nel nodo.
*/
const char *getMatricolaCoda(elementoCoda e) {
    if (e != NULL)
        return e->matricola;
    else
        return NULL;
}

/*
* Funzione: getFasciaCoda
* ---
* Restituisce la fascia associata al nodo.
*/
int getFasciaCoda(elementoCoda e) {
    if (e != NULL)
        return e->fasciaOraria;
    else
        return -1;
}