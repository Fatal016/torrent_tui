#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

#include "../Inc/main.h"

int main(int argc, char** argv) {
	
	wchar_t *category_menu_items[] = {
		L"Torrent Info",
		L"Tracker Info",
		L"Meta Data",
		L"Files"
	};
	int category_menu_size = 4;

	struct menu_t category_menu = {
		.items = category_menu_items,
		.ref_x = 40,
		.ref_y = 20,
		.item_index = 0,
		.size_x = max_size(category_menu_items, category_menu_size),
		.size_y = category_menu_size
	};

	int bytes_waiting;


	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	setlocale(LC_CTYPE, "");  

	wprintf(L"\033[2J\033[H");


	draw_menu(&category_menu);
	

	moveCursor(category_menu.ref_x + 1, category_menu.ref_y + 1);
	
	/* This doesn't fully work since I want it based on the box, not the text */
	wprintf(L"\033[30;47m %ls \033[?25l", category_menu.items[0]);

	
	//wprintf(L"\033[47m%20s\033[0m", "");
	//wprintf(L"\x1b[30m[%dC", 20);
	//wprintf(L"\x1b[47m\x1b[30m[%dC", wcslen(category_menu.items[0]));

	fflush(stdout);

	char c;

	while (1) {
		// Check if input is available
		ioctl(STDIN_FILENO, FIONREAD, &bytes_waiting);

		if (bytes_waiting > 0) {
			c = getchar();  // Read a character from stdin
		wprintf(L"in here\n");

			if (c == 27) {  // Check if it's the escape character (ASCII 27)
				if (getchar() == '[') {  // Check if the next character is '['
					switch (getchar()) {  // Read the actual key code
						case 'A':
							printf("Up arrow key pressed\n");
							break;
						case 'B':
							wprintf(L"Down arrow key pressed\n");
							break;
						case 'C':
							printf("Right arrow key pressed\n");
							break;
						case 'D':
							printf("Left arrow key pressed\n");
							break;
						default:
							break;
					}
				}
			}

			// Exit the loop on 'q' key press
			if (c == 'q') {
				break;
			}
		}

		// Optionally sleep or perform other tasks here
	}




	while (1) {}
}

int max_size(wchar_t** items, int len)
{
/*
	int max = 0;

	for (int i = 0; i < len; i++) {
		if (strlen(items[i]) > max) {
			max = strlen(items[i]);
		}
	}

	return max;
*/
	return 13;
}

int draw_box(int size_x, int size_y, int ref_x, int ref_y) 
{
	wprintf(L"\033[%d;%dH", ref_y, ref_x);

	/* Upper bar */
	wprintf(L"%lc", 0x250C);
	for (int i = 0; i < size_x + 1; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc\n", 0x2510);


	/* Sides */
	for (int i = 0; i < size_y; i++) {
		wprintf(L"\033[%dC%lc\033[%dC%lc\n", ref_x-1, 0x2502, size_x + 1, 0x2502);
	}	

	/* Lower Bar */
	wprintf(L"\033[%dC%lc", ref_x-1, 0x2514);
	for (int i = 0; i < size_x + 1; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc", 0x2518);

	return 0;
}

int draw_menu(struct menu_t *menu)
{
	draw_box(menu->size_x, menu->size_y, menu->ref_x, menu->ref_y);
	for (int i = 0; i < menu->size_y; i++) {
		moveCursor(menu->ref_x + 2, menu->ref_y + 1 + i);
		wprintf(L"%ls", menu->items[i]);
	}
	return 0;
}

void moveCursor(int x, int y)
{
	wprintf(L"\033[%d;%dH", y, x);
}
