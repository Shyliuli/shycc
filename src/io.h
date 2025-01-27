#ifndef IO_H
#define IO_H
#include <stdio.h>
#include <string.h>
#include "version.h"
#include <assert.h>
#include "mem.h"
#include "utils.h"
#include "command.h"
void load_program_2_mem(char *filename);
void print_help();
void print_version();
char* read_a_line(FILE *fp, uint16_t line_num);
#endif// IO_H