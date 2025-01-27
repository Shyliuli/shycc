#include "command.h"
const char* addr_2_command(uint8_t hex_address) {
    for (int i = 0; i < COMMAND_TABLE_SIZE; i++) {
        if (command_table[i].hex_address == hex_address) {
            return command_table[i].command_name;
        }
    }
    return NULL; // 未找到对应的命令
}

// 通过命令名称查找地址
uint16_t command_2_addr(const char* command_name) {
    for (int i = 0; i < COMMAND_TABLE_SIZE; i++) {
        if (strcmp(command_table[i].command_name, command_name) == 0) {
            return command_table[i].hex_address;
        }
    }
    return 0xFF; // 未找到对应的地址，返回一个无效值
}