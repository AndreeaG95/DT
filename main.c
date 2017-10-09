#include <stdio.h>
#include "parser.h"


extern at_response response;

int main(void) {
	
	char test[] = "\r\nOK\r";
	
	int i = 0;
	int ret;
	while(!(ret = at_parse(test[i])))
	{
		printf("%d -- %d\n", i, ret);
		i++;
	}
	
	printf("%d -- %d\n", i, ret);
	
	
  return 0;
}
