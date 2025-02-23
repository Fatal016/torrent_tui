#ifndef H_DATA
#define H_DATA

#include "bencode.h"

int constructTorrentInfo(struct menu_t*, struct field_t*);
int constructTrackerInfo(struct menu_t*, struct bencode_module*);
int constructMetaInfo(struct menu_t*, struct field_t*, struct bencode_module*);
int constructFiles(struct menu_t*, struct bencode_module*);

#endif
