#ifndef __DEBUG_H
#define __DEBUG_H

#define DEBUG 1
#define INFO 2
#define WARNING 3
#define ERROR 4

void log_message(int level, const char *format, ...);

#endif
