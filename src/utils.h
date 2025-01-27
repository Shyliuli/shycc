#ifndef UTILS_H
#define UTILS_H
#include <string.h>
#include<ctype.h>
#include <stdbool.h>
#define str_t short
str_t type_is(char* strval);
enum str_type{
    NUM,
    LABEL,
    HEX,
    STR,
    COMMAND,
    REG,
    DATASTART,
    DATAEND,
    CODESTART,
    CODEEND,
};

#endif //UTILS_H