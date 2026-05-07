
#define NUM_FASCE 5



typedef struct elementoCodaInterno *elementoCoda;
// Puntatore alla struttura interna della coda




int enqueue(const char *matricola, int fasciaOraria);
// enqueue: inserisce uno studente in coda per una fascia oraria




elementoCoda dequeue(int fasciaOraria);
// dequeue: rimuove e restituisce il primo studente in attesa per una fascia oraria

 


elementoCoda peek(int fasciaOraria);
//peek: restituisce il primo studente in attesa senza rimuoverlo




int rimuoviDaCoda(const char *matricola, int fasciaOraria);
// rimuoviDaCoda: rimuove uno studente specifico dalla coda



 
void visualizzaCoda(int fasciaOraria);
// visualizzaCoda: stampa a schermo tutti gli studenti in attesa per una fascia oraria




void visualizzaTutteLeCoda();
// visualizzaTutteLeCoda: stampa a schermo tutti gli studenti



 
int codaVuota(int fasciaOraria);
//codaVuota: controlla se la coda e' vuota per una fascia




int dimensioneCoda(int fasciaOraria);
//dimensioneCoda: restituisce il numero di studenti in attesa per una fascia oraria



const char *getMatricolaCoda(elementoCoda e); // restituisce la matricola 
int getFasciaCoda(elementoCoda e);   // restituisce la fascia oraria 

