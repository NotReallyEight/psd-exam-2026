# Report Progetto PSD

## NOTE GENERALI

    Al interno di ogni adt ci saranno delle funzioni di "utility" ossia funzioni che permettono di 
    leggere i campi interni dall'esterno ma in modo controllato, senza MAI poter accedere direttamente
    alla struttura (rispetttando l'information hiding)
    es pratico: nel main ci serve sapere il posto assegnato di una prenotazione
        printf("%d", p->posto); SBAGLIATO
        printf("%d", postoAssegnato(p)); CORRETTO

    inoltre supponendo che il nostro software potrebbe essere usato da chiunque
    per preservare ancora di piu la sicurezza abbiamo deciso di usare il 
    "const" sui parametri in input delle funzioni, in modo da dare una sorta
    di "garanzia" al utente. (il const serve a non far modificare dalla funzione gli input che riceve).


### ADT-studente

   Motivazione scelta ADT:



#### Funzioni:
        
##### creaStudente(const char* nome, const char* cognome, const char* matricola, const char* corsoDiLaurea);

        SPECIFICA SINTATTICA:
            creaStudente(const char*,const char*, const char*, const char*,) -> int
            creaStudente (nome,cognome,matricola,corsoDiLaurea)-> int

        SPECIFICA SEMANTICA
            alloca dinamicamente un nuovo studente copiando i dati passati come parametro e lo inserisce nella tabella hash usando la matricola come chiave.

        PRECONDIZIONI
            nome!=  NULL
            cognome!=NULL
            matricola!=NULL & non è già presente nella tabella (essendo univoca)
            corsoDiLaurea !=NULL

        POSTCONDIZIONI
            Successo (0) -> un nuovo studente è stato inserito nella tabella, la memoria è stata allocata correttamente, il contatore degli studenti è incrementato di 1

            Errore (-1) -> la tabella rimane invariata, nessuna memoria viene trattenuta; si verifica se almeno uno tra: parametro NULL, matricola duplicata, malloc fallita


        RAZIONALE DI TEST
            Obiettivo
                Verificare che creaStudente si comporti correttamente sia nei casi validi che in quelli di errore, coprendo tutte le precondizioni definite nella specifica.

            Casi di Test    
            Caso 1 — Inserimento valido
                Input:  nome="Mario", cognome="Rossi", matricola="N46001234", corsoDiLaurea="Informatica"
                Atteso: ritorna 0, studente presente nella tabella
                Scopo:  verificare il caso base di funzionamento corretto

            Caso 2 — Matricola duplicata
                Input:  inserisci due volte la stessa matricola "N46001234"
                Atteso: primo inserimento ritorna 0, secondo ritorna -1
                Scopo:  verificare che il sistema rifiuti matricole già esistenti

            Caso 3 — Parametro NULL
                Input:  nome=NULL, cognome="Rossi", matricola="N46001234", corsoDiLaurea="Informatica"
                Atteso: ritorna -1
                Scopo:  verificare la robustezza ai parametri invalidi

            Caso 4 — Tutti i parametri NULL
                Input:  tutti i parametri a NULL
                Atteso: ritorna -1, nessun crash
                Scopo:  verificare che non ci siano segfault o comportamenti indefiniti

            Caso 5 — Stringhe vuote
                Input:  nome="", cognome="", matricola="", corsoDiLaurea=""
                Atteso: ritorna -1
                Scopo:  verificare la gestione di input degeneri

            Caso 6 — Inserimenti multipli validi
                Input:  10 studenti con matricole diverse
                Atteso: tutti ritornano 0, tutti trovabili con cercaStudente
                Scopo:  verificare stabilità su inserimenti successivi

            Caso 7 — Inserimento dopo eliminazione
                Input:  inserisci "N46001234", elimina "N46001234", reinserisci "N46001234"
                Atteso: tutti e tre ritornano 0
                Scopo:  verificare la corretta gestione del tombstone — la cella liberata viene riutilizzata


##### eliminaStudente(const char* matricola);

        SPECIFICA SINTATTICA:

            eliminaStudente(cosnt char*)->int
            eliminaStudente(matricola) ->int

        SPECIFICA SEMANTICA

            Cerca lo studente con la matricola indicata nella tabella, libera la memoria allocata per i suoi campi e marca la cella come tombstone (occupato = -1) per preservare la correttezza del linear probing.

        PRECONDIZIONI
            matricola!=NULL
            che lo studente con quella matricola esiste nella tabella

        POSTCONDIZIONI 

            Successo (0) -> i campi nome, cognome, corsoDiLaurea e matricola sono stati liberati con free, la cella è marcata come tombstone, il contatore degli studenti è decrementato di 1
            Errore (-1) -> la tabella rimane invariata; si verifica se: matricola è NULL oppure lo studente non esiste nella tabella

        Effetti collaterali 
        La memoria precedentemente allocata per lo studente viene rilasciata

        RAZIONALE CASI DI TEST
            Obiettivo
                Verificare che eliminaStudente rimuova correttamente uno studente esistente, gestisca i casi di errore senza corrompere la struttura e preservi la correttezza del linear probing tramite il tombstone.

            Casi di Test
                Caso 1 — Eliminazione valida
                Input:  inserisci "N46001234", poi elimina "N46001234"
                Atteso: ritorna 0, cercaStudente("N46001234") ritorna NULL
                Scopo:  verificare il caso base di funzionamento corretto

                Caso 2 — Matricola non esistente
                Input:  elimina "N46999999" (mai inserita)
                Atteso: ritorna -1, tabella invariata
                Scopo:  verificare che il sistema non corrompa nulla su chiave inesistente
                
                Caso 3 — Parametro NULL
                Input:  eliminaStudente(NULL)
                Atteso: ritorna -1, nessun crash
                Scopo:  verificare la robustezza ai parametri invalidi

                Caso 4 — Doppia eliminazione
                Input:  inserisci "N46001234", elimina "N46001234", elimina "N46001234" di nuovo
                Atteso: prima eliminazione ritorna 0, seconda ritorna -1
                Scopo:  verificare che il tombstone non venga trattato come elemento attivo

                Caso 5 — Eliminazione e reinserimento
                Input:  inserisci "N46001234", elimina "N46001234", reinserisci "N46001234"
                Atteso: tutte e tre le operazioni ritornano 0
                Scopo:  verificare che la cella tombstone venga correttamente riutilizzata

                Caso 6 — Eliminazione con collisione attiva
                Input:  inserisci due studenti con stessa posizione hash,
                elimina il primo, cerca il secondo
                Atteso: eliminazione ritorna 0, cercaStudente del secondo ritorna puntatore valido
                Scopo:  verificare che il tombstone non spezzi la catena di probing




### ADT-prenotazioni

#### Funzioni:

##### int creaPrenotazione(const char*matricola, const char *data, int fasciaOraria, int posto);

        SPECIFICA SINTATTICA
            creaPrenotazioni(const char*, const char*, int, int)-> int
            creaPrenotazioni (matricola, data, fasciaOraria, posto)->int

        SPECIFICA SEMANTICA
            alloca dinamicamente una nuova prenotazione copiando i dati passati come parametro e la inserisce nella lista. La combinazione matricola + data + fasciaOraria deve essere univoca: uno studente non può avere due prenotazioni nella stessa fascia dello stesso giorno.

        PRECONDIZIONI
            matricola !=NULL
            data != NULL
            0<= fasciaOraria <= NUM_FASCE
            0<= posto<= NUM_POSTI
            Non esiste già una prenotazione con la stessa combinazione matricola + data + fasciaOraria
            Il posto richiesto è disponibile nella fascia oraria indicata

        POSTCONDIZIONI
            Successo (0) -> la prenotazione è stata inserita nella lista, la memoria è stata allocata correttamente, il contatore delle prenotazioni è incrementato di 1, il posto risulta non più disponibile
            Errore (-1) -> la lista rimane invariata, nessuna memoria viene trattenuta; si verifica se: matricola o data sono NULL, fasciaOraria o posto sono negativi, esiste già una prenotazione duplicata, malloc fallita


        RAZIONALE DI CASI
        Razionale dei Casi di Test — creaPrenotazione

        Obiettivo
        Verificare che creaPrenotazione inserisca correttamente una prenotazione nei casi validi e gestisca tutti i casi di errore senza corrompere la struttura.

        Caso 1 — Inserimento valido
            Input:  matricola="N46001234", data="2026-05-10" fasciaOraria=0, posto=5
            Atteso: ritorna 0, cercaPrenotazione restituisce puntatore valido
            Scopo:  verificare il caso base di funzionamento corretto

        Caso 2 — Prenotazione duplicata
            Input:  inserisci due volte matricola="N46001234", data="2026-05-10", fasciaOraria=0
            Atteso: primo inserimento ritorna 0, secondo ritorna -1
            Scopo:  verificare che uno studente non possa prenotare due volte nella stessa fascia dello stesso giorno

        Caso 3 — Parametro NULL
            Input:  matricola=NULL, data="2026-05-10", fasciaOraria=0, posto=5
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare la robustezza ai parametri invalidi

        Caso 4 — Fascia oraria non valida
            Input:  matricola="N46001234", data="2026-05-10", fasciaOraria=-1, posto=5
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare il controllo sul range 0 <= fasciaOraria < NUM_FASCE

        Caso 5 — Fascia oraria fuori range
            Input:  matricola="N46001234", data="2026-05-10", fasciaOraria=5, posto=5
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare che un indice >= NUM_FASCE venga rifiutato

        Caso 6 — Posto negativo
            Input:  matricola="N46001234", data="2026-05-10", fasciaOraria=0, posto=-1
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare il controllo sul valore del posto

        Caso 7 — Stesso studente, fascia diversa
            Input:  
            inserisci matricola="N46001234", data="2026-05-10", fasciaOraria=0 
            inserisci matricola="N46001234", data="2026-05-10", fasciaOraria=1
            Atteso: entrambi ritornano 0
            Scopo:  verificare che uno studente possa prenotare in fasce diverse

        Caso 8 — Inserimenti multipli validi
            Input:  5 studenti con matricole diverse, stessa fascia oraria
            Atteso: tutti ritornano 0, tutti trovabili con cercaPrenotazione
            Scopo:  verificare stabilità su inserimenti successivi


#####  int annullaPrenotazione(const char *matricola, const char *data, int fasciaOraria);

        SPECIFICA SINTATTTICA:
            annullaOrenotazione(const char*, const char* int)->int
            annullaPrenotazione(matricola, data,fasciaOraria)-> int

        SPECIFICA SEMANTICA
            La funzione cerca nella lista la prenotazione identificata dalla combinazione matricola + data + fasciaOraria, la rimuove dalla lista, libera la memoria allocata e rende il posto nuovamente disponibile.

        PRECONDIZIONI
            matricola!=NULL
            data!=NULL
            0<=fasciaOraria<=NUM_FASCE
            Esiste una prenotazione con la combinazione matricola + data + fasciaOraria

        POSTCONDIZIONI
            Successo (0) -> la prenotazione è stata rimossa dalla lista, la memoria è stata liberata, il posto torna disponibile, il contatore delle prenotazioni è decrementato di 1
            Errore (-1) -> la lista rimane invariata; si verifica se: matricola o data sono NULL, fasciaOraria fuori range, prenotazione non trovata

            Effetti collaterali
            La memoria allocata per la prenotazione viene rilasciata

        RAZIONALE CASI 

        Obiettivo
        Verificare che annullaPrenotazione rimuova correttamente una prenotazione esistente, liberi la memoria e gestisca tutti i casi di errore senza corrompere la lista.

        Caso 1 — Annullamento valido
            Input:  inserisci matricola="N46001234", data="2026-05-10", fasciaOraria=0 poi annulla matricola="N46001234", data="2026-05-10", fasciaOraria=0
            Atteso: ritorna 0, cercaPrenotazione restituisce NULL
            Scopo:  verificare il caso base di funzionamento corretto

        Caso 2 — Prenotazione non esistente
        Input:  annulla matricola="N46999999", data="2026-05-10", fasciaOraria=0 (mai inserita)
        Atteso: ritorna -1, lista invariata
        Scopo:  verificare che il sistema non corrompa nulla su prenotazione inesistente

        Caso 3 — Parametro NULL
            Input:  annullaPrenotazione(NULL, "2026-05-10", 0)
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare la robustezza ai parametri invalidi

        Caso 4 — Fascia oraria non valida
            Input:  annullaPrenotazione("N46001234", "2026-05-10", -1)
            Atteso: ritorna -1, lista invariata
            Scopo:  verificare il controllo sul range 0 <= fasciaOraria < NUM_FASCE

        Caso 5 — Doppio annullamento
            Input:  inserisci e annulla matricola="N46001234", data="2026-05-10", fasciaOraria=0 poi annulla di nuovo la stessa prenotazione
            Atteso: primo annullamento ritorna 0, secondo ritorna -1
            Scopo:  verificare che una prenotazione già rimossa non possa essere annullata una seconda volta

        Caso 6 — Annullamento e reinserimento
            Input:  inserisci, annulla e reinserisci matricola="N46001234", data="2026-05-10", fasciaOraria=0, posto=5
            Atteso: tutte e tre le operazioni ritornano 0
            Scopo:  verificare che dopo un annullamento sia possibile reinserire la stessa prenotazione

        Caso 7 — Annullamento in mezzo alla lista
            Input:  inserisci 3 prenotazioni diverse, annulla quella in mezzo
            Atteso: annullamento ritorna 0, le altre due sono ancora trovabili con cercaPrenotazione
            Scopo:  verificare la corretta gestione dei puntatori prev e next durante la rimozione di un nodo centrale

##### prenotazione cercaPrenotazione(const char *matricola, const char *data,int fasciaOraria)

        SPECIFICA SINTATTICA
            cercaprenotazione(const char*,const char*, int)->prenotazione
            cercaPrenotazione(matricola, data,fasciaOraria)->prenotazione

        SPECIFICASEMANTICA
            Scorre la lista delle prenotazioni cercando quella identificata dalla combinazione univoca matricola + data + fasciaOraria. Restituisce un puntatore alla prenotazione trovata, accessibile solo tramite le funzioni di utility.

            PRECONDIZIONI
             





