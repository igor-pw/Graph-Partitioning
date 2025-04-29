#include <ctype.h>
#include "headers/input.h"

Flags check_flag(const char *flag)
{
	//funkcja zwraca przypisana do flagi wartosc liczbowa
	if(strcmp(flag, "--divide") == 0)
		return Divide;

	else if(strcmp(flag, "--margin") == 0)
		return Margin;

	else if(strcmp(flag, "--input") == 0)
		return Input_name;

	else if(strcmp(flag, "--output") == 0)
		return Output_name;

	else if(strcmp(flag, "--strict") == 0)
		return Strict;

	//jezeli podana flaga nie poprawna zwracamy wartosc typu Error
	else
		return Error;
}

void *scan_flags(Flags *flags, char **argv, int flag_index)
{
	//przeszukuje argumenty wywolania w celu odnalezienia flag
	
	void *value = NULL;

	//jezeli pierwsze 2 znaki napisu to '-' sprawdzamy czy flaga jest poprawna
	if(argv[flag_index][0] == '-' && argv[flag_index][1] == '-')
		*flags = check_flag(argv[flag_index]);
	
	//zwracamy NULL
	if(*flags == 0)
		return value;

	//jezeli nastepna wartosc to kolejna flaga zwracamy wartosc Error
	if(argv[++flag_index][0] == '-' && argv[flag_index][1] == '-')
	{
		*flags = Error;
		return value;
	}

	//przypisujemy wskaznik do wartosc flagi
	else
		value = argv[flag_index];

	//zwracamy wskanik
	return value;
}

