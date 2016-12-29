#include<stdio.h>
#include <map>
#include <queue>
#include <algorithm>
#include <iterator>
#define NONE '.'
#define WHITE 'W'
#define BLACK 'B'
#define GOAL 'G'
#define LEFT 'L'
#define UP 'U'
#define RIGHT 'R'
#define DOWN 'D'

char map[8][9];

unsigned long long int goalState = 0;
unsigned long long int wallState = 0;

std::map<unsigned long long int, int> alreadyEntered;

std::vector<int> goalX;
std::vector<int> goalY;

int goalLength[8][8];

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
	
	int getMinLengthToGoal() const
	{
		if (minLength > 0)
			return minLength;

		int maxLength = 0;

		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (getBit(m_State, x, y) == 1 && goalLength[y][x] > maxLength)
				{
					maxLength = goalLength[y][x];
				}
			}
		}

		minLength = maxLength + m_Move.size();

		return minLength;
	}	

	int heuristic() const
	{
		if (cost > 0)
		{
			return cost;
		}

		int maxLength = 0;

		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (getBit(m_State, x, y) == 1 && goalLength[y][x] > maxLength)
				{
					maxLength = goalLength[y][x];
				}
			}
		}

		cost = maxLength;

		return cost;
	}

	bool operator<(const Case& rhs) const
	{
		return heuristic() > rhs.heuristic();
	}

	mutable int cost = -1;
	mutable int minLength = -1;
};

void getMinCase(const Case& startCase)
{
	int minNum = -1; //일반적으로 최단 거리가 이거보다 먼 경우는 없을거라고 기대.

	std::priority_queue<Case> queue[2];
	int index = 0;

	queue[index].push(startCase);

	while (!queue[index].empty())
	{
		auto nowCase = queue[index].top();
		queue[index].pop();

		std::vector<int> moveList = { LEFT, UP, RIGHT, DOWN };
		std::vector<Case> nextCase;
		std::transform(moveList.begin(), moveList.end(), std::back_inserter(nextCase), [&nowCase](int dir)
		{
			return nowCase.getNextCase(dir);
		});

		for (auto& next : nextCase)
		{
			if (minNum != -1 && next.getMinLengthToGoal() >= minNum)
				continue;

			auto it = alreadyEntered.find(next.m_State);

			//이미 더 짧게 도착한 적 있는 경우
			if (it != alreadyEntered.end())
			{
				if (it->second <= next.m_Move.size())
				{
					continue;
				}
				else
				{
					it->second = next.m_Move.size();
				}
			}
			else
			{
				alreadyEntered[next.m_State] = next.m_Move.size();
			}

			if (next.m_State == goalState)
			{
				minNum = next.m_Move.size();
				printf("move : %d, path : %s\n", next.m_Move.size(), next.m_Move.c_str());

				//이 시점에서 가망 없는 애들 싹 다 비우기
				int nextIndex = (index + 1) % 2;

				//갱신된 최단 거리보다 더 짧게 도착 가능한 애들만 남기고 나머지 다 버림
				while (!queue[index].empty())
				{
					if (queue[index].top().getMinLengthToGoal() < minNum)
					{
						queue[nextIndex].push(queue[index].top());
					}

					queue[index].pop();
				}

				index = nextIndex;

				break;
			}

			queue[index].push(next);
		}
	}
}

void initLengthToGoal(int x, int y, int dis)
{
	//이미 더 빨리 도착
	if (goalLength[y][x] != 0 && goalLength[y][x] < dis)
		return;

	std::vector<int> moveList = { LEFT, UP, RIGHT, DOWN };

	goalLength[y][x] = dis;

	for (int i = 0; i < 4; i++)
	{
		int nextX = x;
		int nextY = y;

		switch (moveList[i])
		{
		case LEFT:
			nextX--;
			break;
		case RIGHT:
			nextX++;
			break;
		case UP:
			nextY--;
			break;
		case DOWN:
			nextY++;
			break;
		}

		if (nextX >= 0 && nextX < 8 && nextY >= 0 && nextY < 8 && 
			getBit(wallState, nextX, nextY) == 0 && getBit(goalState, nextX, nextY) == 0)
		{
			initLengthToGoal(nextX, nextY, dis + 1);
		}
	}
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("arg error\n");
		return -1;
	}

	int moveNum = 0;
	Case firstCase;

	freopen(argv[1], "r", stdin);

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
				setBit(&firstCase.m_State, x, y, 1);
			}

			if (map[y][x] == GOAL)
			{
				setBit(&goalState, x, y, 1);
				goalX.push_back(x);
				goalY.push_back(y);
			}

			if (map[y][x] == BLACK)
			{
				setBit(&wallState, x, y, 1);
			}
		}
	}

	for (int i = 0; i < goalX.size(); i++)
	{
		initLengthToGoal(goalX[i], goalY[i], 0);
	}

	getMinCase(firstCase);

	return 0;
}