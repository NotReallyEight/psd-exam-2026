#include <stdio.h>
#include <stdlib.h>

#include "studente.h"

// definizione struttura dati privata per lo studente
typedef struct studenteInterno{ 
    char* nome; // stringa per allocazione dinamica del nome
    char* cognome; // stringa per allocazione dinamica del cognome
    char* matricola; // chiave univoca per identificazione e hashing
    char*corsoDiLaurea; // stringa per il corso di studi dello studente
    int occupato; // stato cella: 1 attivo, 0 mai usata, -1 rimosso (tombstone)
}studenteInterno;


// struttura contenitore per la tabella hash
typedef struct tabellaStudentiInterna { 
    studenteInterno *celle; // puntatore all'array di record studenteinterno
    int capacita; // numero totale di slot fisici allocati nell'array
    int dimensione; // conteggio effettivo degli studenti inseriti e attivi
} tabellaStudentiInterna;

static tabellaStudentiInterna *tabella = NULL; // riferimento statico alla tabella

static unsigned int hash(const char *matricola, int capacita) { // funzione hash djb2
    unsigned int h = 5381; // valore standard iniziale per la distribuzione
    while (*matricola) // ciclo finche non si raggiunge il terminatore della stringa
        h = ((h << 5) + h) + (unsigned char)(*matricola++); // algoritmo: h * 33 + codice ascii carattere
    return h % (unsigned int)capacita; // mapping dell'hash nel range degli indici della tabella
}

static char *duplicaStringa(const char *src) { // funzione per duplicare stringhe in memoria
    char *dst; // puntatore per la nuova stringa destinazione
    if (!src) 
        return NULL;
    dst = (char *)malloc(strlen(src) + 1); // allocazione memoria: lunghezza stringa piu terminatore \0
    if (!dst) 
        return NULL;
    strcpy(dst, src); // copia fisica dei caratteri dalla sorgente alla destinazione
    return dst; // ritorna l'indirizzo della nuova stringa allocata
}

static int trovaPosizione(const char *matricola) { // cerca l'indice di una matricola esistente
    unsigned int idx = hash(matricola, tabella->capacita); // calcolo indice di partenza tramite hash
    int i; // contatore per scansione lineare delle collisioni
    for (i = 0; i < tabella->capacita; i++) { // ciclo di ricerca su tutta la capacita massima
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita; // calcolo posizione con scansione 
        if (tabella->celle[pos].occupato == 0)  
            return -1; // se trova cella vuota lo studente non esiste
        if (tabella->celle[pos].occupato == 1 && // se la cella e occupata da un dato valido
            strcmp(tabella->celle[pos].matricola, matricola) == 0) // e la matricola coincide con la chiave
            return (int)pos; // ritorna l'indice dove e stato trovato lo studente
    }
    return -1; // ritorna -1 se lo studente non e presente dopo scansione completa
}

static int trovaSlotInserimento(const char *matricola) { // cerca posto per nuovo inserimento
    unsigned int idx = hash(matricola, tabella->capacita); // indice di partenza calcolato sulla chiave
    int primoTombstone = -1, i; // variabili per tracciamento primo slot eliminato e contatore
    for (i = 0; i < tabella->capacita; i++) { // scansione lineare per risoluzione collisioni
        unsigned int pos = (idx + i) % (unsigned int)tabella->capacita; // calcolo indice successivo 
        if (tabella->celle[pos].occupato == 0) // se la cella non e mai stata usata
            if (primoTombstone != -1) { // usa il tombstone se trovato prima, altrimenti pos
                return primoTombstone;
            } else {
                return (int)pos;
        }

        if (tabella->celle[pos].occupato == -1 && primoTombstone == -1) // se e un record eliminato (tombstone)
            primoTombstone = (int)pos; // salva la posizione per riutilizzo efficiente memoria

        if (tabella->celle[pos].occupato == 1 && strcmp(tabella->celle[pos].matricola, matricola) == 0) // chiave esistente
            return -1; // ritorna errore per impedire inserimento di duplicati
    }
    return primoTombstone; // ritorna l'eventuale tombstone se la tabella e satura di cancellati
}

