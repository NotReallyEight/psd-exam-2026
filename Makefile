# ============================================================
# Makefile - psd-exam-2026
# Standard: C99 | Compilatore: gcc
# ============================================================

CC     = gcc
STD    = -std=c99
WARNS  = -Wall -Wextra
CFLAGS = $(STD) $(WARNS)

# --- Percorsi ---

SRC_ADT  = src/adt
INC_ADT  = include/adt
TESTS    = tests

# --- Sorgenti ADT ---

ADT_SRCS = \
    $(SRC_ADT)/studente.c \
    $(SRC_ADT)/prenotazioni.c \
    $(SRC_ADT)/listaAttesa.c \
    $(SRC_ADT)/accesso.c \
    # $(SRC_ADT)/posti.c \
    $(SRC_ADT)/lista-accessi.c

# --- Flag inclusione header ---

INC = -I$(INC_ADT)

#
# Target principale: compila ed esegue tutti i test
#			

all: test1 test2 test6

# --- Caso di test 1: Registrazione studenti ---

test1: $(TESTS)/1-caso/test_registrazione_studenti.c $(ADT_SRCS)
	$(CC) $(CFLAGS) $(INC) \
		$(TESTS)/1-caso/test_registrazione_studenti.c \
		$(ADT_SRCS) \
		-o $(TESTS)/1-caso/test1
	./$(TESTS)/1-caso/test1



# --- Caso di test 2: Inserimento prenotazioni ---

test2: $(TESTS)/2-caso/test_inserimento_prenotazioni.c $(ADT_SRCS)
	$(CC) $(CFLAGS) $(INC) \
		$(TESTS)/2-caso/test_inserimento_prenotazioni.c \
		$(ADT_SRCS) \
		-o $(TESTS)/2-caso/test2
	./$(TESTS)/2-caso/test2


# --- Caso di test 6: Lista di attesa ---
test6: $(TESTS)/6-caso/test_lista_attesa.c $(ADT_SRCS)
	$(CC) $(CFLAGS) $(INC) \
		$(TESTS)/6-caso/test_lista_attesa.c \
		$(ADT_SRCS) \
		-o $(TESTS)/6-caso/test6
	./$(TESTS)/6-caso/test6

# 
# Pulizia eseguibili generati
# 

clean:
	rm -f $(TESTS)/1-caso/test1 \
	      $(TESTS)/2-caso/test2 \
	      $(TESTS)/6-caso/test6

.PHONY: all test1 test2 test6 clean
