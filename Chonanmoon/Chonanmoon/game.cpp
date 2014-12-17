#include <stdio.h>
#include <vector>
#include <conio.h>
#define NONE '.'
#define WHITE 'W'
#define BLACK 'B'
#define GOAL 'G'
#define LEFT 'L'
#define UP 'U'
#define RIGHT 'R'
#define DOWN 'D'

//숫자 n의 b번째 bit를 v로 설정.(0번째부터 시작)
void setBit(unsigned long long int* n, long long int x, long long int y, long long int v)
{
	if (v)
		*n |= (unsigned long long int)1 << (y * 8 + x);
	else
		*n &= ~((unsigned long long int)1 << (y * 8 + x));
}

//숫자 n의 b번째 bit값 얻어옴.
int getBit(unsigned long long int n, long long int x, long long int y)
{
	return (n >> (y * 8 + x)) & 0x1;
}

char map[8][9];

char moveList[4] = { LEFT, UP, RIGHT, DOWN };

unsigned long long int goalState = 0;

std::vector<int> goalX;
std::vector<int> goalY;


struct Case
{
	unsigned long long int m_State = 0;
	std::string m_Move = "";

	Case getNextCase(char dir)
	{
		Case newCase;

		newCase.m_State = m_State;
		newCase.m_Move = m_Move + dir;

		switch (dir)
		{
		case UP:
			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					if (getBit(newCase.m_State, x, y) == 1)
					{
						if (y>0 && map[y - 1][x] != BLACK&& getBit(newCase.m_State, x, y - 1) != 1)
						{
							setBit(&newCase.m_State, x, y, 0);
							setBit(&newCase.m_State, x, y - 1, 1);
						}
					}
				}
			}
			break;
		case RIGHT:
			for (int x = 7; x >= 0; x--)
			{
				for (int y = 0; y < 8; y++)
				{
					if (getBit(newCase.m_State, x, y) == 1)
					{
						if (x < 7 && map[y][x + 1] != BLACK && getBit(newCase.m_State, x + 1, y) != 1)
						{
							setBit(&newCase.m_State, x, y, 0);
							setBit(&newCase.m_State, x + 1, y, 1);
						}
					}
				}
			}
			break;
		case DOWN:
			for (int y = 7; y >= 0; y--)
			{
				for (int x = 0; x < 8; x++)
				{
					if (getBit(newCase.m_State, x, y) == 1)
					{
						if (y < 7 && map[y + 1][x] != BLACK && getBit(newCase.m_State, x, y + 1) != 1)
						{
							setBit(&newCase.m_State, x, y, 0);
							setBit(&newCase.m_State, x, y + 1, 1);
						}
					}
				}
			}
			break;
		case LEFT:
			for (int x = 0; x < 8; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					if (getBit(newCase.m_State, x, y) == 1)
					{
						if (x > 0 && map[y][x - 1] != BLACK&& getBit(newCase.m_State, x - 1, y) != 1)
						{
							setBit(&newCase.m_State, x, y, 0);
							setBit(&newCase.m_State, x - 1, y, 1);
						}
					}
				}
			}
			break;
		}

		return newCase;
	}
};

Case nowCase;

void printBoard()
{
	system("cls");
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (map[y][x] == GOAL)
			{
				if (getBit(nowCase.m_State, x, y) == 1)
				{
					printf("▩");
				}
				else
				{
					printf("▣");
				}
			}
			else if (map[y][x] == BLACK)
			{
				printf("■");
			}
			else if (getBit(nowCase.m_State, x, y) == 1)
			{
				printf("□");
			}
			else
			{
				printf("▒");
			}
		}
		printf("\n");
	}
}


int main()
{
	int moveNum = 0;
	

	freopen("input.txt", "r", stdin);

	for (int y = 0; y < 8; y++)
	{
		scanf("%s", map[y]);
	}

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (map[y][x] == WHITE)
			{
				setBit(&nowCase.m_State, x, y, 1);
			}

			if (map[y][x] == GOAL)
			{
				goalX.push_back(x);
				goalY.push_back(y);
			}
		}
	}

	

	for (;;)
	{
		printBoard();
		char key = getch();

		switch (key)
		{
		case 'a':
			nowCase = nowCase.getNextCase(LEFT);
			break;
		case 'w':
			nowCase = nowCase.getNextCase(UP);
			break;
		case 's':
			nowCase = nowCase.getNextCase(DOWN);
			break;
		case 'd':
			nowCase = nowCase.getNextCase(RIGHT);
			break;
		}
	}
	return 0;
}