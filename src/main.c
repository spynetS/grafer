#include <stdio.h>
#include "./posfix.h"
#include "./turing.h"
#include <string.h>

int main(){

	Calculator calc;
	calc.v_index = 0;
	calc.f_index = 0;

	while(1){
		printf("> ");
		char str[120];
		fgets(str, sizeof(str), stdin);  // Reads a line from stdin into str

		str[strlen(str)-1] = '\0';
		if(strcmp(str,"exit") == 0){
			break;
		}
		printf("'%s'\n",str);

		double value = eval(&calc,str);

		printf("%lf\n",value);
	}



	return 0;
}
