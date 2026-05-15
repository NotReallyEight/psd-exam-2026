#include "accesso.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// La struct con tutte le proprietà necessarie per creare un record di un accesso
struct accesso
{
  char *matricola;
  char *data;
  int fasciaOraria;
  int postoAssegnato;
  int haPrenotazione;
};

Accesso creaAccesso(const char *matricola, const char *data, int fasciaOraria, int postoAssegnato, int haPrenotazione)
{
  // Alloca memoria per un nuovo accesso
  Accesso nuovoAccesso = malloc(sizeof(struct accesso));

  // Se l'allocazione fallisce, restituisce NULL
  if (nuovoAccesso == NULL)
  {
    return NULL;
  }

  // Copia i dati nei rispettivi campi della struct Accesso
  strcpy(nuovoAccesso->matricola, matricola);
  strcpy(nuovoAccesso->data, data);
  nuovoAccesso->fasciaOraria = fasciaOraria;
  nuovoAccesso->postoAssegnato = postoAssegnato;
  nuovoAccesso->haPrenotazione = haPrenotazione;

  return nuovoAccesso;
}

void stampaAccesso(Accesso a)
{
  // Stampa tutte le informazioni riguardanti un accesso
  printf("%-12s | %-10s | Fascia:%d | Posto:%-2d | Prenotato:%s\n",
         a->matricola, a->data, a->fasciaOraria, a->postoAssegnato,
         a->haPrenotazione ? "SÌ" : "NO");
}
