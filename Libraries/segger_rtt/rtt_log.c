#ifdef RTT_LOG_ENABLED

#include "rtt_log.h"
#include<stdarg.h>  // VA Lists

// Forward declartion of Segger vprintf.
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

#define LOG_BUFF 0
#define BUFF_SIZE 100
//#define NULL 0

// Spaces and length for maximum file names
#define MAX_FILE_LEN 25 

static char buffer[BUFF_SIZE]; // The buffer

void rtt_log_general_print(int terminal, const char* color, const char *file, int line, const char * sFormat, ...) {
  static int initialized = 0;
                             //01234567890123456789012345
  static const char *spaces = "                          ";
	if(!initialized) {
  	SEGGER_RTT_ConfigUpBuffer(LOG_BUFF, 0, buffer, BUFF_SIZE, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
		initialized = 1;
	}

	// Set Terminal
	SEGGER_RTT_SetTerminal(terminal);

	// Set Color
	SEGGER_RTT_WriteString(LOG_BUFF, color);
	
	// Print file name
	const char *lastSlash = file;
	const char *current = file;
	while(*current!=0) {
		if(*current == '\\' || *current == '/') 
			lastSlash = current+1;
		current++;
	}
	// Print name/line number
        SEGGER_RTT_printf(LOG_BUFF, "%s:%d",lastSlash,line);	
	
	int nameLen = (current-lastSlash)+1+(line>999?4:((line>99)?3:((line>9)?2:1)));
	
	// Print justification spacing	
	if(nameLen<MAX_FILE_LEN) 
		SEGGER_RTT_WriteString(LOG_BUFF,spaces+nameLen);

	// Print actual error message stuff
        va_list ParamList;
        va_start(ParamList, sFormat);
	SEGGER_RTT_vprintf(LOG_BUFF, sFormat, &ParamList);

       SEGGER_RTT_WriteString(LOG_BUFF, "\r\n");
}
#endif
