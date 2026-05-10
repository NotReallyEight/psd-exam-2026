#include <stdlib.h>
#include "lista-accessi.h"

// ADT della lista concatenata degli accessi
struct nodoAccessi {
	// Valore della testa della lista concatenata
	Accesso accesso;
	// Puntatore al nodo successivo nella lista concatenata
	struct nodoAccessi *next;
};

ListaAccessi creaListaAccessi(void) {
  return NULL;
}

int lunghezzaListaAccessi(ListaAccessi lista) {
  // Inizializza il contatore a 0
  int count = 0;

  while (lista != NULL) {
    // Aumenta il contatore di 1
    count++;
    // Passa al nodo successivo della lista collegata
    lista = lista->next;
  }

  return count;
}

ListaAccessi aggiungiAccesso(ListaAccessi lista, Accesso a) {
  struct nodoAccessi *newNode;
  newNode = malloc(sizeof(struct nodoAccessi));

  // Controllo sicuro per vedere se l'allocazione della memoria con la malloc è avvenuta correttamente
  if (newNode != NULL) {
    // Setta il valore dell'item dato dal client
    newNode->accesso = a;
    // Imposta la lista (cioè il nodo precedentemente in testa) come secondo elemento della lista collegata
    newNode->next = lista;
    lista = newNode;
  }

  return lista;
}

Accesso getFirst(ListaAccessi lista) {
  // Come da specifica, restituisce NULL se la lista è vuota
  if (lista == NULL) return NULL;

  return lista->accesso;
}

ListaAccessi tailList(ListaAccessi lista) {
  // Come da specifica, restituisce NULL se il numero di nodi della lista è minore di 2
  if (getFirst(lista) == NULL) return NULL;

  return lista->next;
}

void stampaListaAccessi(ListaAccessi lista) {
  while (getFirst(lista) != NULL) {
    stampaAccesso(getFirst(lista));
    lista = tailList(lista);
  }

  return;
}

void liberaListaAccessi(ListaAccessi lista) {
  while (lista != NULL) {
    // Salva il riferimento al nodo successivo per poter liberare memoria in avanti
    struct nodoAccessi *next = lista->next;

    // Dealloca la memoria del nodo corrente
    free(lista);

    lista = next;
  }
}
