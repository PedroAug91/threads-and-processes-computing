CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
LDFLAGS = -pthread

SRC     = ./src
OBJ     = $(SRC)/main.o $(SRC)/arguments.o
TARGET  = varredor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(SRC)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC)/main.o: $(SRC)/main.c $(SRC)/arguments.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRC)/*.o $(TARGET)

.PHONY: all clean
