#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <termios.h>

#include "../Inc/tui.h"
#include "../Inc/menu.h"
#include "../Inc/bencode.h"

int main(int argc, char** argv) {

	int result;

	/* Setting non-canonical so getchar() is processed immediately */
	set_noncanonical_mode();


	if (argc != 2) {
		printf("Usage: ./tui <bencode file>");
		exit(-1);
	}

	char *filepath = argv[1];

	struct bencode_module bencode = {
		.announce               = NULL,
		.announce_list          = NULL,
		.comment                = NULL,
		.created_by             = NULL,
		.encoding               = NULL,
		.info                   = NULL,
		.url_list               = NULL,
		.head_pointer           = NULL,
		.size_pointer           = NULL,
		.announce_list_index    = 0,
		.info_file_index        = 0,
		.file_path_index        = 0,
		.url_list_index         = 0
	};

	result = parse_single(filepath, &bencode);

	struct menu_t *active_menu = &category_menu;


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
				if (active_menu->cur_y > 1) {
					clear_style(active_menu);
					active_menu->cur_y--;
					set_style(active_menu);
				} else {
					clear_style(active_menu);
					active_menu->cur_y = active_menu->size_y;
					set_style(active_menu);
				}
				break;
			case 66:
				if (active_menu->cur_y < active_menu->size_y) {
					clear_style(active_menu);
					active_menu->cur_y++;
					set_style(active_menu);
				} else {
					clear_style(active_menu);
					active_menu->cur_y = 1;
					set_style(active_menu);
				}
				break;	
			case 67:
		
				active_menu = &torrent_info_menu;
				
				wprintf(L"\033[0m");
				wprintf(L"\033[2J\033[H");
				draw_menu(active_menu);
				active_menu->cur_y = 1;
				set_style(active_menu);
				
				break;
			case 68:
			
				if (active_menu->prev_menu != NULL) {	
					active_menu = active_menu->prev_menu;
				}

				wprintf(L"\033[0m");
				wprintf(L"\033[2J\033[H");

				draw_menu(active_menu);
				active_menu->cur_y = 1;
				set_style(active_menu);
	
				break;
			
			default:
			//	wprintf(L"%ld\n", ch);
				break;
		}

		fflush(stdout);
	}
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
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
