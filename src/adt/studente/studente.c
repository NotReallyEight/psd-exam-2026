#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studente.h"

// STRUTTURE INTERNE

typedef struct studenteInterno {
    char *nome; // stringa per allocazione dinamica del nome
    char *cognome; // stringa per allocazione dinamica del cognome
    char *matricola; // chiave univoca per identificazione e hashing
    char *corsoDiLaurea; // stringa per il corso di studi dello studente
    int occupato; // stato cella: 1 attivo, 0 mai usata, -1 rimosso (tombstone)
} studenteInterno;

typedef struct tabellaStudentiInterna {
    studenteInterno *celle; // puntatore all'array di record studenteInterno
    int capacita; // numero totale di slot fisici allocati nell'array
    int dimensione; // conteggio effettivo degli studenti inseriti e attivi
} tabellaStudentiInterna;

static tabellaStudentiInterna *tabella = NULL; // riferimento statico alla tabella

// FUNZIONI INTERNE

static unsigned int hash(const char *matricola, int capacita) { // funzione hash djb2
    unsigned int h = 5381; // valore standard iniziale per la distribuzione

    while (*matricola) // ciclo finche non si raggiunge il terminatore della stringa
        h = ((h << 5) + h) + (unsigned char)(*matricola++); // algoritmo: h * 33 + codice ascii carattere

    return h % (unsigned int)capacita; // mapping dell'hash nel range degli indici della tabella
}

static char *duplicaStringa(const char *src) { // funzione per duplicare stringhe in memoria
    char *dst; // puntatore per la nuova stringa destinazione

    if (!src)
        return NULL; // controllo sorgente nulla

    dst = (char *)malloc(strlen(src) + 1); // allocazione memoria: lunghezza stringa piu terminatore \0

    if (!dst)
        return NULL; // controllo fallimento allocazione

    strcpy(dst, src); // copia fisica dei caratteri dalla sorgente alla destinazione

    return dst; // ritorna l'indirizzo della nuova stringa allocata
}

static int trovaPosizione(const char *matricola) { // cerca l'indice di una matricola esistente
    unsigned int idx = hash(matricola, tabella->capacita); // calcolo indice di partenza tramite hash
    int i; // contatore per scansione lineare delle collisioni

    for (i = 0; i < tabella->capacita; i++) { // ciclo di ricerca su tutta la capacita massima
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita; // calcolo posizione con scansione lineare

        if (tabella->celle[pos].occupato == 0)
            return -1; // cella vuota: lo studente non esiste

        if (tabella->celle[pos].occupato == 1 &&
            strcmp(tabella->celle[pos].matricola, matricola) == 0)
            return (int)pos; // matricola trovata: ritorna indice
    }

    return -1; // studente non presente dopo scansione completa
}

static int trovaSlotInserimento(const char *matricola) { // cerca posto per nuovo inserimento
    unsigned int idx = hash(matricola, tabella->capacita); // indice di partenza calcolato sulla chiave
    int primoTombstone = -1, i; // tracciamento primo slot eliminato e contatore

    for (i = 0; i < tabella->capacita; i++) { // scansione lineare per risoluzione collisioni
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita; // calcolo indice successivo

        if (tabella->celle[pos].occupato == 0) { // cella mai usata
            if (primoTombstone != -1)
                return primoTombstone; // usa il tombstone trovato prima
            else
                return (int)pos; // usa la cella libera
        }

        if (tabella->celle[pos].occupato == -1 && primoTombstone == -1)
            primoTombstone = (int)pos; // salva posizione tombstone per riutilizzo

        if (tabella->celle[pos].occupato == 1 &&
            strcmp(tabella->celle[pos].matricola, matricola) == 0)
            return -1; // matricola gia presente: impedisce duplicati
    }

    return primoTombstone; // tombstone disponibile se tabella satura di cancellati
}

