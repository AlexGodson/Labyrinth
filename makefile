CC=gcc
INC = /include
# libraries
# libs = 
# Compiler Flags
CFLAGS=-Wall -Werror
# Source File Folder
_SRC=src
SRC=$(wildcard $(_SRC)/*.c)
_OBJ=src/OBJ
OBJ=$(patsubst $(_SRC)/%.c, $(_OBJ)/%.o, $(SRC))
# Output Program
OUT=main

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(libs)

$(_OBJ)/%.o: $(_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(libs)


run: all
	./$(OUT) /home/alexg/Code/
	./$(OUT) ~/Downloads
	./$(OUT) .

debug: $(OBJ)
	$(CC) -g $(CFLAGS) $(OBJ) -o $(OUT) $(libs)

test: $(OUT)
	valgrind --leak-check=yes --track-origins=yes -s ./main .

clear:
	rm $(OBJ)*
	rm $(OUT)

