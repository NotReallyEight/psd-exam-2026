/*
* File: studente.c
* ---
* Implementazione dell'ADT studente definito nel file studente.h.
*
* Il modulo gestisce un archivio studenti tramite tabella hash
* con scansione lineare delle collisioni.
*
* Le specifiche pubbliche sono descritte nel file header;
* qui vengono documentati i dettagli interni.
*
* Autore: Raffaele Severino
* Data inizio: 02/04/2026
* Data ultima modifica: 13/05/2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"

// --- STRUTTURE INTERNE ---

/*
* Record interno di uno studente.
*/
typedef struct studenteInterno {
    char *nome; // Nome dello studente
    char *cognome; // Cognome dello studente
    char *matricola; // Identificativo univoco
    char *corsoDiLaurea; // Corso frequentato
    int occupato; // 1 attivo, 0 libero, -1 eliminato
} studenteInterno;

/*
* Struttura della tabella hash.
*/
typedef struct tabellaStudentiInterna {
    studenteInterno *celle; // Array delle celle
    int capacita; // Numero slot disponibili
    int dimensione; // Numero studenti presenti
} tabellaStudentiInterna;

static tabellaStudentiInterna *tabella = NULL; // Tabella globale del modulo

// --- FUNZIONI INTERNE ---

/*
* Funzione: hash
* ---
* Calcola l'indice iniziale associato a una matricola.
*
* Usa djb2, funzione semplice e adatta a chiavi testuali.
*/
static unsigned int hash(const char *matricola, int capacita) {
    unsigned int h = 5381;

    while (*matricola)
        h = ((h << 5) + h) + (unsigned char)(*matricola++);

    return h % (unsigned int)capacita;
}

/*
* Funzione: duplicaStringa
* ---
* Alloca una copia indipendente della stringa ricevuta.
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
* Funzione: trovaPosizione
* ---
* Cerca una matricola già presente nella tabella.
*
* La scansione continua finché non trova la chiave
* oppure uno slot mai usato.
*/
static int trovaPosizione(const char *matricola) {
    unsigned int idx = hash(matricola, tabella->capacita);
    int i;

    for (i = 0; i < tabella->capacita; i++) {
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita;

        // Slot mai usato: la chiave non può trovarsi oltre
        if (tabella->celle[pos].occupato == 0)
            return -1;

        if (tabella->celle[pos].occupato == 1 &&
            strcmp(tabella->celle[pos].matricola, matricola) == 0)
            return (int)pos;
    }

    return -1;
}

/*
* Funzione: trovaSlotInserimento
* ---
* Cerca la posizione migliore per un nuovo inserimento.
*
* Se incontra slot eliminati li memorizza, così potranno
* essere riutilizzati senza interrompere la ricerca.
*/
static int trovaSlotInserimento(const char *matricola) {
    unsigned int idx = hash(matricola, tabella->capacita);
    int primoTombstone = -1;
    int i;

    for (i = 0; i < tabella->capacita; i++) {
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita;

        /*
        * Slot mai usato:
        * se esiste un tombstone conviene riutilizzarlo,
        * altrimenti si usa questa cella libera.
        */
        if (tabella->celle[pos].occupato == 0) {
            if (primoTombstone != -1)
                return primoTombstone;
            else
                return (int)pos;
        }

        /*
        * Il primo tombstone viene salvato perché rappresenta
        * il miglior slot riutilizzabile trovato finora.
        */
        if (tabella->celle[pos].occupato == -1 &&
            primoTombstone == -1)
            primoTombstone = (int)pos;

        // Blocca inserimenti con matricola duplicata
        if (tabella->celle[pos].occupato == 1 &&
            strcmp(tabella->celle[pos].matricola, matricola) == 0)
            return -1;
    }

    return primoTombstone;
}

/*
* Funzione: rehash
* ---
* Espande la tabella quando piena.
*
* Gli elementi attivi vengono ricollocati usando
* la nuova capacità per migliorare distribuzione e spazio.
*/
static int rehash() {
    int nuovaCapacita = tabella->capacita + 20;
    int i;

    studenteInterno *nuoveCelle = (studenteInterno *)calloc(nuovaCapacita, sizeof(studenteInterno));

    if (!nuoveCelle)
        return -1;

    for (i = 0; i < tabella->capacita; i++) {

        // Solo gli elementi validi devono essere trasferiti
        if (tabella->celle[i].occupato == 1) {
            unsigned int idx = hash(tabella->celle[i].matricola, nuovaCapacita);
            int j;

            for (j = 0; j < nuovaCapacita; j++) {
                unsigned int pos = (idx + j) % (unsigned int)nuovaCapacita;

                if (nuoveCelle[pos].occupato == 0) {

                    /*
                    * Si spostano i puntatori esistenti senza duplicare
                    * le stringhe, evitando copie inutili di memoria.
                    */
                    nuoveCelle[pos].matricola = tabella->celle[i].matricola;
                    nuoveCelle[pos].nome = tabella->celle[i].nome;
                    nuoveCelle[pos].cognome = tabella->celle[i].cognome;
                    nuoveCelle[pos].corsoDiLaurea = tabella->celle[i].corsoDiLaurea;

                    nuoveCelle[pos].occupato = 1;
                    break;
                }
            }
        }
    }

    free(tabella->celle);
    tabella->celle = nuoveCelle;
    tabella->capacita = nuovaCapacita;

    return 0;
}

// --- CICLO DI VITA ---

