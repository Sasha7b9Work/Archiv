#pragma once

struct va_list
{
    char *a;
};

#define va_start(x, y) x.a = (char *)y; //-V1059

void va_end(va_list);
