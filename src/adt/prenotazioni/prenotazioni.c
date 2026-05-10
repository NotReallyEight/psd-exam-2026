#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prenotazioni.h"

#define NUM_FASCE 5



typedef struct prenotazioneInterna { // struttura per il singolo nodo della lista
    char *matricola; // puntatore alla stringa matricola
    char *data; // puntatore alla stringa data
    int fasciaOraria; // intero per la fascia oraria
    int posto; // intero per il posto assegnato
    struct prenotazioneInterna *next; // puntatore al nodo successivo
} prenotazioneInterna;


typedef struct listaPrenotazioni { // struttura testata per la gestione lista
    prenotazioneInterna *testa; // puntatore al primo nodo
    int dimensione; // contatore elementi presenti
} listaPrenotazioni;


static listaPrenotazioni *lista = NULL; // riferimento  alla lista

static char *duplicaStringa(const char *src) { // funzione per clonare stringhe
    char *dst; // puntatore destinazione
    if (!src) 
        return NULL; // controllo sorgente nulla

    dst = (char *)malloc(strlen(src) + 1); // allocazione memoria stringa

    if (!dst) 
        return NULL; // controllo fallimento allocazione


    strcpy(dst, src); // copia della stringa
    return dst; // ritorno indirizzo nuova stringa
}
static prenotazioneInterna *creaNodo(const char *matricola, const char *data, int fasciaOraria, int posto) { // alloca un nuovo nodo

    prenotazioneInterna *nodo; // puntatore al nuovo nodo

    nodo = (prenotazioneInterna *)malloc(sizeof(prenotazioneInterna)); // allocazione memoria nodo
    if (!nodo) 
        return NULL; // controllo errore allocazione


    nodo->matricola = duplicaStringa(matricola); // copia della matricola
    nodo->data = duplicaStringa(data); // copia della data
    nodo->fasciaOraria = fasciaOraria; // assegnazione fascia
    nodo->posto = posto; // assegnazione posto
    nodo->next = NULL; // inizializzazione puntatore next


    if (!nodo->matricola || !nodo->data) { // verifica successo duplicazione stringhe
        free(nodo->matricola); // rilascio memoria matricola
        free(nodo->data); // rilascio memoria data
        free(nodo); // rilascio memoria nodo
        return NULL; // ritorno fallimento
    }


    return nodo; // ritorno puntatore nodo pronto
}
static void liberaNodo(prenotazioneInterna *nodo) { // libera la memoria di un nodo
    if (!nodo) 
        return; // protezione nodo nullo


    free(nodo->matricola); // rilascio stringa matricola
    free(nodo->data); // rilascio stringa data
    free(nodo); // rilascio struttura
}


int creaListaPrenotazioni() { // inizializza la struttura lista
    lista = (listaPrenotazioni *)malloc(sizeof(listaPrenotazioni)); // allocazione testata

    if (!lista) 
        return -1; // controllo errore memoria

    lista->testa = NULL; // testa iniziale a null
    lista->dimensione = 0; // dimensione iniziale a zero

    return 0; // successo
}
void distruggiListaPrenotazioni() { // dealloca l'intera lista
    prenotazioneInterna *curr, *next; // puntatore per scorrimento

    if (!lista) 
        return; // protezione lista non esistente


    curr = lista->testa; // inizio dalla testa

    while (curr) { // ciclo fino alla fine
        next = curr->next; // salvo il prossimo
        liberaNodo(curr); // libero il corrente
        curr = next; // passo al prossimo
    }

    free(lista); // libero la testata
    lista = NULL; // azzero puntatore globale
}


int creaPrenotazione(const char *matricola, const char *data, int fasciaOraria, int posto) { // inserimento nuova prenotazione

    prenotazioneInterna *nodo, *curr; // puntatori di appoggio


    if (!lista || !matricola || !data) { // controllo validita parametri
        printf("Errore: parametri non validi.\n"); // messaggio errore parametri
        return -1; // ritorno errore
    }
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) { // controllo range fascia
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1); // errore fascia
        return -1; // ritorno errore
    }
    if (posto < 0) { // controllo validita posto
        printf("Errore: posto non valido.\n"); // errore posto negativo
        return -1; // ritorno errore
    }


    curr = lista->testa; // controllo esistenza duplicato
    while (curr) { // scansione lista
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) { // verifica tripla chiave
            printf("Errore: prenotazione gia' esistente per questa fascia.\n"); // errore duplicato
            return -1; // ritorno errore
        }

        curr = curr->next; // nodo successivo
    }

    nodo = creaNodo(matricola, data, fasciaOraria, posto); // creazione fisica nodo

    if (!nodo) { // verifica allocazione
        printf("Errore: allocazione memoria fallita.\n"); // errore memoria
        return -1; // ritorno errore
    }

    nodo->next = lista->testa; // inserimento in testa
    lista->testa = nodo; // aggiornamento testa
    lista->dimensione++; // incremento dimensione
    return 0; // successo

}
int annullaPrenotazione(const char *matricola, const char *data, int fasciaOraria) { // rimozione di una prenotazione

    prenotazioneInterna *curr, *prev; // puntatori per ricerca

    if (!lista || !matricola || !data) { // controllo parametri
        printf("Errore: parametri non validi.\n"); // errore parametri
        return -1; // ritorno errore
    }
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) { // controllo fascia
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1); // errore fascia
        return -1; // ritorno errore
    }

    curr = lista->testa; // inizio ricerca
    prev = NULL; // tracciamento precedente
    while (curr) { // scansione lista
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) { // corrispondenza trovata
            if (prev) 
                prev->next = curr->next; // aggancio precedente a successivo
            else 
                lista->testa = curr->next; // sposto testa al secondo


            liberaNodo(curr); // deallocazione nodo rimosso
            lista->dimensione--; // decremento contatore

            return 0; // successo
        }

        prev = curr; // aggiornamento precedente
        curr = curr->next; // passaggio al successivo
    }

    printf("Errore: prenotazione non trovata.\n"); // messaggio se non trovato
    return -1; // ritorno errore
}

