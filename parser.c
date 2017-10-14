/*
 *  parser is a library for extracting and parsing AT command responses. 
 */
 
#include <stdio.h>
#include <ctype.h>
#include "parser.h"


/********************************
 *     Macro Definitions        *
 ********************************/

/* Macro to reset from a syntax error. */
#define RESET {printf("Syntax error in state %d\n", state); state = 0; return -1;}


/********************************
 *      Global Variables        *
 ********************************/

int state;            /* State of state machine. */
int line;             /* Current line count. */
int ind;              /* Current index on line. */
at_response response; /* The response data. */


/*******************************
 *    Function Definitions     *
 *******************************/

/******************************************************
 * Function to parse responses to AT commands.
 * Input: next char in the response.
 * Output: - returns -1 and resets state on a syntax error
 *         - returns  0 if syntax is respected so far
 *         - returns AT_OK if end of response reached succesfully and response is "OK"
 *         - returns AT_ERROR if end of response reached succesfully and response is "ERROR" 
 *****************************************************/
int at_parse(char c) 
{
  
  switch(state) 
    {
    case 0:   /* Initial state. Reset everything. */
      response.line_count = 0;
      response.ok = -1;
      line = 0;
      ind = 0;
      if(c == '\r') /* Expected CR. */
	state = 1;
      else 
	RESET
	  break;
      
    case 1: /* Read a CR, expect LF. */
      if (c == '\n')
	state = 2;
      else 
	RESET
	  break;
      
    case 2: /* Read CRLF prologue. Expect '+' or epilogue. */
      if(c == '+')
	state = 3;
      else if (c == 'O')
	state = 8;
      else if (c == 'E')
	state = 12;
      else
	RESET
	  break;
      
    case 3: /* Got a '+' ==> response data. */
      if (isprint(c)) /* Expect at least a printable character. */
	{
	  state = 18;
	  if(ind < MAX_LINESIZE - 1 && line < MAX_LCOUNT) /* Not to overflow buffer */
	    response.data[line][ind++] = c;
	}
      else
	RESET
	  break;
      
    case 18: /* Got at least one printable character, read the rest of the line. */
      if (c == '\r') /* Line ending. */
	{
	  state = 4;
	  if(line < MAX_LCOUNT)
	    response.data[line][ind++] = '\0';
	  ind = 0;
	  line++;
	}
      else if(isprint(c)) /* Keep reading until '/r'. */
	{
	  if(ind < MAX_LINESIZE - 1 && line < MAX_LCOUNT) /* Not to overflow buffer */
	    response.data[line][ind++] = c;
	}
      else 
	RESET
	  break;
      
    case 4: /* Data line ending expected. */
      if (c == '\n')
	{
	  state = 5;
	}
      else
	RESET
	  break;
      
    case 5: /* New data line or response epilogue expected. */
      if (c == '\r') /* Read epilogue. */
	state = 6;
      else if(c == '+') /* Read another data line. */
	{
	  state = 3;
	}
      else
	RESET
	  break;
      
    case 6: /* LF before response value expected. */
      if (c == '\n')
	state = 7;
      else
	RESET
	  break;
      
    case 7: /* Response value expected, either "OK" or "ERROR". */
      if (c == 'O')
	state = 8; /* OK */
      else if ( c == 'E')
	state = 12; /* ERROR */
      else
	RESET
	  break;
      
    case 8: /* OK */
      if (c == 'K')
	state = 9;
      else 
	RESET
	  break;
      
    case 9: /* Read "OK", expect final CRLF. */
      if (c == '\r')
	state = 10;
      else
	RESET
	  break;
      
    case 10: /* Final LF expected. */
      if (c == '\n')
	{	
	  state = 0;
	  response.line_count = line <= MAX_LCOUNT ? line : MAX_LCOUNT;
	  response.ok = AT_OK;
	  return AT_OK;
	}
      else 
	RESET
	  break;
      
    case 12: /* ERROR */
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
      
    case 16: /* Read "ERROR", expect final CRLF. */
      if (c == '\r')
	state = 17;
      else
	RESET
	  break;
      
    case 17: /* Final LF expected. */
      if (c == '\n')
	{	
	  state = 0;
	  response.line_count = line <= MAX_LCOUNT ? line : MAX_LCOUNT;
	  response.ok = AT_ERROR;
	  return AT_ERROR;
	}
      else 
	RESET
	  break;
      
      
    default: /* Illegal state, should never reach here. */
      fputs("Illegal state reached.\n", stderr);
      RESET
    }
  
  return 0;   /* Return everything is fine so far. */
}
