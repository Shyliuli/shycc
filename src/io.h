#ifndef IO_H
#define IO_H
#include <stdio.h>
#include <string.h>
#include "version.h"
#include <assert.h>
#include "mem.h"
void load_program_2_mem(char *filename);
void print_help();
void print_version();
char* read_a_line(FILE *fp, uint16_t line);
#endif// IO_H