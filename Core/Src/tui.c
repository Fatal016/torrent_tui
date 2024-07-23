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
#include "../Inc/menu_templates.h"
#include "../Inc/bencode.h"

int main(int argc, char** argv) {

	int result;

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

	/* Setting non-canonical so getchar() is processed immediately */
	set_noncanonical_mode();

	
	/* Data Initialization */
	tracker_info_menu.size_y = bencode.announce_list_index;
	
	tracker_info_menu_items = (struct field_t**)malloc(tracker_info_menu.size_y * sizeof(struct field_t*));
	tracker_info_menu.items = (void *)tracker_info_menu_items;	

	for (int i = 0; i < tracker_info_menu.size_y; i++) {
		((struct field_t**)tracker_info_menu.items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)tracker_info_menu.items)[i]->field_name = (wchar_t*)malloc(4 * sizeof(wchar_t));
		((struct field_t**)tracker_info_menu.items)[i]->field_value = (wchar_t*)malloc(128 * sizeof(wchar_t));	

		swprintf(((struct field_t**)tracker_info_menu.items)[i]->field_name, 3*sizeof(wchar_t), L"%03d:", i); 
		swprintf(((struct field_t**)tracker_info_menu.items)[i]->field_value, strlen(bencode.announce_list[i]) * sizeof(wchar_t), L"%s", bencode.announce_list[i]);
	}
	resize_menu(&tracker_info_menu);

	
	torrent_info_menu_items = (struct field_t**)malloc(torrent_info_menu.size_y * sizeof(struct field_t*));
	torrent_info_menu.items = (void *)torrent_info_menu_items;

	for (int i = 0; i < torrent_info_menu.size_y; i++) {
		int namelen = wcslen(torrent_info_menu_items_template[i].field_name);

		((struct field_t**)torrent_info_menu.items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)torrent_info_menu.items)[i]->field_name = (wchar_t*)malloc(128 * sizeof(wchar_t));
		
		swprintf(((struct field_t**)torrent_info_menu_items)[i]->field_name, namelen * sizeof(wchar_t), L"%ls", torrent_info_menu_items_template[i].field_name);
	}

	resize_menu(&category_menu);

	struct menu_t *active_menu = &category_menu;


	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	setlocale(LC_CTYPE, "");  

	wprintf(L"\033[2J\033[H");

	resize_menu(&category_menu);
	draw_menu(&category_menu);

	category_menu.cur_y = 1;
	set_style(&category_menu, &w);	


	
	torrent_info_menu_items[0]->field_value = strtowstr(argv[1]);
//	swprintf(torrent_info_menu_items[0].field_value, strlen(argv[1]), L"%s", argv[1]);
	resize_menu(&torrent_info_menu);


	/* Hiding cursor */
	wprintf(L"\033[?25l");

	fflush(stdout);
	
	int ch;

	while (1) {
		ch = getchar();

		switch(ch) {
			case UP_ARROW:
				if (active_menu->size_y > w.ws_row && active_menu->cur_y == 1) {
					wprintf(L"\033[1T");
				//	draw_box(active_menu->size_x, active_menu->size_y, 1, 1);
				//	draw_menu(active_menu);
				} else {

					if (active_menu->cur_y > 1) {
						clear_style(active_menu);
						active_menu->cur_y--;
						set_style(active_menu, &w);
					} else {
						clear_style(active_menu);
						active_menu->cur_y = active_menu->size_y;
						set_style(active_menu, &w);
					}
				}
				break;
			case DOWN_ARROW:
				if (active_menu->size_y > w.ws_row && active_menu->cur_y > w.ws_row - 3 && active_menu->cur_y < active_menu->size_y) {
					draw_box(active_menu->size_x, active_menu->size_y, active_menu->ref_x, active_menu->ref_y);
					wprintf(L"\033[1S");
					clear_style(active_menu);
					active_menu->cur_y++;

					wprintf(L"\033[2J\033[H");
					//active_menu->cur_y = 1;
					active_menu->item_offset++;
					draw_field(active_menu);
					set_style(active_menu, &w);
					wprintf(L"\033[0m");
				} else {
					active_menu->item_offset = 0;
					if (active_menu->cur_y < active_menu->size_y) {
						clear_style(active_menu);
						active_menu->cur_y++;
						set_style(active_menu, &w);
					} else {
						clear_style(active_menu);
						active_menu->cur_y = 1;
						set_style(active_menu, &w);
					}
				}
				break;	
			case RIGHT_ARROW:
				if (active_menu->type == MENU) {			
					active_menu = active_menu->items[active_menu->cur_y - 1];
				
					wprintf(L"\033[0m");
					wprintf(L"\033[2J\033[H");
					
					if (active_menu->type == MENU) {
						draw_menu(active_menu);
					} else if (active_menu->type == FIELD) {
						draw_field(active_menu);
					}

					//	active_menu->cur_y = 1;
					set_style(active_menu, &w);
				}	
				break;
			case LEFT_ARROW:
				if (active_menu->prev_menu != NULL) {	
					active_menu = active_menu->prev_menu;

					wprintf(L"\033[0m");
					wprintf(L"\033[2J\033[H");

					draw_menu(active_menu);
					set_style(active_menu, &w);
				}
				break;
			default:
			//	wprintf(L"%ld\n", ch);
				break;
		}

		fflush(stdout);
	}
}

void moveCursor(int x, int y)
{
	wprintf(L"\033[%d;%dH", y, x);
}

void set_noncanonical_mode() {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

wchar_t* strtowstr(char *str) {

	size_t len = strlen(str);
	wchar_t *wstr = malloc(len * sizeof(wchar_t));
	mbstowcs(wstr, str, len);

	return wstr;
}
