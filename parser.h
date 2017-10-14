#ifndef PARSER_H
#define PARSER_H


/********************************
 *     Macro Definitions        *
 ********************************/

#define MAX_LCOUNT 1024
#define MAX_LINESIZE 1024

#define AT_OK 1
#define AT_ERROR 2


/***************************************
 *       Type Definitions              *
 ***************************************/
typedef struct {
  
   int ok;                              /* Response "OK"/"ERROR". */  
   int line_count;                      /* Number of lines in response. */
   char data[MAX_LCOUNT][MAX_LINESIZE]; /* Response data. */
 
} at_response; /* Structure to keep response data. */

 
/****************************************
 *      Function Declarations           *
 ****************************************/
 int at_parse(char c);
 

#endif

