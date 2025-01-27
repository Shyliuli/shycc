#include "io.h"
void print_help()
{
    //shycc xxx,xxx可以是option(-h,-v)也可以是文件名(执行编译的shy源代码)
    printf("shycc [option] [file]\n");
    printf("option:\n");
    printf("\t-h\tprint help\n");
    printf("\t-v\tprint version\n");

}
// 定义一个名为 print_version 的函数，用于打印程序版本信息
void print_version()
{
    printf("shycc version %s\n", VERSION);
}
// 定义一个函数，用于将程序加载到内存中
void load_program_2_mem(char* filename)
{
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Unable to open file %s\n", filename);
    exit(1);
  }
  load_data(fp);
  load_command(fp);
  fclose(fp);
}

// 读取文件流的某一行内容
void load_data(FILE *fp)
{
    int line = 1;
    while(1) {
        char ** aline= read_a_line(fp, line);
        line++;
        if(aline == NULL || aline[0]==NULL){
            printf("Error: Unable to read line %d\n", line);
            assert(0);
        }
        if(type_is(aline[0])==DATASTART){
            break;
        }
    }//找到数据开始
    while(1) {
        char ** aline= read_a_line(fp, line);
        line++;
        if(aline == NULL || aline[0]==NULL || aline[1]==NULL){
            printf("Error: Unable to read line %d\n", line);
            assert(0);
        }
        if(type_is(aline[0])==DATAEND){
            break;
        }
        uint16_t addr=0;
        sscanf(aline[0], "%x", &addr);
        uint16_t val=0;
        switch (type_is(aline[1]))
        {
        case NUM:
            sscanf(aline[1], "%d", &val);
            mem_write(addr, val);
            break;
        case HEX:
            sscanf(aline[1], "%x", &val);
            mem_write(addr, val);
            break;
        default:
            int len =strlen(aline[1]);
            for(int i=0; i<len; i++){
                mem_write(addr+i, aline[1][i]);
            }
            break;
        }

    }
}
void load_command(FILE *fp)
{
    uint16_t line = 1;
    uint16_t pc = 0;
    while(1) {
        char ** aline= read_a_line(fp, line);
        line++;
        if(aline == NULL || aline[0]==NULL){
            printf("Error: Unable to read line %d\n", line);
            assert(0);
        }
        if(type_is(aline[0])==CODESTART){
            break;
        }
    }
    while(1) {
        char ** aline= read_a_line(fp, line);
        line++;
        if(aline == NULL || aline[0]==NULL){
            printf("Error: Unable to read line %d\n", line);
            assert(0);
        }
        if(type_is(aline[0])==CODEEND){
            break;
        }
        if(type_is(aline[0])==LABEL){
            label_add(aline[0], pc);
            continue;
        }
        if(type_is(aline[0])==COMMAND){
            mem_write(pc+PROGRAM_START, command_2_addr(aline[0]));
          if(aline[1]!=NULL){
                if(type_is(aline[1])==LABEL){
                    mem_write(pc+PROGRAM_START+1, label_2_addr(aline[1]));
                }
                if(type_is(aline[1])==NUM){
                    int num;
                    sscanf(aline[1], "%d", &num);
                    mem_write(pc+PROGRAM_START+1, num);
                }
                if(type_is(aline[1])==REG){
                    mem_write(pc+PROGRAM_START+1, reg_2_addr(aline[1]));
                }
                if(type_is(aline[1])==HEX){
                    int num;
                    sscanf(aline[1], "%x", &num);
                    mem_write(pc+PROGRAM_START+1, num);
                }
                if(aline[2]!=NULL){
                    if(type_is(aline[2])==LABEL){
                        mem_write(pc+PROGRAM_START+2, label_2_addr(aline[2]));
                    }
                    if(type_is(aline[2])==NUM){
                        int num;
                        sscanf(aline[2], "%d", &num);
                        mem_write(pc+PROGRAM_START+2, num);
                    }
                    if(type_is(aline[2])==REG){
                        mem_write(pc+PROGRAM_START+2, reg_2_addr(aline[2]));
                    }
                    if(type_is(aline[2])==HEX){
                        int num;
                        sscanf(aline[2], "%x", &num);
                        mem_write(pc+PROGRAM_START+2, num);
                    }
                }
                else{
                    mem_write(pc+PROGRAM_START+2, 0);
                }
          }
          else{
            mem_write(pc+PROGRAM_START+1, 0);
            mem_write(pc+PROGRAM_START+2, 0);
          }  
        }

    pc+=3;
    }

}
char** read_a_line(FILE *fp, uint16_t line) {
    if (line == 0 || !fp) return NULL;

    long start_pos = ftell(fp);
    rewind(fp);

    char *buffer = NULL;
    size_t buf_size = 128;
    uint16_t current_line = 0;
    int ch;

    buffer = malloc(buf_size);
    if (!buffer) return NULL;

    // 读取指定行内容（逻辑不变）
    while (current_line < line) {
        size_t len = 0;
        while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != '\r') {
            if (len + 1 >= buf_size) {
                buf_size *= 2;
                char *new_buf = realloc(buffer, buf_size);
                if (!new_buf) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buf;
            }
            buffer[len++] = (char)ch;
        }

        if (ch == '\r') ch = fgetc(fp); // 处理Windows换行
        buffer[len] = '\0';
        current_line++;
        if (ch == EOF) break;
    }

    fseek(fp, start_pos, SEEK_SET); // 恢复文件指针

    if (current_line != line) {
        free(buffer);
        return NULL; // 行数不足直接返回
    }

    // 优化内存分配
    char *clean_line = realloc(buffer, strlen(buffer) + 1);
    if (!clean_line) clean_line = buffer;

    // 分割逻辑
    char **result = malloc(3 * sizeof(char*)); // 固定分配3个指针
    if (!result) {
        free(clean_line);
        return NULL;
    }
    result[0] = result[1] = result[2] = NULL; // 初始化为NULL

    char *token = strtok(clean_line, " \t"); // 按空格/制表符分割
    int count = 0;
    while (token && count < 3) {
        result[count] = strdup(token); // 独立复制字符串
        if (!result[count]) { // 内存分配失败处理
            for (int i = 0; i < count; i++) free(result[i]);
            free(result);
            free(clean_line);
            return NULL;
        }
        count++;
        token = strtok(NULL, " \t");
    }

    free(clean_line); // 释放原始行内容

    // 如果完全没有分割出任何token（例如空行或全空格）
    if (count == 0) {
        free(result);
        return NULL;
    }

    return result;
}