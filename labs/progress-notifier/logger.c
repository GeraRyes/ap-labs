#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void changeColor(int attr, int fg, int bg);
void defaultColor();
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

void changeColor(int attr, int fg, int bg)
{
    char command[13];
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

void defaultColor() {
    printf("%c[0m", 0x1B);
}

int infof(const char *format, ...)
{
    changeColor(0, 4, 0);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    defaultColor();
    return 0;
}

int warnf(const char *format, ...)
{
    changeColor(0, 6, 0);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    defaultColor();
    return 0;
}

int errorf(const char *format, ...)
{
    changeColor(0, 1, 0);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    defaultColor();
    return 0;
}

int panicf(const char *format, ...)
{
    changeColor(0, 2, 0);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    defaultColor();
    return 0;
} 