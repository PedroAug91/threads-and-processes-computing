CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O3
LDFLAGS = -pthread

SRC     = ./src
OBJ     = $(SRC)/main.o $(SRC)/arguments.o $(SRC)/compute.o
TARGET  = varredor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(SRC)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC)/main.o: $(SRC)/main.c $(SRC)/arguments.h $(SRC)/compute.o
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SRC)/*.o $(TARGET)

.PHONY: all clean
