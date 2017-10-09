/*
 *  parser is a library for extracting and parsing AT command responses. 
 */
 
#include <stdio.h>
#include "parser.h"

int state;
int line;
int index;
at_response response;

#define RESET {state = 0; return -1;}

int at_parse(char c) 
{
		
	switch(state) 
	{
		case 0:
		response.line_count = 0;
		response.ok = 0;
		line = 0;
		index = 0;
		if(c == '\r')
			state = 1;
		else 
			RESET
		break;
		
		case 1:
		if (c == '\n')
			state = 2;
		else 
			RESET
		break;
		
		case 2:
		if(c == '+')
			state = 3;
		else if (c == 'O')
			state = 8;
		else if (c == 'E')
			state = 12;
		else
			RESET
		break;
		
		case 3:
		if (isprint(c))
		{
			state = 20;
			response.data[line][index] = c;
			index++;
		}
		else
			RESET
		break;
		
		case 20:
		if (c == '\r')
		{
			state = 4;
			response.data[line][index++] = '\0';
		}
		else if(isprint(c))
		{
			response.data[line][index++] = c;
		}
		else 
			RESET
		break;
		
		case 4:
		if (c == '\n')
			state = 5;
		else
			RESET
		break;
		
		case 5:
		if (c == '\r')
			state = 6;
		else if(c == '+')
		{
			line++;
			state = 3;
		}
		else
			RESET
		break;
		
		case 6:
		if (c == '\n')
			state = 7;
		else
			RESET
		break;
		
		case 7:
		if (c == 'O')
			state = 8;
		else if ( c == 'E')
			state = 12;
		else
			RESET
		break;
		
		case 8:
		if (c == 'K')
			state = 9;
		else 
			RESET
		break;
		
		case 9:
		if (c == '\r')
			state = 10;
		else
			RESET
		break;
		
		case 10:
		if (c == '\n')
		{	
			state = 0;
			response.line_count = line;
			return AT_OK;
		}
		else 
			RESET
		break;
		
		case 12:
		if (c == 'R')
			state = 13;
		else
			RESET
		break;
		
		case 13:
		if (c == 'R')
			state = 14;
		else
			RESET
		break;
		
		case 14:
		if (c == 'O')
			state = 15;
		else
			RESET
		break;

		case 15:
		if (c == 'R')
			state = 16;
		else
			RESET
		break;
		
		case 16:
		if (c == '\r')
			state = 17;
		else
			RESET
		break;
		
		case 17:
		if (c == '\n')
		{	
			state = 0;
			response.line_count = line;
			return AT_ERROR;
		}
		else 
			RESET
		break;
		
		
		default:
		printf("contrabandistule\n");
		return -1;		
	}
	
	
	
  return 0;
}
