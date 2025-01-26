#include "io.h"

int main(int argc,char* args[]){
    if(argc >= 2){
        if(strcmp(args[1],"-h") == 0){
            print_help();
            return 0;
        }
        else if(strcmp(args[1],"-v") == 0){
            print_version();
            return 0;
        }
        else
            load_program_2_mem(args[1]);
    }
    trans_2_sfs();
    output();
    return 0;
}