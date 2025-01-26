#include "mem.h"
void mem_init(){
    for(uint16_t i = 0; i < MEM_SIZE; i++){
        mem[i] = 0;
    }
}
void mem_write(uint16_t address, uint16_t value){
    mem[address] = value;
}
uint16_t mem_read(uint16_t address){
    return mem[address];
}