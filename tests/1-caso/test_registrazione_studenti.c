/*
 * File: test_registrazione_studenti.c
 * ---
 * Caso di test 1: Verifica della registrazione degli studenti.
 *
 * Verifica il corretto funzionamento di creaStudente
 * nei casi validi e nei casi di errore.
 *
 * Autore: Raffaele Severino
 * Data inizio: 13/05/2026
 * Data ultima modifica: 19/05/2026
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "studente.h"

/* 
 * Funzioni di test
 */

/*
 * Verifica che uno studente valido venga
 * registrato correttamente nel sistema.
 */
void test_inserimento_valido() {
    studente s;

    creaTabella(10);

    assert(creaStudente("Mario", "Rossi", "N46001234", "Informatica") == 0 && "Inserimento valido deve ritornare 0");

    s = cercaStudente("N46001234");

    assert(s != NULL && "Lo studente inserito deve essere trovabile");

    assert(strcmp(getMatricola(s), "N46001234") == 0 && "La matricola deve corrispondere");

    assert(strcmp(getNome(s), "Mario") == 0 && "Il nome deve corrispondere");

    assert(strcmp(getCognome(s), "Rossi") == 0 && "Il cognome deve corrispondere");

    assert(strcmp(getCorsoDiLaurea(s), "Informatica") == 0  && "Il corso di laurea deve corrispondere");

    distruggiTabella();

    printf("test_inserimento_valido: PASSATO\n");
}

/*
 * Verifica che il sistema rifiuti l'inserimento
 * di una matricola già presente.
 */
void test_matricola_duplicata() {
    creaTabella(10);

    creaStudente("Mario", "Rossi", "N46001234", "Informatica");

    assert(creaStudente("Luigi", "Verdi", "N46001234", "Matematica") == -1
           && "Matricola duplicata deve ritornare -1");

    distruggiTabella();

    printf("test_matricola_duplicata: PASSATO\n");
}

/*
 * Verifica che il sistema gestisca correttamente
 * parametri NULL senza crash.
 */
void test_parametro_null() {
    creaTabella(10);

    assert(creaStudente(NULL, "Rossi", "N46001234", "Informatica") == -1
           && "Nome NULL deve ritornare -1");

    assert(creaStudente("Mario", NULL, "N46001234", "Informatica") == -1
           && "Cognome NULL deve ritornare -1");

    assert(creaStudente("Mario", "Rossi", NULL, "Informatica") == -1
           && "Matricola NULL deve ritornare -1");

    assert(creaStudente("Mario", "Rossi", "N46001234", NULL) == -1
           && "Corso NULL deve ritornare -1");

    distruggiTabella();

    printf("test_parametro_null: PASSATO\n");
}

/*
 * Verifica che il sistema gestisca correttamente
 * inserimenti multipli con matricole diverse.
 */
void test_inserimenti_multipli() {
    studente s1, s2, s3;

    creaTabella(10);

    assert(creaStudente("Mario", "Rossi", "N46001234", "Informatica") == 0 && "Primo inserimento deve ritornare 0");

    assert(creaStudente("Luigi","Verdi", "N46001235", "Matematica") == 0 && "Secondo inserimento deve ritornare 0");

    assert(creaStudente("Giovanni", "Bianchi", "N46001236", "Fisica") == 0 && "Terzo inserimento deve ritornare 0");

    s1 = cercaStudente("N46001234");
    s2 = cercaStudente("N46001235");
    s3 = cercaStudente("N46001236");

    assert(s1 != NULL && "Primo studente deve essere trovabile");
    assert(s2 != NULL && "Secondo studente deve essere trovabile");
    assert(s3 != NULL && "Terzo studente deve essere trovabile");

    distruggiTabella();

    printf("test_inserimenti_multipli: PASSATO\n");
}

/*
 * Verifica che il rehash avvenga correttamente
 * quando la tabella raggiunge la capacita massima.
 */
void test_rehash() {
    int i;
    char matricola[12];
    studente s;

    creaTabella(5);

    /*
     * Si inseriscono piu studenti della capacita iniziale
     * per forzare il rehash automatico della tabella.
     */
    for (i = 0; i < 8; i++) {
        sprintf(matricola, "N4600%04d", i);
        assert(creaStudente("Nome", "Cognome", matricola, "Informatica") == 0
               && "Inserimento durante rehash deve ritornare 0");
    }

    for (i = 0; i < 8; i++) {
        sprintf(matricola, "N4600%04d", i);
        s = cercaStudente(matricola);
        assert(s != NULL && "Studente deve essere trovabile dopo rehash");
    }

    distruggiTabella();

    printf("test_rehash: PASSATO\n");
}

/* 
 * Main
 */

int main() {
    printf("=== Caso di test 1: Registrazione studenti ===\n\n");

    test_inserimento_valido();
    test_matricola_duplicata();
    test_parametro_null();
    test_inserimenti_multipli();
    test_rehash();

    printf("\nTutti i test sono stati superati.\n");

    return 0;
}