#ifndef RTT_LOG_H__
#define RTT_LOG_H__

#ifdef RTT_LOG_ENABLED

#include"segger_rtt.h"  // Included for colors

// Function Declarations
void rtt_log_general_print(int terminal, const char* color, const char *file, int line, const char *sFormat, ...);

// Convenience Macros
// #define log_init() rtt_log_init()

// Log ERROR (Red / Terminal 0)
#define loge(...)	rtt_log_general_print(0,RTT_CTRL_TEXT_BRIGHT_RED,__FILE__,__LINE__,__VA_ARGS__)
#define logw(...)	rtt_log_general_print(0,RTT_CTRL_TEXT_BRIGHT_YELLOW,__FILE__,__LINE__,__VA_ARGS__)
#define logi(...)	rtt_log_general_print(0,RTT_CTRL_TEXT_WHITE,__FILE__,__LINE__,__VA_ARGS__)

#else

#define loge(...) 
#define logw(...)
#define logi(...)
#define log_init() 

#endif // RTT_LOG_ENABLED

#endif // RTT_LOG_H__
