/*
 * File: test_inserimento_prenotazioni.c
 * ---
 * Caso di test 2: Test dell'inserimento delle prenotazioni.
 *
 * Verifica il corretto funzionamento di creaPrenotazione
 * nei casi validi e nei casi di errore.
 *
 * Autore: Raffaele Severino
 * Data: 13/05/2026
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "prenotazioni.h"

/*
 * Funzioni di test
 */

/*
 * Verifica che una prenotazione valida venga
 * inserita correttamente nel sistema.
 */
void test_inserimento_valido() {
    prenotazione p;

    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", 0, 1) == 0 && "Inserimento valido deve ritornare 0");

    p = cercaPrenotazione("N46001234", "2026-05-29", 0);

    assert(p != NULL && "La prenotazione inserita deve essere trovabile");

    assert(strcmp(getMatricolaPrenotazione(p), "N46001234") == 0 && "La matricola deve corrispondere");

    assert(strcmp(getDataPrenotazione(p), "2026-05-29") == 0 && "La data deve corrispondere");

    assert(getFasciaOraria(p) == 0 && "La fascia oraria deve corrispondere");

    assert(getPostoAssegnato(p) == 1 && "Il posto assegnato deve corrispondere");

    distruggiListaPrenotazioni();

    printf("test_inserimento_valido: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti una prenotazione
 * duplicata per la stessa combinazione
 * matricola + data + fasciaOraria.
 */
void test_prenotazione_duplicata() {
    creaListaPrenotazioni();

    creaPrenotazione("N46001234", "2026-05-29", 0, 1);

    assert(creaPrenotazione("N46001234", "2026-05-29", 0, 2) == -1 && "Prenotazione duplicata deve ritornare -1");

    distruggiListaPrenotazioni();

    printf("test_prenotazione_duplicata: PASSATO\n");
}

/*
 * Verifica che il sistema gestisca correttamente
 * parametri NULL senza crash.
 */
void test_parametro_null() {
    creaListaPrenotazioni();

    assert(creaPrenotazione(NULL, "2026-05-29", 0, 1) == -1 && "Matricola NULL deve ritornare -1");

    assert(creaPrenotazione("N46001234", NULL, 0, 1) == -1 && "Data NULL deve ritornare -1");

    distruggiListaPrenotazioni();

    printf("test_parametro_null: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti una fascia
 * oraria con valore negativo.
 */
void test_fascia_oraria_negativa() {
    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", -1, 1) == -1 && "Fascia oraria negativa deve ritornare -1");

    distruggiListaPrenotazioni();

    printf("test_fascia_oraria_negativa: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti una fascia
 * oraria con valore >= NUM_FASCE.
 */
void test_fascia_oraria_fuori_range() {
    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", NUM_FASCE, 1) == -1 && "Fascia oraria >= NUM_FASCE deve ritornare -1");

    distruggiListaPrenotazioni();

    printf("test_fascia_oraria_fuori_range: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti un posto
 * con valore negativo.
 */
void test_posto_negativo() {
    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", 0, -1) == -1 && "Posto negativo deve ritornare -1");

    distruggiListaPrenotazioni();

    printf("test_posto_negativo: PASSATO\n");
}

/*
 * Verifica che lo stesso studente possa prenotare
 * in fasce orarie diverse nello stesso giorno.
 */
void test_stesso_studente_fascia_diversa() {
    prenotazione p1, p2;

    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", 0, 1) == 0 && "Prima prenotazione deve ritornare 0");

    assert(creaPrenotazione("N46001234", "2026-05-29", 1, 2) == 0 && "Seconda prenotazione fascia diversa deve ritornare 0");

    p1 = cercaPrenotazione("N46001234", "2026-05-29", 0);
    p2 = cercaPrenotazione("N46001234", "2026-05-29", 1);

    assert(p1 != NULL && "Prima prenotazione deve essere trovabile");
    assert(p2 != NULL && "Seconda prenotazione deve essere trovabile");

    assert(getFasciaOraria(p1) == 0 && "Prima prenotazione deve avere fascia 0");

    assert(getFasciaOraria(p2) == 1 && "Seconda prenotazione deve avere fascia 1");

    distruggiListaPrenotazioni();

    printf("test_stesso_studente_fascia_diversa: PASSATO\n");
}

/*
 * Verifica che inserimenti multipli di studenti
 * diversi avvengano correttamente.
 */
void test_inserimenti_multipli() {
    prenotazione p1, p2, p3;

    creaListaPrenotazioni();

    assert(creaPrenotazione("N46001234", "2026-05-29", 0, 1) == 0  && "Prima prenotazione deve ritornare 0");

    assert(creaPrenotazione("N46001235", "2026-05-29", 0, 2) == 0 && "Seconda prenotazione deve ritornare 0");

    assert(creaPrenotazione("N46001236", "2026-05-29", 0, 3) == 0 && "Terza prenotazione deve ritornare 0");

    p1 = cercaPrenotazione("N46001234", "2026-05-29", 0);
    p2 = cercaPrenotazione("N46001235", "2026-05-29", 0);
    p3 = cercaPrenotazione("N46001236", "2026-05-29", 0);

    assert(p1 != NULL && "Prima prenotazione deve essere trovabile");
    assert(p2 != NULL && "Seconda prenotazione deve essere trovabile");
    assert(p3 != NULL && "Terza prenotazione deve essere trovabile");

    assert(getPostoAssegnato(p1) == 1 && "Posto prima prenotazione deve essere 1");

    assert(getPostoAssegnato(p2) == 2 && "Posto seconda prenotazione deve essere 2");

    assert(getPostoAssegnato(p3) == 3 && "Posto terza prenotazione deve essere 3");

    distruggiListaPrenotazioni();

    printf("test_inserimenti_multipli: PASSATO\n");
}

/* 
 * Main
 */

int main() {
    printf("=== Caso di test 2: Inserimento prenotazioni ===\n\n");

    test_inserimento_valido();
    test_prenotazione_duplicata();
    test_parametro_null();
    test_fascia_oraria_negativa();
    test_fascia_oraria_fuori_range();
    test_posto_negativo();
    test_stesso_studente_fascia_diversa();
    test_inserimenti_multipli();

    printf("\nTutti i test sono stati superati.\n");

    return 0;
}