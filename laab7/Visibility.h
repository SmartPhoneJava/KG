#pragma once

#define FULL_UNVISIBLE -1
#define PART_VISIBLE 1
#define FULL_VISIBLE 0

typedef struct Visibility
{
	int T1;
	int T2;
	int T3;
	int T4;
} Visibility;

// ������� ��������� �������(������ ���������\�����������, ���������)
int checkCut(Visibility vis1, Visibility vis2);