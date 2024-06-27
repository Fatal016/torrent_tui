CC = gcc
CFLAGS = -Wall -g -Wextra -pedantic

CC_SOURCES = \
Core/Src/tui.c \
Core/Src/menu.c

HEADERS = \
Core/Inc/tui.h \
Core/Inc/menu.h \
Core/Inc/bencode.h

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
