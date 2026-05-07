

typedef struct studenteInterno* studente;
//l'utente vede solo il puntatore alla struttura (per garantire l'information hiding)




//aggiungere la tabella studenti (tabella hash)
//con relative funzioni crea e distruggi




int creaStudente(const char*nome, const char*cognome, const char* matricola, const char*corsoDiLaurea);
//creaStudente: alloca e inserisce un nuovo studente nel sistema




int eliminaStudente(const char*matricola);
//eliminaStudente: rimuove lo studente con la matricola indicata




studente cercaStudente(const char*matricola);
//cercaStudente: cerca uno studente per matricola




void stampaStudente(studente s);
//stampaStudente: stampa a schermo i dati dello studente




/*
funzioni utili per leggere i campi interni di una 
prenotazione dall'esterno in modo controllato,
senza mai accedere direttamente alla struct

rispettando quindi l'information hiding
*/


const char *getMatricola(studente s); //ritorna la matricola
const char *getNome(studente s); //ritorna il nome
const char*getCognome(studente s); //ritorna il cognome
const char *getCorsoDiLaurea(studente s); //ritorna il corso di laurea

