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
#include "../Inc/data.h"
#include "../Inc/bencode.h"

int main(int argc, char** argv) {

	int result, ch;

	if (argc != 2) {
		printf("Usage: ./tui <bencode file>");
		exit(-1);
	}

	char *filepath = argv[1];

	struct bencode_module bencode;
	result = parse_single(filepath, &bencode);

	if (result != 0) {
		printf("Error parsing file\n");
		return -1;
	}

	/* Setting non-canonical so getchar is processed immediately */
	set_noncanonical_mode();
	
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	setlocale(LC_CTYPE, "");  

	/***********************/
	/* Data Initialization */
	/***********************/

	/* Torrent Info */
	torrent_info_menu_items = (struct field_t**)malloc(torrent_info_menu.size_y * sizeof(struct field_t*));
	torrent_info_menu.items = (void *)torrent_info_menu_items;	
	constructTorrentInfo(&torrent_info_menu, torrent_info_menu_items_template);
	swprintf(torrent_info_menu_items[0]->field_value, strlen(argv[1]) * sizeof(wchar_t), L"%s", argv[1]);
	resize_menu(&torrent_info_menu);

	/* Tracker Info */
	tracker_info_menu.size_y = bencode.announce_list_index;	
	tracker_info_menu.items = (void *)malloc(tracker_info_menu.size_y * sizeof(struct field_t*));
	constructTrackerInfo(&tracker_info_menu, &bencode);
	resize_menu(&tracker_info_menu);

	/* Meta Info */
	meta_info_menu_items = (struct field_t**)malloc(meta_info_menu.size_y * sizeof(struct field_t*));
	meta_info_menu.items = (void *)meta_info_menu_items;	
	constructMetaInfo(&meta_info_menu, meta_info_menu_items_template, &bencode);
	resize_menu(&meta_info_menu);

	/* Files */
	files_menu.size_y = bencode.info_file_index;
	files_menu.items = (void *)malloc(files_menu.size_y * sizeof(struct field_t*));
	constructFiles(&files_menu, &bencode);
	resize_menu(&files_menu);


	wprintf(L"\033[2J\033[H");

	struct menu_t *active_menu = &category_menu;
	
	resize_menu(active_menu);
	draw_menu(active_menu);
	set_style(active_menu, &w);	

	/* Hiding cursor */
	wprintf(L"\033[?25l");

	fflush(stdout);
	
	while (1) {
		ch = getchar();

		switch(ch) {
			case UP_ARROW:
				if (active_menu->item_offset > 0) {
					active_menu->item_offset--;
					draw_box(active_menu->size_x, active_menu->size_y, active_menu->ref_x, active_menu->ref_y);
					wprintf(L"\033[1S");
					clear_style(active_menu, &w);
					active_menu->cur_y--;
					wprintf(L"\033[2J\033[H");
					draw_field(active_menu);
					set_style(active_menu, &w);
					wprintf(L"\033[0m");
				} else {

					if (active_menu->cur_y > 1) {
						clear_style(active_menu, &w);
						active_menu->cur_y--;
						set_style(active_menu, &w);
					} else {
				//		clear_style(active_menu);
				//		active_menu->cur_y = active_menu->size_y;
				//		set_style(active_menu, &w);
					}
				}
				break;
			case DOWN_ARROW:
				if (active_menu->size_y > w.ws_row && active_menu->cur_y > w.ws_row - 2 && active_menu->cur_y <= active_menu->size_y) {
				
					if (active_menu->cur_y == active_menu->size_y) break;


					draw_box(active_menu->size_x, active_menu->size_y, active_menu->ref_x, active_menu->ref_y);
					wprintf(L"\033[1S");
					clear_style(active_menu, &w);
					active_menu->cur_y++;
					wprintf(L"\033[2J\033[H");
					active_menu->item_offset++;
					draw_field(active_menu);
					set_style(active_menu, &w);
					wprintf(L"\033[0m");
				} else {
					active_menu->item_offset = 0;
					if (active_menu->cur_y < active_menu->size_y) {
						clear_style(active_menu, &w);
						active_menu->cur_y++;
						set_style(active_menu, &w);
					} else {
				//		clear_style(active_menu);
				//		active_menu->cur_y = 1;
				//		set_style(active_menu, &w);
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
