#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum flags {Error, Divide, Margin, Input_name, Output_name, Strict} Flags;

Flags check_flags(const char *flag);
void *scan_flags(Flags *flags, char **argv, int flag_index);
