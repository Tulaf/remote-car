#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

#define LOG_LEVEL INFO // ���嵱ǰ����־����


void log_message(int level, const char *format, ...)
{
    if (level < LOG_LEVEL) {
        return; // �����Ϣ������ڵ�ǰ���õ���־�����򲻴�ӡ
    }

    va_list args;
    va_start(args, format);

    switch (level) {
        case DEBUG:
            printf("DEBUG: ");
            break;
        case INFO:
            printf("INFO: ");
            break;
        case WARNING:
            printf("WARNING: ");
            break;
        case ERROR:
            printf("ERROR: ");
            break;
        default:
            printf("UNKNOWN LEVEL: ");
            break;
    }

    vprintf(format, args);
    va_end(args);
}
