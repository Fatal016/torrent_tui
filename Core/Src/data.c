#include <string.h>
#include <wchar.h>
#include <stdlib.h>

#include "../Inc/menu.h"
#include "../Inc/bencode.h"

int constructTorrentInfo(struct menu_t *menu, struct field_t *template){

	for (int i = 0; i < menu->size_y; i++) {
		int namelen = wcslen(template[i].field_name);

		((struct field_t**)menu->items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)menu->items)[i]->field_name = (wchar_t*)malloc(128 * sizeof(wchar_t));
		
		((struct field_t**)menu->items)[i]->field_value = (wchar_t*)malloc(128 * sizeof(wchar_t));


		swprintf(((struct field_t**)menu->items)[i]->field_name, namelen * sizeof(wchar_t), L"%ls", template[i].field_name);
	}

	return 0;
}

int constructTrackerInfo(struct menu_t *menu, struct bencode_module *bencode) {

	for (int i = 0; i < menu->size_y; i++) {
		((struct field_t**)menu->items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)menu->items)[i]->field_name = (wchar_t*)malloc(4 * sizeof(wchar_t));
		((struct field_t**)menu->items)[i]->field_value = (wchar_t*)malloc(128 * sizeof(wchar_t));

		swprintf(((struct field_t**)menu->items)[i]->field_name, 3*sizeof(wchar_t), L"%03d:", i);
		swprintf(((struct field_t**)menu->items)[i]->field_value, strlen(bencode->announce_list[i]) * sizeof(wchar_t), L"%s", bencode->announce_list[i]);
	}

	return 0;
}

int constructFiles(struct menu_t *menu, struct bencode_module *bencode) {
	
	for (int i = 0; i < menu->size_y; i++) {
		((struct field_t**)menu->items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)menu->items)[i]->field_name = (wchar_t*)malloc(4 * sizeof(wchar_t));
		((struct field_t**)menu->items)[i]->field_value = (wchar_t*)malloc(128 * sizeof(wchar_t));

		swprintf(((struct field_t**)menu->items)[i]->field_name, 3*sizeof(wchar_t), L"%03d:", i);
		
		for (int j = 0; j < bencode->info->files[i]->file_path_index; j++) {
			swprintf(((struct field_t**)menu->items)[i]->field_value, strlen(bencode->info->files[i]->path[j]) * sizeof(wchar_t), L"/%s", bencode->info->files[i]->path[j]);
		}
	}

	return 0;
}

int constructMetaInfo(struct menu_t *menu, struct field_t *template, struct bencode_module *bencode) {
	
	for (int i = 0; i < menu->size_y; i++) {
			
		int namelen = wcslen(template[i].field_name);		

		((struct field_t**)menu->items)[i] = (struct field_t*)malloc(sizeof(struct field_t*));
		((struct field_t**)menu->items)[i]->field_name = (wchar_t*)malloc(128 * sizeof(wchar_t));
		((struct field_t**)menu->items)[i]->field_value = (wchar_t*)malloc(128 * sizeof(wchar_t));
	
		swprintf(((struct field_t**)menu->items)[i]->field_name, namelen * sizeof(wchar_t), L"%ls", template[i].field_name);

		switch (i) {
			case 0:
				swprintf(((struct field_t**)menu->items)[i]->field_value, strlen(bencode->info->name) * sizeof(wchar_t), L"%s", bencode->info->name);		

				break;
			case 1:

				swprintf(((struct field_t**)menu->items)[i]->field_value, 128, L"%lld", bencode->creation_date);

				break;
			case 2:
				swprintf(((struct field_t**)menu->items)[i]->field_value, strlen(bencode->created_by) * sizeof(wchar_t), L"%s", bencode->created_by);
				
				break;
			case 3:
				swprintf(((struct field_t**)menu->items)[i]->field_value, strlen(bencode->comment) * sizeof(wchar_t), L"%s", bencode->comment);
			
				break;
	
			case 4:
				swprintf(((struct field_t**)menu->items)[i]->field_value, sizeof(long long int), L"%lld", bencode->info->piece_length);
		
				break;
		}

	}
	
	return 0;
}
