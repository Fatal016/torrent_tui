#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

#include "../Inc/main.h"


int main(int argc, char** argv) {
	

	wchar_t *category_menu[] = {
		L"Torrent Info",
		L"Tracker Info",
		L"Meta Data",
		L"Files"
	};

	int category_menu_size = 4;


	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	//printf("\033[2J\033[H");
	setlocale(LC_CTYPE, "");  

	wprintf(L"\033[2J\033[H");

	
	//printf("\f");
	//printf("\033[2J");
	//printf("\033[H");

//	draw_box(w.ws_row, w.ws_col - 2);


	draw_box(max_size(category_menu, category_menu_size), category_menu_size, 0,0 );
	//moveCursor(1, 1);
	wprintf(L"%ls", category_menu[0]);



/*
	wprintf(L"%lc", 0x2514);
	for (int i = 0; i < w.ws_col - 2; i++) {
		wprintf(L"%lc", 0x2500);	
	}
	wprintf(L"%lc\n", 0x2518);
	for (int i = 0; i < category_menu_size; i++) {
		wprintf(L"%lc")
	}	
*/


	//wprintf(L"%lc Announce List", 0x2502);





	fflush(stdout);

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
	return 12;
}

int draw_box(int size_x, int size_y, int ref_x, int ref_y) 
{
	wprintf(L"\033[%d;%dH", ref_x, ref_y);

	/* Upper bar */
	wprintf(L"%lc", 0x250C);
	for (int i = 0; i < size_x + 2; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc\n", 0x2510);


	/* Sides */
	for (int i = 0; i < size_y; i++) {
		wprintf(L"%lc\033[%dC%lc\n", 0x2502, size_x + 2, 0x2502);
	}	

	/* Lower Bar */
	wprintf(L"%lc", 0x2514);
	for (int i = 0; i < size_x + 2; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc", 0x2518);

	return 0;
}


void moveCursor(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}