prenotazione cercaPrenotazione(const char *matricola, const char *data, int fasciaOraria) { // ricerca nodo specifico
    prenotazioneInterna *curr; // puntatore scorrimento

    if (!lista || !matricola || !data) { // controllo input
        printf("Errore: parametri non validi.\n"); // errore input
        return NULL; // ritorno nullo
    }
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) { // controllo fascia
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1); // errore fascia
        return NULL; // ritorno nullo
    }


    curr = lista->testa; // ricerca lineare
    while (curr) { // scansione nodi
        if (strcmp(curr->matricola, matricola) == 0 && strcmp(curr->data, data) == 0 && curr->fasciaOraria == fasciaOraria) 
            return curr; // trovato ritorno puntatore

        curr = curr->next; // prossimo nodo
    }
    return NULL; // non trovato
}

static const char *nomiFasce[] = { // nomi descrittivi fascie orarie
    "09:00 - 11:00",
    "11:00 - 13:00",
    "13:30 - 15:30",
    "15:30 - 17:30",
    "17:30 - 19:30"
};


void visualizzaPrenotazioni() { // stampa 
    prenotazioneInterna *curr; // puntatore scorrimento

    if (!lista || !lista->testa) { // controllo se vuota
        printf("Nessuna prenotazione attiva.\n"); // avviso lista vuota
        return; // fine funzione
    }

    printf("=== Prenotazioni attive (%d) ===\n", lista->dimensione); // stampa testata conteggio

    curr = lista->testa; // inizio stampa
    while (curr) { // ciclo su tutti i nodi
        printf("Matricola: %s | Data: %s | Fascia: %s | Posto: %d\n", curr->matricola, curr->data, nomiFasce[curr->fasciaOraria], curr->posto); // stampa dettagli
        curr = curr->next; // nodo successivo
    }

}



void visualizzaPrenotazioniPerFascia(int fasciaOraria) { // stampa filtrata
    prenotazioneInterna *curr; // puntatore scorrimento

    int trovati = 0; // contatore locale
    if (fasciaOraria < 0 || fasciaOraria >= NUM_FASCE) { // controllo validita fascia
        printf("Errore: fascia oraria non valida. Inserire un valore tra 0 e %d.\n", NUM_FASCE - 1); // errore fascia
        return; // fine funzione
    }


    printf("=== Prenotazioni fascia %s ===\n", nomiFasce[fasciaOraria]); // testata fascia

    curr = lista->testa; // scansione filtrata
    while (curr) { // ciclo nodi
        if (curr->fasciaOraria == fasciaOraria) { // se fascia coincide
            printf("Matricola: %s | Data: %s | Posto: %d\n", curr->matricola, curr->data, curr->posto); // stampa dati
            trovati++; // incremento trovati
        }
        curr = curr->next; // prossimo
    }

    if (!trovati) 
        printf("Nessuna prenotazione per questa fascia.\n"); // messaggio se nessun risultato
}



const char *getMatricolaPrenotazione(prenotazione p) { // getter matricola
    if (p != NULL) 
        return p->matricola; // ritorno dato
    else 
        return NULL; // ritorno nullo
}
const char *getDataPrenotazione(prenotazione p) { // getter data
    if (p != NULL) 
        return p->data; // ritorno dato
    else 
        return NULL; // ritorno nullo
}
int getFasciaOraria(prenotazione p) { // getter fascia
    if (p != NULL) 
        return p->fasciaOraria; // ritorno dato
    else 
        return -1; // ritorno codice errore
}
int getPostoAssegnato(prenotazione p) { // getter posto
    if (p != NULL) 
        return p->posto; // ritorno dato
    else 
        return -1; // ritorno codice errore
}