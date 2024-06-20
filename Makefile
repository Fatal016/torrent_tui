CC = gcc
CFLAGS = -Wall -g -Wextra -pedantic

CC_SOURCES = \
Core/Src/main.c

HEADERS = \
Core/Inc/main.h \

EXECUTABLE = tui

OBJECTS = $(CC_SOURCES:.c=.o)

all: $(EXECUTABLE)
	rm -f $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
