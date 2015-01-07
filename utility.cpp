#include "utility.h"
#include <iostream>
#include <string.h>

namespace utility
{
	char* itoa(int val, char* c){
		int i = 0;
		do{
			c[i] = val % 10 + '0';
			val /= 10;
			i++;
		} while (val);
		c[i] = '\0';
		return reverse(c);
	}

	char* reverse(char* c){
		char tmp;
		int size = strlen(c);
		for (int i = 0; i < size / 2; i++){
			tmp = c[i];
			c[i] = c[size - 1 - i];
			c[size - 1 - i] = tmp;
		}
		return c;
	}
}