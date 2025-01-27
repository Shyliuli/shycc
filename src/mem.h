#ifndef MEM_H
#define MEM_H
#define MEM_SIZE 0xFFFF
#include <stdint.h>
uint16_t mem[MEM_SIZE]; // 1M "16-bit" memory
void mem_init();
void mem_write(uint16_t addr, uint16_t val);
uint16_t mem_read(uint16_t addr);
#endif // MEM_H