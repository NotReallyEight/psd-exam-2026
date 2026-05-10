#define NUM_FASCE 5


typedef struct prenotazioneInterna * prenotazione;

int  creaListaPrenotazioni();
// creaListaPrenotazioni: crea lista prenotazioni


void distruggiListaPrenotazioni();
// disstruggiListaPrenotazioni: distrgge lista prenotazioni


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



const char *getMatricolaPrenotazione(prenotazione p); //restituisce la matricola
const char *getDataPrenotazione(prenotazione p);//restituisce la data
int getFasciaOraria(prenotazione p);//restituisce la fascia oraria
int getPostoAssegnato(prenotazione p);//restituisce il posto assegnato