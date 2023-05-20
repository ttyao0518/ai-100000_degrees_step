#include <iostream>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#endif
#include <stdio.h> // freopen

using namespace std;

int grid[10][10];

const int weight = 5, hight = 6;
const int max_mov = 15;

int score = 0;
struct settings
{
	int delay;
}setting;

struct data
{
	int x, y;
}datas[20];

void __init__()
{
	setting.delay = 150;
	score = 0;
	for (int i = 1; i <= hight; i++)
		for (int j = 1; j <= weight; j++)
			grid[i][j] = 1;
}

void show(int step)
{
	printf("\033c");
	for (int i = 1; i <= hight; i++)
	{
		for (int j = 1; j <= weight; j++)
		{
			switch (grid[i][j])
			{
				case 1:
					printf("↑ ");
					break;
				case 2:
					printf("→ ");
					break;
				case 3:
					printf("↓ ");
					break;
				case 4:
					printf("← ");
					break;
				default:
					printf("R");
					break;
			}
		}
		printf("\n");
	}
	printf("score: %d\tsteps: %d\n", score, step);
}

int turn(int last)
{
	if (last < 4)
		return last + 1;
	return 1;
}

void mov(int x, int y, int step)
{
	grid[x][y] = turn(grid[x][y]);
	score += 90;
	show(step);
	while (1)
	{
		#ifdef _WIN32
		sleep(setting.delay);
		#elif __linux__
		usleep(1000 * setting.delay);
		#endif
		switch (grid[x][y])
		{
			case 1:
				x--;
				break;
			case 2:
				y++;
				break;
			case 3:
				x++;
				break;
			case 4:
				y--;
				break;
			default:
				printf("\033c ERROR");
				exit(0);
		}
		if (grid[x][y] == 0)
			return;
		else
		{
			score += 90;
			grid[x][y] = turn(grid[x][y]);
			show(step);
		}
	}
	return;
}

void game()
{

	printf("\033c");
	__init__();
	FILE *ai;
	ai = fopen("ai.out", "r");
	for (int i = 1; i <= max_mov; i++)
	{
		show(i - 1);
		int x, y;
		fscanf(ai, "%d %d\n", &x, &y);
		mov(x, y, i);
		getchar();
	}
	show(15);
	printf("GAME OVER!\n");
}

int main()
{
	game();
	return 0;
}
