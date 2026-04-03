#include <stdio.h>
#include <stdlib.h>

#include "studente.h";


struct studenteInterno{
    char*nome; // stringa con nome studente
    char*cognome; //"" cognome studente
    char* matricola;//"" matricola studente
    char*corsoDiLaurea; // "" corso di laurea studente
    int attivo; // utile per la tabella 1= attivo; 0 =libero, -1=tombstone
};