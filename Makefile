CC = gcc
CFLAGS = -Wall -g -Wextra -pedantic

CC_SOURCES = \
Core/Src/tui.c \
Core/Src/menu.c \
Core/Src/menu_templates.c \
Core/Src/data.c \
Core/Src/bencode.c \
Core/Src/client.c \
Core/Src/client_utils.c

HEADERS = \
Core/Inc/tui.h \
Core/Inc/menu.h \
Core/Inc/menu_templates.h \
Core/Inc/data.h \
Core/Inc/bencode.h \
Core/Inc/client.h \
Core/Inc/client_utils.h

EXECUTABLE = tui.bin

OBJECTS = $(CC_SOURCES:.c=.o)

all: $(EXECUTABLE)
	rm -f $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lssl -lcrypto -lcurl

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
