#ifndef _TEST_COMMON_H
#define _TEST_COMMON_H

#include <stdint.h>

void _printStringAndNumber(char *, unsigned long);
void _printStr(const char *);
void _printNewLine();

uint32_t _compute_kernel_entries(uint64_t);
#endif
