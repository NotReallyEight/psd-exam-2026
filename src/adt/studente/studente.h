/*
* File: studente.h
* ---
* Gestione di un ADT per l'anagrafica studenti tramite
* tabella dedicata con accesso controllato ai dati.
*
* Il modulo consente la creazione, ricerca, stampa ed
* eliminazione degli studenti registrati nel sistema.
*
* Autore: Raffaele Severino
* Data inizio: 02/04/2026
* Data ultima modifica: 13/05/2026
*/




/*
* Puntatore alla struttura interna studente.
*
* L'implementazione concreta è nascosta nel file .c
* per garantire l'information hiding.
*/
typedef struct studenteInterno *studente;







/*
* Funzione: creaTabella
* ---
* Alloca e inizializza la tabella che conterrà
* gli studenti del sistema.
*
* Parametri:
* capacita: numero massimo iniziale di posizioni.
*
* Pre-condizione:
* capacita deve essere maggiore di zero.
*
* Post-condizione:
* Se l'operazione riesce, la tabella viene creata
* e resa disponibile al modulo.
*
* Ritorna:
* 0 se la creazione ha successo, valore negativo
* in caso di errore.
*/
int creaTabella(int capacita);






/*
* Funzione: distruggiTabella
* ---
* Dealloca la tabella studenti e tutte le risorse
* dinamiche ad essa associate.
*
* Parametri:
* Nessuno.
*
* Pre-condizione:
* Nessuna.
*
* Post-condizione:
* La tabella non è più disponibile nel sistema.
*/
void distruggiTabella();





/*
* Funzione: stampaTabella
* ---
* Visualizza l'elenco degli studenti attualmente
* registrati nella tabella.
*
* Parametri:
* Nessuno.
*
* Pre-condizione:
* La tabella deve essere stata creata.
*
* Post-condizione:
* I dati dello studente vengono stampati a schermo.
* Se s è NULL viene stampato un messaggio di errore.
*/
void stampaTabella();






/*
* Funzione: creaStudente
* ---
* Crea un nuovo studente e lo inserisce nella tabella.
*
* Parametri:
* nome: nome dello studente.
* cognome: cognome dello studente.
* matricola: codice identificativo univoco.
* corsoDiLaurea: corso di appartenenza.
*
* Pre-condizione:
* Tutte le stringhe devono essere valide e non nulle.
*
* Post-condizione:
* Se non esistono duplicati, lo studente viene
* registrato nel sistema.
*
* Ritorna:
* 0 se l'inserimento ha successo, valore negativo
* in caso di errore.
*/
int creaStudente(const char *nome, const char *cognome, const char *matricola,const char *corsoDiLaurea);





/*
* Funzione: eliminaStudente
* ---
* Rimuove dalla tabella lo studente associato
* alla matricola indicata.
*
* Parametri:
* matricola: identificativo dello studente.
*
* Pre-condizione:
* La matricola deve essere valida.
*
* Post-condizione:
* Se presente, lo studente viene eliminato
* dal sistema.
*
* Ritorna:
* 0 se la rimozione ha successo, valore negativo
* se lo studente non esiste o in caso di errore.
*/
int eliminaStudente(const char *matricola);






/*
* Funzione: cercaStudente
* ---
* Cerca uno studente tramite matricola.
*
* Parametri:
* matricola: identificativo dello studente.
*
* Pre-condizione:
* La matricola deve essere valida.
*
* Post-condizione:
* Nessuna modifica ai dati interni.
*
* Ritorna:
* Puntatore allo studente trovato, NULL se assente.
*/
studente cercaStudente(const char *matricola);






/*
* Funzione: stampaStudente
* ---
* Mostra a video i dati dello studente ricevuto.
*
* Parametri:
* s: studente da visualizzare.
*
* Pre-condizione:
* s deve riferirsi a uno studente valido.
*
* Post-condizione:
* I dati dello studente vengono stampati a schermo.
* Se s è NULL viene stampato un messaggio di errore.
*/
void stampaStudente(studente s);






/*
* Funzioni getter.
* ---
* Consentono la lettura controllata dei campi interni
* senza accesso diretto alla struttura dati.
*/



/*
* Funzione: getMatricola
* ---
* Restituisce la matricola dello studente.
*
* Parametri:
* s: studente da consultare.
*
* Ritorna:
* Puntatore alla stringa matricola, NULL se non valido.
*/
const char *getMatricola(studente s);





/*
* Funzione: getNome
* ---
* Restituisce il nome dello studente.
*
* Parametri:
* s: studente da consultare.
*
* Ritorna:
* Puntatore alla stringa nome, NULL se non valido.
*/
const char *getNome(studente s);






/*
* Funzione: getCognome
* ---
* Restituisce il cognome dello studente.
*
* Parametri:
* s: studente da consultare.
*
* Ritorna:
* Puntatore alla stringa cognome, NULL se non valido.
*/
const char *getCognome(studente s);







/*
* Funzione: getCorsoDiLaurea
* ---
* Restituisce il corso di laurea dello studente.
*
* Parametri:
* s: studente da consultare.
*
* Ritorna:
* Puntatore alla stringa del corso di laurea,
* NULL se non valido.
*/
const char *getCorsoDiLaurea(studente s);