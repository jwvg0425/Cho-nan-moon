#include<stdio.h>
#include<map>
#include<queue>
#include<set>
#define NONE '.'
#define WHITE 'W'
#define BLACK 'B'
#define GOAL 'G'
#define LEFT 'L'
#define UP 'U'
#define RIGHT 'R'
#define DOWN 'D'
#define FILE

char map[8][9];

char moveList[4] = { LEFT, UP, RIGHT, DOWN };

unsigned long long int goalState = 0;
unsigned long long int wallState = 0;

std::set<unsigned long long int> alreadyEntered;

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
						if (y>0 && map[y - 1][x] != BLACK&& getBit(newCase.m_State,x,y-1) != 1)
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
						if (x<7 && map[y][x + 1] != BLACK && getBit(newCase.m_State, x + 1, y) != 1)
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
						if (y<7 && map[y + 1][x] != BLACK && getBit(newCase.m_State, x, y + 1) != 1)
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

bool isChecked(unsigned long long int state)
{
	if (alreadyEntered.find(state) == alreadyEntered.end())
	{
		return false;
	}

	return true;
}

std::queue<Case> moveCase;
int minMove = 30;


int main()
{
	int moveNum = 0;
	Case firstCase;

	freopen("input.txt", "r", stdin);

	for (int y = 0; y < 8; y++)
	{
		scanf("%s", map[y]);
	}

#ifdef FILE
	freopen("output.txt", "w", stdout);
#endif

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (map[y][x] == WHITE)
			{
				setBit(&firstCase.m_State, x, y, 1);
			}

			if (map[y][x] == GOAL)
			{
				setBit(&goalState, x, y, 1);
			}

			if (map[y][x] == BLACK)
			{
				setBit(&wallState,x,y,1);
			}
		}
	}

	moveCase.push(firstCase);
	alreadyEntered.insert(firstCase.m_State);

	for (int move = 0;; move++)
	{
		int caseSize = moveCase.size();

		if (caseSize == 0)
		{
			printf("path not found.\n");
			return 0;
		}

		printf("move : %d, case : %d\n", move, caseSize);

		for (int i = 0; i < caseSize; i++)
		{
			Case nowCase = moveCase.front();
			Case nextCase;
			moveCase.pop();

			//도착
			if (nowCase.m_State == goalState)
			{
				printf("%s\n", nowCase.m_Move.c_str());
				return 0;
			}

			for (int j = 0; j < 4; j++)
			{
				nextCase = nowCase.getNextCase(moveList[j]);
				if (!isChecked(nextCase.m_State))
				{
					moveCase.push(nextCase);
					alreadyEntered.insert(nextCase.m_State);
				}
			}
		}
	}

	return 0;
}