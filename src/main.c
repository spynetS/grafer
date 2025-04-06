#include <stdio.h>
#include "./posfix.h"

int main(){

	char str[120];

	fgets(str, sizeof(str), stdin);  // Reads a line from stdin into str

	// Remove newline character if present
	//str[strcspn(str, "\n")] = 0;
	printf("%s",str);


	double value = posfix_calculate(str);
	printf("%lf\n",value);

	return 0;
}
