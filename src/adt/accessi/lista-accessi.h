#include "accesso.h"

typedef struct nodoAccessi *ListaAccessi;

// creaListaAccessi: crea una lista concatenata di accessi (vuota)
ListaAccessi creaListaAccessi(void);

// lunghezzaListaAccessi: restituisce la lunghezza della lista degli accessi
int lunghezzaListaAccessi(ListaAccessi lista);