static int rehash() { // funzione per ingrandire la tabella quando piena
    int nuovaCapacita = tabella->capacita + 20; // definizione nuova dimensione (aumenta di 20 ogni volta che è piena)
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
                if (nuoveCelle[pos].occupato == 0) { // se lo slot nella nuova tabella e libero
                    nuoveCelle[pos].matricola= tabella->celle[i].matricola; // sposta puntatore matricola
                    nuoveCelle[pos].nome = tabella->celle[i].nome; // sposta puntatore nome
                    nuoveCelle[pos].cognome = tabella->celle[i].cognome; // sposta puntatore cognome
                    nuoveCelle[pos].corsoDiLaurea = tabella->celle[i].corsoDiLaurea; // sposta puntatore corso
                    nuoveCelle[pos].occupato= 1; // marca la cella nella nuova tabella come attiva
                    break; // passa al prossimo studente da trasferire
                }
            }
        }
    }

    free(tabella->celle); // libera l'array di celle vecchio (non le stringhe che sono state spostate)
    tabella->celle = nuoveCelle; // collega il nuovo array alla struttura principale
    tabella->capacita = nuovaCapacita; // aggiorna il valore della capacita massima

    return 0; // successo operazione di rehashing
}

int creaTabella(int capacita) { // inizializzazione della struttura dati
    if (capacita <= 0) 
        return -1; // controllo validita parametro di input

    tabella = (tabellaStudentiInterna *)malloc(sizeof(tabellaStudentiInterna)); // alloca memoria per l'header della tabella
    if (!tabella) 
        return -1; // controllo errore malloc header

    tabella->celle = (studenteInterno *)calloc(capacita, sizeof(studenteInterno)); // alloca array celle e azzera byte
    if (!tabella->celle) { 
        free(tabella); 
        tabella = NULL; 
        return -1; 
    } 

    tabella->capacita  = capacita; // imposta la capacita iniziale fornita
    tabella->dimensione = 0; // inizializza il numero di elementi correnti a zero
    return 0; // tabella creata con successo
}

void distruggiTabella() { // deallocazione completa della memoria
    int i; // indice per iterare tra le celle
    if (!tabella) 
        return; // se la tabella non esiste esce subito

    for (i = 0; i < tabella->capacita; i++) { // ciclo per liberare ogni stringa allocata
        if (tabella->celle[i].occupato == 1) { // verifica se la cella contiene dati da liberare
            free(tabella->celle[i].matricola); // dealloca stringa della matricola
            free(tabella->celle[i].nome); // dealloca stringa del nome
            free(tabella->celle[i].cognome); // dealloca stringa del cognome
            free(tabella->celle[i].corsoDiLaurea); // dealloca stringa del corso di laurea
        }
    }
    free(tabella->celle); // libera l'array di strutture studenteinterno
    free(tabella); // libera la struttura header della tabella
    tabella = NULL; // azzera il puntatore globale per sicurezza
}

int creaStudente(const char *nome, const char *cognome, const char *matricola, const char *corsoDiLaurea) { // inserimento nuovo studente
    int slot; // variabile per contenere l'indice di destinazione

    if (!tabella || !nome || !cognome || !matricola || !corsoDiLaurea) { //validazione paramerti
    printf("Errore: parametri non validi.\n");
    return -1; 
}

    if (tabella->dimensione == tabella->capacita) // controllo se la tabella e fisicamente piena
        if (rehash() == -1) 
            return -1; // tenta di espandere e ritorna errore se fallisce


    slot = trovaSlotInserimento(matricola); // cerca la posizione ottimale (libera o tombstone)

    if (slot == -1) 
        return -1; // se slot e -1 significa che la matricola e gia presente

    tabella->celle[slot].matricola = duplicaStringa(matricola); // alloca e copia matricola
    tabella->celle[slot].nome = duplicaStringa(nome); // alloca e copia nome
    tabella->celle[slot].cognome = duplicaStringa(cognome); // alloca e copia cognome
    tabella->celle[slot].corsoDiLaurea = duplicaStringa(corsoDiLaurea); // alloca e copia corso

    if (!tabella->celle[slot].matricola || // controllo fallimento allocazione di una delle stringhe
        !tabella->celle[slot].nome || // controllo nome
        !tabella->celle[slot].cognome || // controllo cognome
        !tabella->celle[slot].corsoDiLaurea) { // controllo corso     
        free(tabella->celle[slot].matricola); // cleanup parziale matricola
        free(tabella->celle[slot].nome); // cleanup parziale nome
        free(tabella->celle[slot].cognome); // cleanup parziale cognome
        free(tabella->celle[slot].corsoDiLaurea); // cleanup parziale corso
        return -1; // ritorna errore di allocazione
    }
    tabella->celle[slot].occupato = 1; // imposta lo stato della cella come occupata/valida
    tabella->dimensione++; // incrementa il contatore globale degli studenti presenti

    return 0; // studente inserito con successo
}

