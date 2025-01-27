#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 定义命令结构体
typedef struct {
    uint8_t hex_address;
    char command_name[16];
} Command;

// 定义命令表
Command command_table[] = {
    {0x10, "adda"},
    {0x11, "addn"},
    {0x12, "suba"},
    {0x13, "subn"},
    {0x14, "mula"},
    {0x15, "muln"},
    {0x16, "diva"},
    {0x17, "divn"},
    {0x18, "lsa"},
    {0x19, "lsn"},
    {0x1A, "rsa"},
    {0x1B, "rsn"},
    {0x1C, "anda"},
    {0x1D, "andn"},
    {0x1E, "ora"},
    {0x1F, "orn"},
    {0x20, "nota"},
    {0x21, "notn"},
    {0x22, "xora"},
    {0x23, "xorn"},
    {0x24, "jmpa"},
    {0x25, "jmpn"},
    {0x26, "equa"},
    {0x27, "equn"},
    {0x28, "biga"},
    {0x29, "bign"},
    {0x2A, "bigequa"},
    {0x2B, "bigequn"},
    {0x2C, "smaa"},
    {0x2D, "sman"},
    {0x2E, "smaequa"},
    {0x2F, "smaequn"},
    {0x30, "seta"},
    {0x31, "setn"},
    {0x32, "ina"},
    {0x33, "inn"},
    {0x34, "outa"},
    {0x35, "outn"},
    {0x36, "outaasc"},
    {0x37, "outnasc"}
};

// 命令表的大小
#define COMMAND_TABLE_SIZE (sizeof(command_table) / sizeof(Command))

// 通过地址查找命令名称
const char* find_command_by_address(uint8_t hex_address);

// 通过命令名称查找地址
uint16_t find_address_by_command(const char* command_name);

#endif