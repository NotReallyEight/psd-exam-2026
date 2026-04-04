#include "accesso.h"

// La struct con tutte le proprietà necessarie per creare un record di un accesso
struct accesso {
	char *matricola;
	char *data;
	int fasciaOraria;
	int postoAssegnato;
	int haPrenotazione;
};