/*
* Funzione: creaTabella
* ---
* Alloca e inizializza la struttura principale.
*/
int creaTabella(int capacita) {

    if (capacita <= 0) {
        printf("Errore: capacita non valida.\n");
        return -1;
    }

    if (tabella != NULL) {
        printf("Errore: tabella gia inizializzata.\n");
        return -1;
    }

    tabella = (tabellaStudentiInterna *)malloc(sizeof(tabellaStudentiInterna));

    if (!tabella)
        return -1;

    tabella->celle = (studenteInterno *)calloc(capacita, sizeof(studenteInterno));

    if (!tabella->celle) {
        free(tabella);
        tabella = NULL;
        return -1;
    }

    tabella->capacita = capacita;
    tabella->dimensione = 0;

    return 0;
}

/*
* Funzione: distruggiTabella
* ---
* Libera tutta la memoria associata alla tabella.
*/
void distruggiTabella() {
    int i;

    if (!tabella)
        return;

    for (i = 0; i < tabella->capacita; i++) {

        // Solo celle attive contengono stringhe allocate
        if (tabella->celle[i].occupato == 1) {
            free(tabella->celle[i].matricola);
            free(tabella->celle[i].nome);
            free(tabella->celle[i].cognome);
            free(tabella->celle[i].corsoDiLaurea);
        }
    }

    free(tabella->celle);
    free(tabella);
    tabella = NULL;
}

// --- OPERAZIONI PRINCIPALI ---

/*
* Funzione: creaStudente
* ---
* Inserisce un nuovo studente nella tabella.
*/
int creaStudente(const char *nome, const char *cognome, const char *matricola, const char *corsoDiLaurea) {
    int slot;

    if (!tabella || !nome || !cognome || !matricola || !corsoDiLaurea) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    // Se piena viene espansa prima dell'inserimento
    if (tabella->dimensione == tabella->capacita)
        if (rehash() == -1)
            return -1;

    slot = trovaSlotInserimento(matricola);

    if (slot == -1) {
        printf("Errore: matricola gia esistente.\n");
        return -1;
    }

    /*
    * Si copiano i dati testuali in memoria autonoma,
    * così la struttura non dipende dalle stringhe esterne.
    */
    tabella->celle[slot].matricola = duplicaStringa(matricola);
    tabella->celle[slot].nome = duplicaStringa(nome);
    tabella->celle[slot].cognome = duplicaStringa(cognome);
    tabella->celle[slot].corsoDiLaurea = duplicaStringa(corsoDiLaurea);

    /*
    * Se una qualunque allocazione fallisce,
    * vengono liberate quelle già riuscite.
    */
    if (!tabella->celle[slot].matricola ||
        !tabella->celle[slot].nome ||
        !tabella->celle[slot].cognome ||
        !tabella->celle[slot].corsoDiLaurea) {

        free(tabella->celle[slot].matricola);
        free(tabella->celle[slot].nome);
        free(tabella->celle[slot].cognome);
        free(tabella->celle[slot].corsoDiLaurea);

        return -1;
    }

    tabella->celle[slot].occupato = 1;
    tabella->dimensione++;

    return 0;
}

/*
* Funzione: eliminaStudente
* ---
* Elimina lo studente indicato dalla matricola.
*/
int eliminaStudente(const char *matricola) {
    int pos;

    if (!tabella || !matricola) {
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    pos = trovaPosizione(matricola);

    if (pos == -1) {
        printf("Errore: studente non trovato.\n");
        return -1;
    }

    free(tabella->celle[pos].matricola);
    free(tabella->celle[pos].nome);
    free(tabella->celle[pos].cognome);
    free(tabella->celle[pos].corsoDiLaurea);

    /*
    * Lo slot non viene marcato libero ma tombstone,
    * così la ricerca hash futura resta corretta.
    */
    tabella->celle[pos].matricola = NULL;
    tabella->celle[pos].nome = NULL;
    tabella->celle[pos].cognome = NULL;
    tabella->celle[pos].corsoDiLaurea = NULL;
    tabella->celle[pos].occupato = -1;

    tabella->dimensione--;

    return 0;
}

/*
* Funzione: cercaStudente
* ---
* Cerca e restituisce lo studente richiesto.
*/
studente cercaStudente(const char *matricola) {
    int pos;

    if (!tabella || !matricola)
        return NULL;

    pos = trovaPosizione(matricola);

    if (pos == -1)
        return NULL;

    return &tabella->celle[pos];
}

// --- VISUALIZZAZIONE ---

/*
* Funzione: stampaStudente
* ---
* Mostra a video i dati di uno studente.
*/
void stampaStudente(studente s) {

    if (!s) {
        printf("Errore: studente non valido.\n");
        return;
    }

    printf("Matricola: %s\n", s->matricola);
    printf("Nome: %s\n", s->nome);
    printf("Cognome: %s\n", s->cognome);
    printf("Corso: %s\n", s->corsoDiLaurea);
}

/*
* Funzione: stampaTabella
* ---
* Visualizza tutti gli studenti registrati.
*/
void stampaTabella() {
    int i;

    if (!tabella) {
        printf("Errore: tabella non inizializzata.\n");
        return;
    }

    if (tabella->dimensione == 0) {
        printf("Nessuno studente registrato.\n");
        return;
    }

    printf("=== Studenti registrati (%d) ===\n", tabella->dimensione);

    for (i = 0; i < tabella->capacita; i++)
        if (tabella->celle[i].occupato == 1)
            stampaStudente(&tabella->celle[i]);
}

// --- GETTER ---

const char *getMatricola(studente s) {
    if (s != NULL)
        return s->matricola;
    else
        return NULL;
}

const char *getNome(studente s) {
    if (s != NULL)
        return s->nome;
    else
        return NULL;
}

const char *getCognome(studente s) {
    if (s != NULL)
        return s->cognome;
    else
        return NULL;
}

const char *getCorsoDiLaurea(studente s) {
    if (s != NULL)
        return s->corsoDiLaurea;
    else
        return NULL;
}