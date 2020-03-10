#include "Printer.h"
#include "ITM_write.h"
#include <stdio.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */


Printer::Printer()
{
    ITM_init();
}

void Printer::print(const char *format, ...)
{
    char buffer [256];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf (buffer, 256, format, argptr);
    va_end(argptr);
    ITM_write(buffer);
}
