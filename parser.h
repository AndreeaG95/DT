#ifndef PARSER_H
#define PARSER_H


#define MAX_LCOUNT 1024
#define MAX_LINESIZE 1024
#define AT_OK 1
#define AT_ERROR 2

// Definitions for the parser.
 typedef struct {
	 int ok;
	 int line_count;
	 char data[MAX_LCOUNT][MAX_LINESIZE];
 } at_response;
 
 
 int at_parse(char c);
 


#endif

