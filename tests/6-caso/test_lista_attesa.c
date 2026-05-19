/*
 * File: test_lista_attesa.c
 * ---
 * Caso di test 6: Test della lista di attesa.
 *
 * Verifica il corretto funzionamento delle operazioni
 * sulla lista d'attesa: inserimento, estrazione,
 * consultazione e rimozione per fascia oraria.
 *
 * Autore: Raffaele Severino
 * Data inizio: 18/05/2026
 * Data ultima modifica: 19/05/2026
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "listaAttesa.h"

/* 
 * Funzioni di test
 */

/*
 * Verifica che uno studente venga inserito
 * correttamente in coda per una fascia oraria.
 */
void test_aggiungi_inCoda_valido() {
    resetCoda();
    elementoCoda e;

    assert(aggiungi_inCoda("N46001234", 0) == 0 && "Inserimento valido deve ritornare 0");

    e = testa(0);

    assert(e != NULL && "La coda non deve essere vuota dopo inserimento");

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "La matricola deve corrispondere");

    assert(getFasciaCoda(e) == 0 && "La fascia oraria deve corrispondere");

    assert(dimensioneCoda(0) == 1 && "La dimensione deve essere 1");

    printf("test_aggiungi_inCoda_valido: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti l'inserimento
 * di uno studente già presente nella stessa fascia.
 */
void test_aggiungi_inCoda_duplicato() {
    resetCoda();
    aggiungi_inCoda("N46001234", 0);

    assert(aggiungi_inCoda("N46001234", 0) == -1  && "Studente duplicato nella stessa fascia deve ritornare -1");

    assert(dimensioneCoda(0) == 1 && "La dimensione non deve cambiare dopo duplicato");

    printf("test_aggiungi_inCoda_duplicato: PASSATO\n");
}

/*
 * Verifica che lo stesso studente possa essere
 * in attesa per fasce orarie diverse.
 */
void test_aggiungi_inCoda_fasce_diverse() {
    resetCoda();
    assert(aggiungi_inCoda("N46001234", 0) == 0 && "Inserimento fascia 0 deve ritornare 0");

    assert(aggiungi_inCoda("N46001234", 1) == 0 && "Inserimento fascia 1 deve ritornare 0");

    assert(dimensioneCoda(0) == 1 && "Dimensione fascia 0 deve essere 1");

    assert(dimensioneCoda(1) == 1 && "Dimensione fascia 1 deve essere 1");

    printf("test_aggiungi_inCoda_fasce_diverse: PASSATO\n");
}

/*
 * Verifica che il sistema gestisca correttamente
 * parametri non validi in aggiungi_inCoda.
 */
void test_aggiungi_inCoda_parametri_invalidi() {
    resetCoda();
    assert(aggiungi_inCoda(NULL, 0) == -1 && "Matricola NULL deve ritornare -1");

    assert(aggiungi_inCoda("N46001234", -1) == -1 && "Fascia negativa deve ritornare -1");

    assert(aggiungi_inCoda("N46001234", NUM_FASCE) == -1 && "Fascia >= NUM_FASCE deve ritornare -1");

    printf("test_aggiungi_inCoda_parametri_invalidi: PASSATO\n");
}

/*
 * Verifica che l'ordine FIFO sia rispettato.
 * Il primo inserito deve essere il primo estratto.
 */
void test_ordine_fifo() {
    resetCoda();
    elementoCoda e;

    aggiungi_inCoda("N46001234", 0);
    aggiungi_inCoda("N46001235", 0);
    aggiungi_inCoda("N46001236", 0);

    e = testa(0);

    assert(e != NULL && "La coda non deve essere vuota");

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "Il primo inserito deve essere in testa");

    printf("test_ordine_fifo: PASSATO\n");
}

/*
 * Verifica che estraiDaCoda rimuova correttamente
 * il primo elemento rispettando l'ordine FIFO.
 */
void test_estrai_daCoda() {
    resetCoda();
    elementoCoda e;

    aggiungi_inCoda("N46001234", 0);
    aggiungi_inCoda("N46001235", 0);

    e = estraiDaCoda(0);

    assert(e != NULL && "estraiDaCoda deve ritornare un elemento valido");

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "Il primo inserito deve essere estratto per primo");

    assert(dimensioneCoda(0) == 1 && "La dimensione deve decrementare dopo estrazione");

    /*
     * Il nodo estratto non appartiene piu alla coda,
     * il chiamante e' responsabile di liberarlo.
     */
    free((void *)e);

    e = testa(0);

    assert(strcmp(getMatricolaCoda(e), "N46001235") == 0 && "Dopo estrazione il secondo deve diventare testa");

    printf("test_estrai_daCoda: PASSATO\n");
}

/*
 * Verifica che estraiDaCoda su coda vuota
 * ritorni NULL senza crash.
 */
