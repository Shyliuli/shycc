#ifndef REG_H
#define REG_H
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// 寄存器数量
#define REGISTER_COUNT 16

// 寄存器名称
const char* register_names[REGISTER_COUNT] = {
    "ax", "bx", "cx", "dx", "ex", "fx", "gx", "hx",
    "ix", "jx", "kx", "lx", "mx", "nx", "ox"
};

// 根据寄存器名称查找索引
uint16_t reg_2_addr(const char* name);

// 根据寄存器索引查找名称
const char* addr_2_reg(int index);
#endif