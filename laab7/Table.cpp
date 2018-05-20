#include "Table.h"

Table* newTable(Cut *cut)
{
	Table* table = (Table*)calloc(1, sizeof(Table));
	table->cut = cut;
	table->next = NULL;
	return table;
}

Table* addToTable(Table* table, Cut *cut)
{
	if (!table)
	{
		return newTable(cut);
	}

	Table* mov = table;

	while (mov->next != NULL)
	{
		mov = mov->next;
	}

	mov->next = newTable(cut);

	return table;
}

Table* deleteOfTable(Table* table, Cut *cut)
{
	if (!table)
		return NULL;
	
	Table* mov = table;

	while (mov->next != NULL)
	{
		if 
		mov = mov->next;
	}

}

Table* delete_from_list(Table *table, double x, double y, int i)
{
	Row *row = table->type, *row_prev = NULL;

	while (row != NULL)
	{
		if ((row->X == x) && (row->Y == y) && (row->i == i))
		{
			break;
		}
		row_prev = row;
		row = row->next;
	}

	if (row_prev)
	{
		if (row)
		{
			row_prev->next = row->next;
		}
		else
		{
			row_prev->next = NULL;
		}
	}
	else
	{
		table->type = row->next;
	}
	//free(row);
	return table;
}

// Поиск в списке по данным точки
Row* search_in_list(Table *table, double x, double y, int i)
{
	Row *row = table->type;
	while (row != NULL)
	{
		if ((row->X == x) && (row->Y == y) && (row->i == i))
		{
			break;
		}
		row = row->next;
	}
	return row;
}