# Report Progetto PSD


## ADT-studente

   Motivazione scelta ADT:

#### Funzioni:
        
#### creaStudente(const char* nome, const char* cognome, const char* matricola, const char* corsoDiLaurea);

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


#### eliminaStudente(const char* matricola);

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

