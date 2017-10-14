#include <stdio.h>
#include "parser.h"
#include<stdlib.h>

#define BUFSIZE 1024

extern at_response response;

int main(int argc, char *argv[]) {
	
  if (argc < 2)
    {
      printf("Usage %s <test_file>\n", argv[0]);
      exit(1);
    }

  FILE *f = fopen(argv[1], "rb");
  if(!f)
    {
      fputs("Error opening file.\n", stderr);
      exit(1);
    }

  char buf[BUFSIZE];

  int chread = 0;
  int syn_error = 0;

  /* Entire file must be valid to print data.
     If valid response is followed by something else ==> data won't be printed. */
  while((chread = fread(buf, 1, BUFSIZE, f)))
    {
      int i;
      for(i = 0; i < chread; i++)
	{
	  if (at_parse(buf[i]) == -1) //End on syntax error on this unit test
	    {
	      syn_error = 1;
	      break;
	    }
	}
      if(syn_error)
	break;
    }

  if(!syn_error)
    {
      puts(response.ok == AT_OK ? "AT_OK" : "AT_ERROR");
      int i;
      for(i = 0; i < response.line_count; i++)
	puts(response.data[i]);
    }
  else
    {
      printf("Syntax error.\n");
      exit(1);
    }

  if(fclose(f))
    {
      fputs("Error closing file.\n", stderr);
      exit(1);
    }

  return 0;
}
