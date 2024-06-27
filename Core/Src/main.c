#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <termios.h>

#include "../Inc/main.h"

int main(int argc, char** argv) {

	/* Setting non-canonical so getchar() is processed immediately */
	set_noncanonical_mode();
	
	wchar_t *category_menu_items[] = {
		L"Torrent Info",
		L"Tracker Info",
		L"Meta Data",
		L"Files"
	};
	int category_menu_size = 4;

	struct menu_t category_menu = {
		.items = category_menu_items,
		.ref_x = 1,
		.ref_y = 1,
		.item_index = 0,
		.size_x = max_size(category_menu_items, category_menu_size),
		.size_y = category_menu_size,
		.cur_x = 0,
		.cur_y = 0
	};

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	setlocale(LC_CTYPE, "");  

	wprintf(L"\033[2J\033[H");


	draw_menu(&category_menu);
	

	category_menu.cur_y = 1;
	set_style(&category_menu);	


	/* Hiding cursor */
	wprintf(L"\033[?25l");

	fflush(stdout);

	int ch;

	while (1) {
		ch = getchar();

		switch(ch) {
			case 65:
				if (category_menu.cur_y > 1) {
					clear_style(&category_menu);
					category_menu.cur_y--;
					set_style(&category_menu);
				}
				break;
			case 66:
				if (category_menu.cur_y < category_menu.size_y) {
					clear_style(&category_menu);
					category_menu.cur_y++;
					set_style(&category_menu);
				}
				break;	
		}

		fflush(stdout);
	}
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
		if (ref_x != 1) {
			wprintf(L"\033[%dC", ref_x-1);
		}
		wprintf(L"%lc\033[%dC%lc\n", 0x2502, size_x + 1, 0x2502);
	}	

	/* Lower Bar */
	if (ref_x != 1) {
		wprintf(L"\033[%dC", ref_x-1);
	}
	wprintf(L"%lc", 0x2514);
	

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

int clear_style(struct menu_t *menu) {
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[0m%*s", menu->size_x + 1, "");
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[0m %ls", menu->items[menu->cur_y - 1]);

	return 0;
}

int set_style(struct menu_t *menu) {
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[47m%*s", menu->size_x + 1, "");
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[30m %ls", menu->items[menu->cur_y - 1]);

	return 0;
}

void set_noncanonical_mode() {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
