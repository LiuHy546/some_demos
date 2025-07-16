CC = gcc
CFLAGS = -Wall -Wextra
TARGET = example_program
SOURCE = example_program.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean 