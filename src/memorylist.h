
#ifndef MEMORY_LIST_H
#define MEMORY_LIST_H
//头文件保护
#include<vector>
#include<iostream>
#define MAX 0xFFFF
#define CODE_FROM 0x2000
// 内建函数编号从0x10开始
#define ADDA_FUNC 0x10
#define ADDN_FUNC 0x11
#define SUBA_FUNC 0x12
#define SUBN_FUNC 0x13
#define MULA_FUNC 0x14
#define MULN_FUNC 0x15
#define DIVA_FUNC 0x16
#define DIVN_FUNC 0x17
#define LSA_FUNC 0x18
#define LSN_FUNC 0x19
#define LRA_FUNC 0x1A
#define LRN_FUNC 0x1B
#define ANDA_FUNC 0x1C
#define ANDN_FUNC 0x1D
#define ORA_FUNC 0x1E
#define ORN_FUNC 0x1F
#define NOTA_FUNC 0x20
#define NOTN_FUNC    0x21
#define XORA_FUNC    0x22
#define XORN_FUNC    0x23
#define JMPA_FUNC    0x24
#define JMPN_FUNC    0x25 
#define EQUA_FUNC    0x26
#define EQUN_FUNC    0x27
#define BIGA_FUNC    0x28
#define BIGN_FUNC    0x29
#define BIGEQUA_FUNC 0x2A
#define BIGEQUN_FUNC 0x2B
#define SMAA_FUNC    0x2C
#define SMAN_FUNC    0x2D
#define SMAEQUA_FUNC 0x2E
#define SMAEQUN_FUNC 0x2F
#define SETA_FUNC     0x30
#define SETN_FUNC     0x31
#define INA_FUNC    0x32 
#define INN_FUNC    0x33  
#define OUTA_FUNC    0x34 
#define OUTN_FUNC    0x35 //命令对应
#define OUTAASC_FUNC    0x36 
#define OUTNASC_FUNC    0x37 
#define ax 0x01
#define bx 0x02
#define cx 0x03
#define dx 0x04
#define ex 0x05
#define fx 0x06
#define gx 0x07
#define hx 0x08
#define ix 0x09
#define jx 0x0A
#define kx 0x0B
#define lx 0x0C
#define mx 0x0D
#define pc address[0x0D]//mx是Program Counter，程序计数器
#define nx 0x0E
#define ox 0x0F//寄存器对应
#define tmp address[0x0F]//ox存储临时结果
std::vector<unsigned short> address(65535,0);


/*
0x00-0x0F:寄存器
ax: 0x01
bx: 0x02
cx: 0x03
dx: 0x04
ex: 0x05
fx: 0x06
gx: 0x07
hx: 0x08
ix: 0x09
jx: 0x0A
kx: 0x0B
lx: 0x0C
mx: 0x0D// pc,保存运行到哪里的
nx: 0x0E// 改变此寄存器则程序退出
ox: 0x0F// 保存equ等指令的判断结果

0x10-0x4F:保留空间（用于命令等）
0x50-0xFFFF: 内存，其中
0x36-0x1FF: 变量存储
0x2000-0xFFFF: 程序
*/


#endif // MEMORY_LIST_H