static int rehash() { // espande la tabella quando piena
    int nuovaCapacita = tabella->capacita + 20; // nuova dimensione aumentata di 20 slot
    int i; // indice per iterare sulla vecchia tabella

    studenteInterno *nuoveCelle = (studenteInterno *)calloc(nuovaCapacita, sizeof(studenteInterno)); // nuova memoria azzerata

    if (!nuoveCelle)
        return -1; // fallimento allocazione nuova area di memoria

    for (i = 0; i < tabella->capacita; i++) { // scansione di ogni cella della tabella originale

        if (tabella->celle[i].occupato == 1) { // processa solo gli elementi attivi
            unsigned int idx = hash(tabella->celle[i].matricola, nuovaCapacita); // nuovo hash con nuova capacita
            int j; // indice per posizionamento nella nuova tabella

            for (j = 0; j < nuovaCapacita; j++) { // ricerca nuovo slot libero per il trasferimento
                unsigned int pos = (idx + j) % (unsigned int)nuovaCapacita; // linear probing sulla nuova tabella

                if (nuoveCelle[pos].occupato == 0) { // slot libero trovato nella nuova tabella
                    nuoveCelle[pos].matricola = tabella->celle[i].matricola; // sposta puntatore matricola
                    nuoveCelle[pos].nome = tabella->celle[i].nome; // sposta puntatore nome
                    nuoveCelle[pos].cognome = tabella->celle[i].cognome; // sposta puntatore cognome
                    nuoveCelle[pos].corsoDiLaurea = tabella->celle[i].corsoDiLaurea; // sposta puntatore corso
                    nuoveCelle[pos].occupato = 1; // marca cella nella nuova tabella come attiva
                    break; // passa al prossimo studente da trasferire
                }
            }
        }
    }

    free(tabella->celle); // libera array celle vecchio (non le stringhe, gia spostate)
    tabella->celle = nuoveCelle; // collega nuovo array alla struttura principale
    tabella->capacita = nuovaCapacita; // aggiorna valore capacita massima

    return 0; // rehashing completato con successo
}

// CICLO DI VITA

int creaTabella(int capacita) { // inizializzazione della struttura dati

    if (capacita <= 0) { // controllo validita parametro di input
        printf("Errore: capacita non valida.\n");
        return -1;
    }

    if (tabella != NULL) { // controllo doppia inizializzazione
        printf("Errore: tabella gia inizializzata.\n");
        return -1;
    }

    tabella = (tabellaStudentiInterna *)malloc(sizeof(tabellaStudentiInterna)); // alloca memoria header tabella

    if (!tabella)
        return -1; // controllo errore malloc header

    tabella->celle = (studenteInterno *)calloc(capacita, sizeof(studenteInterno)); // alloca array celle e azzera byte

    if (!tabella->celle) {
        free(tabella);
        tabella = NULL;
        return -1; // controllo errore calloc celle
    }

    tabella->capacita = capacita; // imposta capacita iniziale fornita
    tabella->dimensione = 0; // inizializza contatore elementi a zero

    return 0; // tabella creata con successo
}

void distruggiTabella() { // deallocazione completa della memoria
    int i; // indice per iterare tra le celle

    if (!tabella)
        return; // tabella non esistente: nessuna operazione

    for (i = 0; i < tabella->capacita; i++) { // ciclo per liberare ogni stringa allocata

        if (tabella->celle[i].occupato == 1) { // solo celle attive contengono stringhe da liberare
            free(tabella->celle[i].matricola); // dealloca stringa matricola
            free(tabella->celle[i].nome); // dealloca stringa nome
            free(tabella->celle[i].cognome); // dealloca stringa cognome
            free(tabella->celle[i].corsoDiLaurea); // dealloca stringa corso di laurea
        }
    }

    free(tabella->celle); // libera array di strutture studenteInterno
    free(tabella); // libera struttura header della tabella
    tabella = NULL; // azzera puntatore globale per sicurezza
}

// FUNZIONI PER OPERAZIONI DI BASE

