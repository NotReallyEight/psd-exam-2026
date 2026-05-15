/*
 * File: prenotazioni.h
 * ---
 * Interfaccia pubblica dell'ADT prenotazioni.
 *
 * Il modulo gestisce le prenotazioni dei posti
 * nell'aula studio, organizzate per fascia oraria.
 *
 * Autore: Raffaele Severino
 * Data inizio: 02/04/2026
 * Data ultima modifica: 13/05/2026
 */



#define NUM_FASCE 5


/*
 * Puntatore alla struttura interna della prenotazione.
 * L'implementazione è nascosta nel file .c
 * per garantire l'information hiding.
 */
typedef struct prenotazioneInterna *prenotazione;





/*
 * Funzione: creaListaPrenotazioni
 * ---
 * Alloca e inizializza la lista che conterrà
 * le prenotazioni del sistema.
 *
 * Parametri:
 * Nessuno.
 *
 * Pre-condizione:
 * La lista non deve essere già inizializzata.
 *
 * Post-condizione:
 * La lista è pronta all'uso con zero prenotazioni.
 *
 * Ritorna:
 * 0 se la creazione ha successo, -1 in caso di errore.
 */
int creaListaPrenotazioni();






/*
 * Funzione: distruggiListaPrenotazioni
 * ---
 * Dealloca la lista e tutte le risorse
 * dinamiche ad essa associate.
 *
 * Parametri:
 * Nessuno.
 *
 * Pre-condizione:
 * Nessuna.
 *
 * Post-condizione:
 * La lista non è più disponibile nel sistema
 * e tutta la memoria è stata liberata.
 */
void distruggiListaPrenotazioni();






/*
 * Funzione: creaPrenotazione
 * ---
 * Inserisce una nuova prenotazione nella lista.
 *
 * La combinazione matricola + data + fasciaOraria
 * deve essere univoca nel sistema.
 *
 * Parametri:
 * matricola: identificativo dello studente.
 * data: data della prenotazione (es. "2026-05-10").
 * fasciaOraria: indice della fascia (da 0 a NUM_FASCE-1).
 * posto: numero del posto da assegnare.
 *
 * Pre-condizione:
 * matricola e data devono essere stringhe valide
 * e non nulle. fasciaOraria deve rientrare nel
 * range definito. posto deve essere >= 0.
 *
 * Post-condizione:
 * Se non esistono duplicati, la prenotazione
 * viene registrata nel sistema.
 *
 * Ritorna:
 * 0 se l'inserimento ha successo, -1 in caso di errore.
 */
int creaPrenotazione(const char *matricola, const char *data, int fasciaOraria, int posto);






/*
 * Funzione: annullaPrenotazione
 * ---
 * Rimuove dalla lista la prenotazione identificata
 * dalla combinazione matricola + data + fasciaOraria.
 *
 * Parametri:
 * matricola: identificativo dello studente.
 * data: data della prenotazione.
 * fasciaOraria: indice della fascia oraria.
 *
 * Pre-condizione:
 * matricola e data devono essere stringhe valide
 * e non nulle. fasciaOraria deve rientrare nel
 * range definito.
 *
 * Post-condizione:
 * Se presente, la prenotazione viene rimossa
 * e la memoria liberata.
 *
 * Ritorna:
 * 0 se la rimozione ha successo, -1 se la
 * prenotazione non esiste o in caso di errore.
 */
int annullaPrenotazione(const char *matricola, const char *data,int fasciaOraria);






/*
 * Funzione: cercaPrenotazione
 * ---
 * Cerca una prenotazione tramite la combinazione
 * matricola + data + fasciaOraria.
 *
 * Parametri:
 * matricola: identificativo dello studente.
 * data: data della prenotazione.
 * fasciaOraria: indice della fascia oraria.
 *
 * Pre-condizione:
 * matricola e data devono essere stringhe valide
 * e non nulle. fasciaOraria deve rientrare nel
 * range definito.
 *
 * Post-condizione:
 * Nessuna modifica ai dati interni.
 *
 * Ritorna:
 * Puntatore alla prenotazione trovata,
 * NULL se assente o in caso di errore.
 */
prenotazione cercaPrenotazione(const char *matricola, const char *data, int fasciaOraria);







/*
 * Funzione: visualizzaPrenotazioni
 * ---
 * Stampa a schermo tutte le prenotazioni
 * attualmente presenti nel sistema.
 *
 * Parametri:
 * Nessuno.
 *
 * Pre-condizione:
 * La lista deve essere stata inizializzata.
 *
 * Post-condizione:
 * Nessuna modifica ai dati interni.
 * Se la lista è vuota viene stampato
 * un messaggio informativo.
 */
void visualizzaPrenotazioni();






/*
 * Funzione: visualizzaPrenotazioniPerFascia
 * ---
 * Stampa a schermo le prenotazioni relative
 * alla fascia oraria indicata.
 *
 * Parametri:
 * fasciaOraria: indice della fascia da visualizzare.
 *
 * Pre-condizione:
 * fasciaOraria deve rientrare nel range definito.
 *
 * Post-condizione:
 * Nessuna modifica ai dati interni.
 * Se non esistono prenotazioni per la fascia
 * viene stampato un messaggio informativo.
 */
void visualizzaPrenotazioniPerFascia(int fasciaOraria);







/*
 * Funzioni getter.
 * ---
 * Consentono la lettura controllata dei campi interni
 * senza accesso diretto alla struttura dati.
 */

/*
 * Funzione: getMatricolaPrenotazione
 * ---
 * Restituisce la matricola della prenotazione.
 *
 * Parametri:
 * p: prenotazione da consultare.
 *
 * Ritorna:
 * Puntatore alla stringa matricola,
 * NULL se p non è valido.
 */
const char *getMatricolaPrenotazione(prenotazione p);





/*
 * Funzione: getDataPrenotazione
 * ---
 * Restituisce la data della prenotazione.
 *
 * Parametri:
 * p: prenotazione da consultare.
 *
 * Ritorna:
 * Puntatore alla stringa data,
 * NULL se p non è valido.
 */
const char *getDataPrenotazione(prenotazione p);






/*
 * Funzione: getFasciaOraria
 * ---
 * Restituisce la fascia oraria della prenotazione.
 *
 * Parametri:
 * p: prenotazione da consultare.
 *
 * Ritorna:
 * Indice della fascia oraria,
 * -1 se p non è valido.
 */
int getFasciaOraria(prenotazione p);







/*
 * Funzione: getPostoAssegnato
 * ---
 * Restituisce il posto assegnato alla prenotazione.
 *
 * Parametri:
 * p: prenotazione da consultare.
 *
 * Ritorna:
 * Numero del posto assegnato,
 * -1 se p non è valido.
 */
int getPostoAssegnato(prenotazione p);

