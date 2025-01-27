#include "reg.h"
uint16_t find_addr_by_reg(const char* name){
    for (int i = 0; i < REGISTER_COUNT; i++) {
        if (strcmp(register_names[i], name) == 0) {
            return i;
        }
    }
    return 0xFF; // 未找到寄存器
}

// 根据寄存器索引查找名称
const char* find_reg_by_addr(int index){
    if (index >= 0 && index < REGISTER_COUNT) {
        return register_names[index];
    }
    return NULL; // 无效索引
}