#define NULLITEM 0

typedef struct accesso *Accesso;

// creaAccesso: crea un accesso, cioè un item della lista concatenata di accessi
Accesso creaAccesso(const char *matricola, const char *data, int fasciaOraria, int postoAssegnato, int haPrenotazione);
