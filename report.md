# Report Progetto PSD

## Accessi (ADT)

L'ADT `Accesso` rappresenta un singolo record di accesso. È un ADT di utility per l'ADT `ListaAccessi`. I dettagli interni rimangono accessibili solo tramite funzioni dedicate secondo l'information hiding. Ciò significa anche che il client non ha bisogno di modificare nulla se l'implementazione interna cambia.

### `creaAccesso(const char *matricola, const char *data, int fasciaOraria, int postoAssegnato, int haPrenotazione)`

Crea e restituisce un record di accesso con i parametri dati dal codice cliente.

#### Specifica sintattica e semantica

**Specifica Sintattica:**

```c
creaAccesso(const char *, const char *, int, int, int) -> Accesso
```

- Tipi usati: `char *`, `int`, `Accesso`

**Specifica Semantica:**

```c
creaAccesso(matricola, data, fasciaOraria, postoAssegnato, haPrenotazione) -> accesso
```

**Precondizioni:**

- `matricola != NULL`
- `data != NULL`
- `fasciaOraria != NULL`
- `postoAssegnato != NULL`
- `haPrenotazione == 0 || haPrenotazione == 1`

**Postcondizioni:**

`accesso` è il record di accesso appena creato per lo storico di accessi

**Effetti collaterali:**

Nessuno.

#### Razionale dei casi di test

##### Obiettivo

Verificare che `creaAccesso` crei correttamente un accesso gestendo tutti gli edge cases.

##### Casi di test

**Caso 1 - Inserimento valido**

**Input:** `matricola="NF12345678", data="2026-04-04", fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce un accesso valido con campi correttamente inizializzato
**Scopo**: verificare il caso base di creazione dell'accesso

**Caso 2 - Parametro matricola `NULL`**

**Input:** `matricola=NULL, data="2026-04-04", fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare la gestione di parametri obbligatori omessi

**Caso 3 - Parametro data `NULL`**

**Input:** `matricola=NF12345678, data=NULL, fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare la gestione di parametri obbligatori omessi

**Caso 4 - Tutti i parametri di tipo stringa `NULL`**

**Input:** `matricola=NULL, data=NULL, fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare la gestione di parametri obbligatori omessi

**Caso 5 - Stringhe vuote**

**Input:** `matricola="", data="", fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare gestione input sbagliati (stringhe vuote)

**Caso 6 - Fascia oraria non valida**

**Input:** `matricola="NF12345678", data="2026-04-04", fasciaOraria = -1, postoAssegnato = 15, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare gestione input sbagliato (fascia oraria)

**Caso 7 - Posto assegnato non valido**

**Input:** `matricola="NF12345678", data="2026-04-04", fasciaOraria = 2, postoAssegnato = -1, haPrenotazione=1`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare gestione errori con posto non valido

**Caso 8 - Booleano non valido di `haPrenotazione`**

**Input:** `matricola="NF12345678", data="2026-04-04", fasciaOraria = 2, postoAssegnato = 15, haPrenotazione=3`
**Output atteso**: restituisce `NULL`
**Scopo**: verificare che `haPrenotazione` sia `0` oppure `1`

## ListaAccessi (ADT)

L'ADT `ListaAccessi` rappresenta una lista collegata i cui item sono di tipo `Accesso` (descritto precedentemente). Le operazioni che manipolano la sua struttura sono accessibili tramite dedicate funzioni secondo l'information hiding. Viene utilizzata una lista collegata perché non sappiamo il numero di accessi per cui una lista collegata è una scelta migliore (in questa circostanza) rispetto ad un semplice array.

### `creaListaAccessi(void)`

Crea e restituisce una nuova lista collegata di accessi vuota.

#### Specifica sintattica e semantica

**Specifica Sintattica:**

```c
creaListaAccessi() -> ListaAccessi
```

**Specifica Semantica:**

```c
creaListaAccessi() -> listaVuota
```

**Precondizioni:**

Nessuna.

**Postcondizioni:**

`listaVuota` è una lista valida e vuota.

**Effetti collaterali:**

Nessuno.

#### Razionale dei casi di test

##### Obiettivo

Verificare che `creaListaAccessi` restituisca una lista valida e gestisca correttamente condizioni di errore o argomenti inattesi.

##### Casi di test

**Caso 1 - Creazione lista vuota (uso standard)**

**Input:**: `creaListaAccessi()`
**Output atteso**: restituisce una `ListaAccessi` valida che rappresenta una lista vuota
**Scopo**: verificare che la lista venga creata correttamente

### `lunghezzaListaAccessi(ListaAccessi lista)`

La funzione `lunghezzaListaAccessi` restituisce il numero di elementi presenti nella lista di accessi `lista`.

#### Specifica sintattica e semantica

**Specifica Sintattica:**

```c
lunghezzaListaAccessi(ListaAccessi) -> int
```

**Specifica Semantica:**

```c
lunghezzaListaAccessi(lista) -> n
```

`n` è il numero di elementi presenti nella lista `lista`.

**Precondizioni:**

- `lista` deve essere un valore restituito in precedenza da `creaListaAccessi()` oppure un valore valido ottenuto dall'ADT.

**Postcondizioni:**

- Restituisce un intero `n >= 0` con il numero degli elementi nella lista. In caso di errore (ad es. `lista` non valida) il valore sarà `-1`.

**Effetti collaterali:**

- Nessuno.

#### Razionale dei casi di test

##### Obiettivo

Verificare che `lunghezzaListaAccessi` riporti correttamente il numero di elementi in diversi scenari.

##### Casi di test

**Caso 1 - Lista appena creata**

**Input:** `lista = creaListaAccessi(); lunghezzaListaAccessi(lista)`
**Output atteso**: `0`
**Scopo**: verificare che la lista iniziale sia vuota

**Caso 2 - Lista con N elementi**

**Input:** aggiungere N elementi tramite le funzioni di inserimento dell'ADT (es. `aggiungiAccesso`) quindi chiamare `lunghezzaListaAccessi(lista)`.
**Output atteso**: `N`
**Scopo**: verificare che il conteggio rifletta le aggiunte

**Caso 3 - Lista dopo rimozioni**

**Input:** aggiungere M elementi, rimuoverne K, chiamare `lunghezzaListaAccessi(lista)`
**Output atteso**: `M-K`
**Scopo**: verificare consistenza dopo operazioni mutative

**Caso 4 - Argomento `NULL` o lista non inizializzata**

**Input:** `lunghezzaListaAccessi(NULL)` o passare un puntatore non valido
**Output atteso**: `-1`
**Scopo**: verificare uso scorretto
