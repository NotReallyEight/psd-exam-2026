#include "accesso.h"

typedef struct nodoAccessi *ListaAccessi;

ListaAccessi creaListaAccessi(void);
// creaListaAccessi: crea una lista concatenata di accessi (vuota)

int lunghezzaListaAccessi(ListaAccessi lista);
// lunghezzaListaAccessi: restituisce la lunghezza della lista degli accessi

ListaAccessi aggiungiAccesso(ListaAccessi lista, Accesso a);
// aggiungiAccesso: aggiunge un accesso in testa alla lista degli accessi e restituisce la lista aggiornata

Accesso getFirst(ListaAccessi lista);
// getFirst: restituisce il primo accesso della lista, NULL se la lista è vuota

ListaAccessi tailList(ListaAccessi lista);
// tailList: restituisce la lista degli accessi escluso il primo elemento. Restituisce NULL se il numero di nodi della lista è minore di 2

void stampaListaAccessi(ListaAccessi lista);
// stampaListaAccessi: stampa tutte le informazioni riguardanti ciascun accesso della lista

void liberaListaAccessi(ListaAccessi lista);
// liberaListaAccessi: dealloca la memoria occupata dalla lista degli accessi e da ciascun accesso contenuto
