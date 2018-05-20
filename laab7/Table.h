#pragma once
#include "Cut.h"
#include <iostream>

typedef struct Table
{
	Cut* cut;
	Table* next;
} Table;