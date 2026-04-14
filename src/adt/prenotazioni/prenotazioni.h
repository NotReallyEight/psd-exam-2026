
typedef struct prenotazioneInterna * prenotazione;

int creaPrenotazione(const char*matricola, const char*data, int fasciaOraria, int posto);
//creaPrenotazione: alloca e inserisce una nuova prenotazione

int annullaPrenotazione(const char*matricola, const char*data, int fasciaOraria);
//annullaPrenotazione: rimuove una prenotazione GIA esistente

prenotazione cercaPrenotazione(const char *matricola, const char *data,int fasciaOraria);
//cercaPrenotazione: cerca una prenotazione per matricola e data

void visualizzaPrenotazioni();
//visualizzaPrenotazioni: stampa tutte le prenotazioni attive

void visualizzaPrenotazioniPerFascia(int fasciaOraria);
//visualizzaPrenotazioniPerFascia: stampa le prenotazioni


/*
funzioni utili per leggere i campi interni di una 
prenotazione dall'esterno in modo controllato,
senza mai accedere direttamente alla struct

rispettando quindi l'information hidig
*/
static char *matricolaPrenotazione(prenotazione p); //restituisce la matricola
static char *dataPrenotazione(prenotazione p);//restituisce la data
static int fasciaOraria(prenotazione p);//restituisce la fascia oraria
static int postoAssegnato(prenotazione p);//restituisce il posto assegnato