void test_estrai_daCoda_vuota() {
    resetCoda();
    assert(estraiDaCoda(0) == NULL && "Estrazione da coda vuota deve ritornare NULL");

    printf("test_estrai_daCoda_vuota: PASSATO\n");
}

/*
 * Verifica che testa ritorni il primo elemento
 * senza modificare la coda.
 */
void test_testa_non_modifica_coda() {
    resetCoda();
    elementoCoda e;

    aggiungi_inCoda("N46001234", 0);
    aggiungi_inCoda("N46001235", 0);

    e = testa(0);

    assert(e != NULL && "testa deve ritornare un elemento valido");

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "testa deve ritornare il primo elemento");

    assert(dimensioneCoda(0) == 2 && "testa non deve modificare la dimensione della coda");

    e = testa(0);

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "Dopo seconda chiamata testa deve ritornare ancora il primo");

    printf("test_testa_non_modifica_coda: PASSATO\n");
}

/*
 * Verifica che rimuoviDaCoda elimini correttamente
 * uno studente in qualsiasi posizione della coda.
 */
void test_rimuovi_daCoda() {
    resetCoda();
    elementoCoda e;

    aggiungi_inCoda("N46001234", 0);
    aggiungi_inCoda("N46001235", 0);
    aggiungi_inCoda("N46001236", 0);

    /*
     * Rimozione in mezzo: verifica che i puntatori
     * prev e next vengano aggiornati correttamente.
     */
    assert(rimuoviDaCoda("N46001235", 0) == 0 && "Rimozione elemento centrale deve ritornare 0");

    assert(dimensioneCoda(0) == 2 && "La dimensione deve decrementare dopo rimozione");

    e = testa(0);

    assert(strcmp(getMatricolaCoda(e), "N46001234") == 0 && "La testa non deve cambiare dopo rimozione centrale");

    printf("test_rimuovi_daCoda: PASSATO\n");
}

/*
 * Verifica che rimuoviDaCoda gestisca correttamente
 * parametri non validi.
 */
void test_rimuovi_daCoda_parametri_invalidi() {
    resetCoda();
    assert(rimuoviDaCoda(NULL, 0) == -1 && "Matricola NULL deve ritornare -1");

    assert(rimuoviDaCoda("N46001234", -1) == -1 && "Fascia negativa deve ritornare -1");

    assert(rimuoviDaCoda("N46001234", NUM_FASCE) == -1 && "Fascia >= NUM_FASCE deve ritornare -1");

    assert(rimuoviDaCoda("N46999999", 0) == -1 && "Matricola non presente deve ritornare -1");

    printf("test_rimuovi_daCoda_parametri_invalidi: PASSATO\n");
}

/*
 * Verifica che codaVuota ritorni il valore
 * corretto in tutti i casi.
 */
void test_coda_vuota() {
    resetCoda();
    assert(codaVuota(0) == 1 && "Coda non inizializzata deve essere vuota");

    aggiungi_inCoda("N46001234", 0);

    assert(codaVuota(0) == 0 && "Coda con elementi non deve essere vuota");

    estraiDaCoda(0);

    assert(codaVuota(0) == 1 && "Coda svuotata deve risultare vuota");

    printf("test_coda_vuota: PASSATO\n");
}

/*
 * Verifica che le code delle diverse fasce
 * siano completamente indipendenti tra loro.
 */
void test_fasce_indipendenti() {
    resetCoda();
    aggiungi_inCoda("N46001234", 0);
    aggiungi_inCoda("N46001235", 1);

    assert(dimensioneCoda(0) == 1 && "Fascia 0 deve avere dimensione 1");

    assert(dimensioneCoda(1) == 1 && "Fascia 1 deve avere dimensione 1");

    estraiDaCoda(0);

    assert(dimensioneCoda(0) == 0 && "Fascia 0 deve essere vuota dopo estrazione");

    assert(dimensioneCoda(1) == 1 && "Fascia 1 non deve essere influenzata");

    printf("test_fasce_indipendenti: PASSATO\n");
}

/*
 * Main
 */

int main() {
    printf("=== Caso di test 6: Lista di attesa ===\n\n");

    test_aggiungi_inCoda_valido();
    test_aggiungi_inCoda_duplicato();
    test_aggiungi_inCoda_fasce_diverse();
    test_aggiungi_inCoda_parametri_invalidi();
    test_ordine_fifo();
    test_estrai_daCoda();
    test_estrai_daCoda_vuota();
    test_testa_non_modifica_coda();
    test_rimuovi_daCoda();
    test_rimuovi_daCoda_parametri_invalidi();
    test_coda_vuota();
    test_fasce_indipendenti();

    printf("\nTutti i test sono stati superati.\n");

    return 0;
}