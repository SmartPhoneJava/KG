#include "Visibility.h"

// ������ �� �����. 0 ���� ������, >0 ���� ��������
int isPointVisible(Visibility vis)
{
	int result = ((vis.T1 + vis.T2 + vis.T3 + vis.T4) == 0);
	return result;
}

// �������� �� ������� �������� �������
bool isCutPartVisible(Visibility v1, Visibility v2)
{
	int count = v1.T1 * v2.T1 + v1.T2 * v2.T2 + v1.T3 * v2.T3 + v1.T4 * v2.T4;
	bool result = (count == 0);
	return result;
}

// ������� ��������� �������(������ ���������\�����������, ���������)
int checkCut(Visibility vis1, Visibility vis2)
{
	int first = isPointVisible(vis1);
	int second = isPointVisible(vis2);

	int result = PART_VISIBLE;

	if ((first + second) == FULL_VISIBLE)
		result = FULL_VISIBLE;
	else if ((first != FULL_VISIBLE) && (second != FULL_VISIBLE))
	{
		if (!isCutPartVisible(vis1, vis2))
			result = FULL_UNVISIBLE;
	}

	return result;
}