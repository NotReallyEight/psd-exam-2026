
#define NUM_FASCE 5

// Puntatore alla struttura interna della coda

typedef struct elementoCodaInterno *elementoCoda;



// enqueue: inserisce uno studente in coda per una fascia oraria

int enqueue(const char *matricola, int fasciaOraria);

// dequeue: rimuove e restituisce il primo studente in attesa per una fascia oraria

elementoCoda dequeue(int fasciaOraria);

//peek: restituisce il primo studente in attesa senza rimuoverlo
 
elementoCoda peek(int fasciaOraria);

// rimuoviDaCoda: rimuove uno studente specifico dalla coda

int rimuoviDaCoda(const char *matricola, int fasciaOraria);



// visualizzaCoda: stampa a schermo tutti gli studenti in attesa per una fascia oraria
 
void visualizzaCoda(int fasciaOraria);

// visualizzaTutteLeCoda: stampa a schermo tutti gli studenti

void visualizzaTutteLeCoda();



//codaVuota: controlla se la coda e' vuota per una fascia
 
int codaVuota(int fasciaOraria);

//dimensioneCoda: restituisce il numero di studenti in attesa per una fascia oraria


int dimensioneCoda(int fasciaOraria);


const char *getMatricolaCoda(elementoCoda e); // restituisce la matricola 
int getFasciaCoda(elementoCoda e);   // restituisce la fascia oraria 

