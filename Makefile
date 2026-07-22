TARGET=game
CC=gcc
FLAGS=-O3
LIBS=-lncurses

all: $(TARGET)

$(TARGET): $(TARGET).c
	@$(CC) $(FLAGS) -o $(TARGET) $(TARGET).c $(LIBS)


clean:
	rm -rf $(TARGET)
