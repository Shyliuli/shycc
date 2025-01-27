#include "utils.h"
#include "command.h"
#include "reg.h"
str_t type_is(char *strval){
    if(strchr(strval, ':') != NULL){
        return LABEL;
    }
    if(strval[0] != NULL && strval[1] != NULL){
        if(strval[0] == '0' && strval[1] == 'x'|| strval[0] == 'X'){
            return HEX;
        }        
    }
    if(isdigit(strval[0])){
        return NUM;
    }
    if(find_address_by_command(strval) != 0xFF){
        return COMMAND;
    }
    if(find_addr_by_reg(strval) != 0xFF){
        return REG;
    }

    if(strval == NULL){
        printf("Error: NULL value\n");
        assert(0);
    }
    if(strcmp(strval, "_datastart_") == 0){
        return DATASTART;
    }
    if(strcmp(strval, "_dataend_") == 0){
        return DATAEND;
    }
    if(strcmp(strval, "_codestart_") == 0){
        return CODESTART;
    }
    if(strcmp(strval, "_codeend_") == 0){
        return CODEEND;
    }
    return STR; // default


}