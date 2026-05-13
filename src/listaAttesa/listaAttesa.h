/*
* File: listaAttesa.h
* ---
* Gestione di un ADT per una lista d'attesa 
* organizzata in fasce orarie multiple.
*
* Autore: Raffaele Severino
* Data inizio: 02/04/2026
* Data ultima modifica: 13/05/2026
*/





#define NUM_FASCE 5

/* * Puntatore alla struttura interna della coda. 
* L'implementazione è nascosta nel file .c .
*/
typedef struct elementoCodaInterno *elementoCoda;





/*
* Funzione: enqueue
* ---
* Inserisce un nuovo studente nella coda relativa a una specifica fascia.
*
* Parametri:
* matricola: stringa identificativa dello studente.
* fasciaOraria: indice della fascia (da 0 a NUM_FASCE-1).
*
* Pre-condizione:
* La matricola deve essere una stringa valida e non nulla. 
* La fasciaOraria deve rientrare nel range definito.
*
* Post-condizione:
* Ritorna 1 se l'inserimento è avvenuto con successo, 0 altrimenti.
*/
int enqueue(const char *matricola, int fasciaOraria);




/*
* Funzione: dequeue
* ---
* Estrae dalla coda il primo studente in attesa per la fascia indicata.
*
* Parametri:
* fasciaOraria: indice della fascia da cui prelevare l'elemento.
*
* Pre-condizione:
* La coda della fascia selezionata non deve essere vuota.
*
* Post-condizione:
* Ritorna il puntatore all'elemento rimosso. La gestione della memoria 
* dell'elemento restituito passa al chiamante.
*/
elementoCoda dequeue(int fasciaOraria);





/*
* Funzione: top
* ---
* Permette di consultare il primo studente della coda senza rimuoverlo.
*
* Parametri:
* fasciaOraria: indice della fascia da consultare.
*
* Pre-condizione:
* Nessuna.
*
* Post-condizione:
* Ritorna il puntatore al primo elemento se presente, NULL altrimenti.
*/
elementoCoda top(int fasciaOraria);






/*
* Funzione: rimuoviDaCoda
* ---
* Cerca e rimuove una matricola specifica all'interno di una determinata fascia.
*
* Parametri:
* matricola: stringa della matricola da cercare.
* fasciaOraria: indice della fascia in cui effettuare la ricerca.
*
* Pre-condizione:
* La stringa matricola deve essere inizializzata.
*
* Post-condizione:
* Ritorna 1 se l'elemento è stato trovato e rimosso, 0 se non presente.
*/
int rimuoviDaCoda(const char *matricola, int fasciaOraria);





/*
* Funzione: visualizzaCoda
* ---
* Stampa il contenuto degli elementi in attesa per la singola fascia.
*
* Parametri:
* fasciaOraria: indice della fascia da visualizzare.
*
* Pre-condizione:
* Nessuna.
*
* Post-condizione:
* Output testuale prodotto su standard output.
*/
void visualizzaCoda(int fasciaOraria);






/*
* Funzione: visualizzaTutteLeCoda
* ---
* Scorre e stampa lo stato di tutte le code gestite dal sistema.
*
* Parametri:
* Nessuno.
*
* Pre-condizione:
* Nessuna.
*
* Post-condizione:
* Riepilogo completo visualizzato a schermo.
*/
void visualizzaTutteLeCoda();






/*
* Funzione: codaVuota
* ---
* Verifica se sono presenti studenti in attesa per la fascia indicata.
*
* Parametri:
* fasciaOraria: indice della fascia da controllare.
*
* Ritorna:
* 1 se la coda è priva di elementi, 0 altrimenti.
*/
int codaVuota(int fasciaOraria);






/*
* Funzione: dimensioneCoda
* ---
* Conteggia il numero di elementi attualmente presenti in una fascia.
*
* Parametri:
* fasciaOraria: indice della fascia.
*
* Ritorna:
* Intero rappresentante il numero totale di studenti in coda.
*/
int dimensioneCoda(int fasciaOraria);






/*
* Funzione: getMatricolaCoda
* ---
* Accede al campo matricola di un elemento della coda.
*
* Parametri:
* e: puntatore all'elemento della coda.
*
* Pre-condizione:
* L'elemento e non deve essere NULL.
*
* Ritorna:
* Puntatore alla stringa contenente la matricola.
*/
const char *getMatricolaCoda(elementoCoda e);






/*
* Funzione: getFasciaCoda
* ---
* Accede al campo relativo alla fascia oraria di un elemento.
*
* Parametri:
* e: puntatore all'elemento della coda.
*
* Ritorna:
* Valore intero della fascia associata all'elemento.
*/
int getFasciaCoda(elementoCoda e);

