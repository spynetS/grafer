#include "calculator.h"
#include "graphing.h"

#include <stdio.h>
#include <stdlib.h>


void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y+1, x+1);
}

void setCharAt(int x, int y, char *c) {
    setCursorPosition(x, y);
    printf("%s",c);
}

void draw(Calculator *calc, Function *func){
	system("clear");
	Graph *g = calc->graph;
	int middle = -g->x_min;
	// horizontal
	for (int i = g->x_min; i < g->width; i++) {
		setCharAt(i*2 ,g->height,"- ");
		if(i % 2 == 0)
		{
			char str[10];
			sprintf(str,"%d ",i);
			setCharAt(i*2,g->height+1,str);
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

	for (int i = 0; i < g->width; i++) {
		int x = g->x_min + i;
		double value = call_function(calc,*func,x);
		char str[10];
		sprintf(str,"%lf",value);


		setCharAt(3+i*2, g->height-value,"*");
	}

	setCharAt(g->width+5,g->height+5,"0");
}
