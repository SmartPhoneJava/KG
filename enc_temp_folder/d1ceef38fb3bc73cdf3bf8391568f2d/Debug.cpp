#include "stdafx.h"
#include <iostream>
#include <stdio.h>

#include "Debug.h"

void debug(const char* text, int number)
{

	char str[BUFFER_MAX_SIZE];
	sprintf_s(str, "%s: %d \n", text, number);

	OutputDebugString(str);
}