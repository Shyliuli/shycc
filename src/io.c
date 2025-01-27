#include "io.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 新增内存释放函数
static void free_line(char **line) {
    if (line) {
        for (int i = 0; i < 3; i++) {
            if (line[i]) free(line[i]);
        }
        free(line);
    }
}

void print_help() {
    printf("shycc [option] [file]\n");
    printf("option:\n");
    printf("\t-h\tprint help\n");
    printf("\t-v\tprint version\n");
}

void print_version() {
    printf("shycc version %s\n", VERSION);
}

// 优化后的行读取函数（保持接口不变）
char** read_a_line(FILE *fp, uint16_t line_num) {
    static long file_pos = 0;  // 使用静态变量记录读取位置
    if (!fp || line_num == 0) return NULL;

    fseek(fp, file_pos, SEEK_SET);  // 从上一次结束位置继续读取

    char *buffer = NULL;
    size_t buf_size = 128;
    size_t len = 0;
    int ch;

    // 动态分配缓冲区
    if (!(buffer = malloc(buf_size))) return NULL;

    // 读取行内容
    while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != '\r') {
        if (len + 1 >= buf_size) {
            buf_size *= 2;
            if (!(buffer = realloc(buffer, buf_size))) {
                free(buffer);
                return NULL;
            }
        }
        buffer[len++] = (char)ch;
    }

    // 记录文件位置供下次读取使用
    file_pos = ftell(fp);

    // 处理Windows换行
    if (ch == '\r') {
        if ((ch = fgetc(fp)) != '\n') {
            ungetc(ch, fp);
            file_pos--;  // 回退位置
        }
    }

    if (len == 0 && ch == EOF) {  // 文件结束
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';

    // 分割行内容（优化内存分配）
    char **result = calloc(3, sizeof(char*));  // 预分配3个指针
    if (!result) {
        free(buffer);
        return NULL;
    }

    char *token = strtok(buffer, " \t");
    for (int i = 0; i < 3 && token; i++) {
        result[i] = strdup(token);
        if (!result[i]) {
            free_line(result);
            free(buffer);
            return NULL;
        }
        token = strtok(NULL, " \t");
    }

    free(buffer);
    return result;
}

// 参数处理统一函数
static void process_argument(char *arg, uint16_t pc, int offset) {
    if (!arg) return;

    uint16_t val = 0;
    switch (type_is(arg)) {
    case LABEL:
        val = label_2_addr(arg);
        break;
    case NUM:
        sscanf(arg, "%hd", &val);
        break;
    case REG:
        val = reg_2_addr(arg);
        break;
    case HEX:
        sscanf(arg, "%hx", &val);
        break;
    default:
        fprintf(stderr, "Invalid argument type: %s\n", arg);
        exit(EXIT_FAILURE);
    }
    mem_write(pc + PROGRAM_START + offset, val);
}

void load_data(FILE *fp) {
    int line = 1;
    char **aline = NULL;

    // 查找数据段开始
    while ((aline = read_a_line(fp, line++))) {
        if (type_is(aline[0]) == DATASTART) {
            free_line(aline);
            break;
        }
        free_line(aline);
    }

    // 处理数据内容
    while ((aline = read_a_line(fp, line++))) {
        if (type_is(aline[0]) == DATAEND) {
            free_line(aline);
            break;
        }

        if (!aline[0] || !aline[1]) {
            fprintf(stderr, "Invalid data format at line %d\n", line-1);
            exit(EXIT_FAILURE);
        }

        uint16_t addr = 0;
        sscanf(aline[0], "%hx", &addr);

        switch (type_is(aline[1])) {
        case NUM:
        case HEX: {
            uint16_t val = 0;
            sscanf(aline[1], type_is(aline[1]) == NUM ? "%hd" : "%hx", &val);
            mem_write(addr, val);
            break;
        }
        default: {
            size_t len = strlen(aline[1]);
            for (size_t i = 0; i < len; i++) {
                mem_write(addr + i, aline[1][i]);
            }
            break;
        }
        }
        free_line(aline);
    }
}

void load_command(FILE *fp) {
    uint16_t line = 1;
    uint16_t pc = 0;
    char **aline = NULL;

    // 查找代码段开始
    while ((aline = read_a_line(fp, line++))) {
        if (type_is(aline[0]) == CODESTART) {
            free_line(aline);
            break;
        }
        free_line(aline);
    }

    // 处理指令
    while ((aline = read_a_line(fp, line++))) {
        if (type_is(aline[0]) == CODEEND) {
            free_line(aline);
            break;
        }

        if (type_is(aline[0]) == LABEL) {
            label_add(aline[0], pc);
            free_line(aline);
            continue;
        }

        if (type_is(aline[0]) == COMMAND) {
            mem_write(pc + PROGRAM_START, command_2_addr(aline[0]));
            process_argument(aline[1], pc, 1);
            process_argument(aline[2], pc, 2);
        }

        pc += 3;
        free_line(aline);
    }
}

void load_program_2_mem(char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    load_data(fp);
    fseek(fp, 0, SEEK_SET);  // 重置文件指针
    load_command(fp);
    
    fclose(fp);
}