int eliminaStudente(const char *matricola) { // rimozione logica e fisica di uno studente

    int pos; // indice della posizione dello studente da rimuovere

    if (!tabella || !matricola) 
        return -1; // validazione input

    pos = trovaPosizione(matricola); // cerca l'indice associato alla matricola
    if (pos == -1) { //se non trovato ritorna errore
    printf("Errore: studente non trovato.\n");
    return -1; 
}


    free(tabella->celle[pos].matricola); // libera memoria stringa matricola
    free(tabella->celle[pos].nome); // libera memoria stringa nome
    free(tabella->celle[pos].cognome); // libera memoria stringa cognome
    free(tabella->celle[pos].corsoDiLaurea); // libera memoria stringa corso di laurea


    tabella->celle[pos].matricola= NULL; // azzera puntatore per evitare dangling pointers
    tabella->celle[pos].nome = NULL; // azzera puntatore nome
    tabella->celle[pos].cognome = NULL; // azzera puntatore cognome
    tabella->celle[pos].corsoDiLaurea = NULL; // azzera puntatore corso
    tabella->celle[pos].occupato = -1; // marca come tombstone per non rompere ricerche hash successive
    tabella->dimensione--; // decrementa il numero di studenti attivi


    return 0; // eliminazione completata
};

studente cercaStudente(const char *matricola) { // ricerca pubblica di uno studente
    int pos; // variabile indice

    if (!tabella || !matricola) 
        return NULL;

    pos = trovaPosizione(matricola); // esegue la scansione hash e lineare
    if (pos == -1) {
        return NULL;               // Caso: Elemento non trovato
    } else {
        return &tabella->celle[pos]; // Caso: Elemento trovato ritorna indirizzo cella castato a studente o null
    } 
}



void stampaStudente(studente s) { // visualizza dati studente

    if (!s) 
        return; // protezione contro puntatori nulli

    printf("Matricola: %s\n", s->matricola); // stampa a video la matricola
    printf("Nome: %s\n", s->nome); // stampa a video il nome
    printf("Cognome: %s\n", s->cognome); // stampa a video il cognome
    printf("Corso: %s\n",s->corsoDiLaurea); // stampa a video il corso di laurea

};

void stampaTabella() { // visualizza intero database studenti
    int i; // indice iterazione

    if (!tabella)
        return; // controllo esistenza tabella

    printf("=== Studenti registrati (%d) ===\n", tabella->dimensione); // header con conteggio elementi
    
    for (i = 0; i < tabella->capacita; i++) // itera su tutta la memoria allocata
        if (tabella->celle[i].occupato == 1) // considera solo gli slot con dati validi (non vuoti o tombstone)
            stampaStudente(&tabella->celle[i]); // chiama la funzione di stampa per lo studente corrente
}


//blocco di funzioni che passano al utente un campo in modo controllato
//cosidetti getter

const char *getMatricola(studente s) { // accesso sicuro alla matricola
    if (s != NULL) { // verifica se il puntatore studente e valido
        return s->matricola; // ritorna puntatore alla stringa matricola (solo lettura)
    } else {
        return NULL;
    }
}

const char *getNome(studente s) { // accesso sicuro al nome
    if (s != NULL) { // verifica validita oggetto
        return s->nome; // ritorna indirizzo stringa nome
    } else {
        return NULL;
    }
}

const char *getCognome(studente s) { // accesso sicuro al cognome
    if (s != NULL) { // verifica validita oggetto
        return s->cognome; // ritorna indirizzo stringa cognome
    } else {
        return NULL;
    }
}

const char *getCorsoDiLaurea(studente s) { // accesso sicuro al corso
    if (s != NULL) { // verifica validita oggetto
        return s->corsoDiLaurea; // ritorna indirizzo stringa corso
    } else {
        return NULL;
    }
}