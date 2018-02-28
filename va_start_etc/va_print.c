#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>


#define DEBUG(...)  printf(__VA_ARGS__)

void va_print(const char *fmt, ...)
{
    va_list    ap;

    va_start(ap, fmt);

    char    buf[64];
    vsnprintf(buf, 64, fmt, ap);
    fputs(buf, stderr);

    int p_str = va_arg( ap, int );
    printf("%s\n", (char *)p_str);

    int v_int = va_arg( ap, int );
    printf("%d\n", v_int);

    int v_char = va_arg( ap, int );
    printf("%c\n", v_char);

    double v_double = va_arg( ap, double );
    printf("%f\n", v_double);

    va_end(ap);
}

void main( void )
{
    va_print("\nParameters: \n\n", "hello world", 123, 'c', 15.60);

    DEBUG("abc:%d, %s, %c, %f\n", 12, "yuriping", 'c', 12.86);
}
