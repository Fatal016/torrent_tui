#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	for (int i = 0; i < w.ws_row; i++) {
		for (int j = 0; j < w.ws_col; j++) {
			printf("%s", "\x1b[31mtest\033[0ming");
		}
	}

    printf("Terminal size: %d columns, %d rows\n", w.ws_col, w.ws_row);
    return 0;
}
