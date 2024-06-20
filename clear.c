#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>


int main() {
    setlocale(LC_CTYPE, "");  
    
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int rows = w.ws_row;
    int columns = w.ws_col;

	wprintf(L"%lc", 0x250C);
	for (int i = 0; i < w.ws_col - 2; i++) {
		wprintf(L"%lc", 0x2500);	
	}
	wprintf(L"%lc\n", 0x2510);
	wprintf(L"%lc Announce List", 0x2502);


	fflush(stdout);


while (1) {}

 //   return 0;
}

