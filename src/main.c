#include <stdio.h>
#include "./tokenizer.h"
#include "./calculator.h"
#include "graphing.h"
#include <stdlib.h>
#include <string.h>

int main(){

	Calculator *calc = new_calculator();

	while(1){
		printf("> ");
		char str[120];
		fgets(str, sizeof(str), stdin);  // Reads a line from stdin into str

		str[strlen(str)-1] = '\0';
		if(strcmp(str,"exit") == 0){
			break;
		}
		if(strcmp(str,"clear") == 0){
			system("clear");
			continue;
		}
	  if (strncmp(str, "plot", 4) == 0) {
			char fname[32];
			int x_min, x_max, y_min, y_max;

			// Parse: plot f -10 20 -10 20
			if (sscanf(str, "plot %s %d %d %d %d", fname, &x_min, &x_max, &y_min, &y_max) == 5) {
				// Find the function by name
				Function *target = NULL;
				for (int i = 0; i < calc->f_index; i++) {
					if (strcmp(calc->functions[i]->name, fname) == 0) {
						target = calc->functions[i];
						break;
					}
				}

				if (target == NULL) {
					printf("Function '%s' not found.\n", fname);
					continue;
				}

				calc->graph->x_min = x_min;
				calc->graph->x_max = x_max;
				calc->graph->y_min = y_min;
				calc->graph->y_max = y_max;

				system("clear");
				draw(calc, target);
				puts("");
			} else {
				printf("Usage: plot <function> <x_min> <x_max> <y_min> <y_max>\n");
			}

			continue;
		}

		printf("'%s'\n",str);
		double value = eval(calc,str);
		printf("%lf\n",value);
	}



	return 0;
}
