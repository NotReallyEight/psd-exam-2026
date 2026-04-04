#include "lista-accessi.h"

// ADT della lista concatenata degli accessi
struct nodoAccessi {
	// Valore della testa della lista concatenata
	Accesso accesso;
	// Puntatore al nodo successivo nella lista concatenata
	struct nodoAccessi *next;
};
