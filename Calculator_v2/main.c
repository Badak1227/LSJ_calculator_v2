#include <stdio.h>
#include "calc.h"

int main() {
	printf(">> ");
	while (input()) {
		printf("\n>> ");
	};
	
	printf("Exit\n");
	return 0;
}