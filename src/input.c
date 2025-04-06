#include "headers/input.h"

void check_flag(Flags *flags, const char *flag)
{
	if(strcmp(flag, "--divide") == 0)
		*flags = Divide;

	else if(strcmp(flag, "--margin") == 0)
		*flags = Margin;

	else if(strcmp(flag, "--input") == 0)
		*flags = Input_name;

	else if(strcmp(flag, "--output") == 0)
		*flags = Output_name;
	
	else
		*flags = Error;
}

void *scan_flags(Flags *flags, char **argv, int flag_index)
{
	void *value = NULL;

	if(argv[flag_index][0] == '-' && argv[flag_index][1] == '-')
		check_flag(flags, argv[flag_index]);
	
	if(*flags == 0)
	{
		printf("Podano nieprawidlowa flage\n");
		return value;
	}

	else
		value = argv[++flag_index];

	return value;
}

