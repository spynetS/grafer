#include "calculator.h"
#include "graphing.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>

#define BG_BLACK  "\x1b[40m"
#define BG_RED    "\x1b[41m"
#define BG_GREEN  "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE   "\x1b[44m"
#define BG_PURPLE "\x1b[45m"
#define BG_CYAN   "\x1b[46m"
#define BG_WHITE  "\x1b[47m"


#define BLACK  "\x1b[30m"
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define PURPLE "\x1b[35m"
#define CYAN   "\x1b[36m"
#define WHITE  "\x1b[37m"
#define BG     "██"

#define RESET "\x1b[0m"
#define BG_RESET "\x1b[10m"

#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"


int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}

void setCharAt(Graph *g, int x, int y, char *c) {
    setCursorPosition(x*3+abs(g->x_min)*3,  g->height-(y-g->y_min));
    printf("%s",c);
	fflush(stdout);
	//msleep(10);
}

void draw(Calculator *calc, Function *func)
{
	if (func == NULL){
		printf("NO FUNCTION PROVIDED\n");
		return;
	}
	Graph *g = calc->graph;
	//g->x_max = 10;

	// vertical
	for(int i = g->y_min; i < g->y_max; i ++){
		setCharAt(g,0,i,RESET"+");
		char str[10];
		sprintf(str,"%d",i);
		setCharAt(g,-1, i, str);
	}
	// horizontal
	for(int i = g->x_min; i < g->x_max; i ++){

		setCharAt(g,i, 0, "-");
		char str[10];
		sprintf(str,"%d",i);
		setCharAt(g,i, -1, str);
	}
	double prev = 0;
	// function
double step = 0.1;  // Smaller step for smoother curves

for(double x = g->x_min; x < g->x_max; x += step) {
    double y = call_function(calc, *func, x);

    if (y < g->y_min || y > g->y_max)
        continue;

    int screen_x = (int)(x); // Or apply scaling
    int screen_y = (int)(y);

    setCharAt(g, screen_x, screen_y, BLUE"*"RESET);
}


	setCharAt(g,g->width, g->y_min-2, "");
}
