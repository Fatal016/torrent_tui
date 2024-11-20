CC = gcc
CFLAGS = -Wall -g -Wextra -pedantic

CC_SOURCES = \
Core/Src/tui.c \
Core/Src/menu.c \
Core/Src/menu_templates.c \
Core/Src/data.c \
Core/Src/bencode.c

HEADERS = \
Core/Inc/tui.h \
Core/Inc/menu.h \
Core/Inc/menu_templates.h \
Core/Inc/data.h \
Core/Inc/bencode.h

EXECUTABLE = tui.bin

OBJECTS = $(CC_SOURCES:.c=.o)

all: $(EXECUTABLE)
	rm -f $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
