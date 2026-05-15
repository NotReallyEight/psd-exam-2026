#define NULLITEM 0

typedef struct accesso *Accesso;

Accesso creaAccesso(const char *matricola, const char *data, int fasciaOraria, int postoAssegnato, int haPrenotazione);
// creaAccesso: crea un accesso, cioè un item della lista concatenata di accessi

void stampaAccesso(Accesso a);
// stampaAccesso: stampa tutte le informazioni riguardanti un accesso
