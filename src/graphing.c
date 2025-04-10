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

void setCharAt(int x, int y, char *c) {
    setCursorPosition(x, y);
    printf("%s",c);
				fflush(stdout);
}

void draw(Calculator *calc, Function *func){

	Graph *g = calc->graph;
	int middle = -g->x_min;
	// horizontal
	for (int i = g->x_min; i < g->x_max; i++) {
		setCharAt(3+i*2 - g->x_min*2 ,g->height,"- ");
		if(i % 2 == 0)
		{
			char str[10];
			sprintf(str,"%d ",i);
			setCharAt(3+i*2 - g->x_min*2,g->height+1,str);


		}
	}
	// vertical
	for (int i = g->height; i > 0; i--) {
		
		setCharAt(3+middle*2,i,"- ");
		if(i % 2 == 0)
		{
			char str[10];
			sprintf(str,"%d ",i);
			setCharAt(middle*2,g->height-i,str);
		}
	}
	//double prev = call_function(calc,*func,g->x_min+10);
	double prev = 0;
	for (int i = g->x_min; i < g->x_max; i++) {

		double value = call_function(calc,*func,i);
		if(value > g->y_max) continue;
		char str[100];

		int min = value-prev < 0;
		for(int j = 0;prev != 0 && j < ((int) abs(value-prev)) - min*2; j++){

			/* sprintf(str,"%lf - %lf = %lf, %d %lf (%lf, %lf)",value,prev,value-prev,i,value, 3+i*2-g->x_min*2,g->height-value+j); */
			/* setCharAt(50, 10,str); */
			//setCharAt(3+i*2 - g->x_min * 2, g->height-value+j,"|");
//			msleep(5);
		}

		prev = value;
		setCharAt(3+i*2 - g->x_min * 2, g->height-value,"*");
	}

	setCharAt(g->width+5,g->height+5,"0");
}