int creaStudente(const char *nome, const char *cognome,
                 const char *matricola, const char *corsoDiLaurea) { // inserimento nuovo studente
    int slot; // variabile per contenere l'indice di destinazione

    if (!tabella || !nome || !cognome || !matricola || !corsoDiLaurea) { // validazione parametri
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    if (tabella->dimensione == tabella->capacita) // tabella fisicamente piena
        if (rehash() == -1)
            return -1; // espansione fallita

    slot = trovaSlotInserimento(matricola); // cerca posizione ottimale (libera o tombstone)

    if (slot == -1) { // matricola gia presente
        printf("Errore: matricola gia esistente.\n");
        return -1;
    }

    tabella->celle[slot].matricola = duplicaStringa(matricola); // alloca e copia matricola
    tabella->celle[slot].nome = duplicaStringa(nome); // alloca e copia nome
    tabella->celle[slot].cognome = duplicaStringa(cognome); // alloca e copia cognome
    tabella->celle[slot].corsoDiLaurea = duplicaStringa(corsoDiLaurea); // alloca e copia corso

    if (!tabella->celle[slot].matricola ||
        !tabella->celle[slot].nome ||
        !tabella->celle[slot].cognome ||
        !tabella->celle[slot].corsoDiLaurea) { // controllo fallimento allocazione stringhe

        free(tabella->celle[slot].matricola); // cleanup parziale matricola
        free(tabella->celle[slot].nome); // cleanup parziale nome
        free(tabella->celle[slot].cognome); // cleanup parziale cognome
        free(tabella->celle[slot].corsoDiLaurea); // cleanup parziale corso
        return -1; // errore di allocazione
    }

    tabella->celle[slot].occupato = 1; // marca cella come attiva
    tabella->dimensione++; // incrementa contatore studenti presenti

    return 0; // studente inserito con successo
}

int eliminaStudente(const char *matricola) { // rimozione logica e fisica di uno studente
    int pos; // indice della posizione dello studente da rimuovere

    if (!tabella || !matricola) { // validazione input
        printf("Errore: parametri non validi.\n");
        return -1;
    }

    pos = trovaPosizione(matricola); // cerca indice associato alla matricola

    if (pos == -1) { // studente non trovato
        printf("Errore: studente non trovato.\n");
        return -1;
    }

    free(tabella->celle[pos].matricola); // libera stringa matricola
    free(tabella->celle[pos].nome); // libera stringa nome
    free(tabella->celle[pos].cognome); // libera stringa cognome
    free(tabella->celle[pos].corsoDiLaurea); // libera stringa corso di laurea

    tabella->celle[pos].matricola = NULL; // azzera puntatore per evitare dangling pointer
    tabella->celle[pos].nome = NULL; // azzera puntatore nome
    tabella->celle[pos].cognome = NULL; // azzera puntatore cognome
    tabella->celle[pos].corsoDiLaurea = NULL; // azzera puntatore corso
    tabella->celle[pos].occupato = -1; // marca come tombstone per preservare linear probing

    tabella->dimensione--; // decrementa contatore studenti attivi

    return 0; // eliminazione completata
}

studente cercaStudente(const char *matricola) { // ricerca pubblica di uno studente
    int pos; // variabile indice

    if (!tabella || !matricola)
        return NULL; // validazione input

    pos = trovaPosizione(matricola); // esegue scansione hash e lineare

    if (pos == -1)
        return NULL; // studente non trovato

    return &tabella->celle[pos]; // ritorna indirizzo cella trovata
}

// VISUALIZZAZIONI

void stampaStudente(studente s) { // visualizza dati di uno studente

    if (!s) { // protezione contro puntatori nulli
        printf("Errore: studente non valido.\n");
        return;
    }

    printf("Matricola: %s\n", s->matricola); // stampa matricola
    printf("Nome: %s\n", s->nome); // stampa nome
    printf("Cognome: %s\n", s->cognome); // stampa cognome
    printf("Corso: %s\n", s->corsoDiLaurea); // stampa corso di laurea
}

void stampaTabella() { // visualizza intero database studenti
    int i; // indice iterazione

    if (!tabella) { // controllo esistenza tabella
        printf("Errore: tabella non inizializzata.\n");
        return;
    }

    if (tabella->dimensione == 0) { // controllo tabella vuota
        printf("Nessuno studente registrato.\n");
        return;
    }

    printf("=== Studenti registrati (%d) ===\n", tabella->dimensione); // header con conteggio

    for (i = 0; i < tabella->capacita; i++) // itera su tutta la memoria allocata
        if (tabella->celle[i].occupato == 1) // considera solo slot con dati validi
            stampaStudente(&tabella->celle[i]); // stampa studente corrente
}

// GETTER

const char *getMatricola(studente s) { // accesso sicuro alla matricola
    if (s != NULL)
        return s->matricola; // ritorna puntatore stringa matricola
    else
        return NULL; // puntatore non valido
}

const char *getNome(studente s) { // accesso sicuro al nome
    if (s != NULL)
        return s->nome; // ritorna puntatore stringa nome
    else
        return NULL; // puntatore non valido
}

const char *getCognome(studente s) { // accesso sicuro al cognome
    if (s != NULL)
        return s->cognome; // ritorna puntatore stringa cognome
    else
        return NULL; // puntatore non valido
}

const char *getCorsoDiLaurea(studente s) { // accesso sicuro al corso
    if (s != NULL)
        return s->corsoDiLaurea; // ritorna puntatore stringa corso
    else
        return NULL; // puntatore non valido
}
