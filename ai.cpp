#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <stdio.h> // freopen
#include <cstdlib> // rand
#include <ctime>	// rand
#include <cstring>

using namespace std;

int max_ai;

struct mov_data
{
	short x, y;
};

struct ai
{
	unsigned short id;
	mov_data datas[20];
	unsigned short score = 0;
}ais[1010];

int grid[10][10];

ai ns[100010];

const int weight = 5, hight = 6;
const int max_mov = 15;

int score = 0;
struct settings
{
	int delay;
}setting;

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
	while (1)
	{
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
		}
	}
	return;
}

void game(ai now)
{
	__init__();
	for (int i = 1; i <= max_mov; i++)
	{
		int x, y;
		x = now.datas[i].x;
		y = now.datas[i].y;
		mov(x, y, i);
	}
}

bool cmp(ai x, ai y)
{
	return x.score > y.score;
}

int main()
{
	srand(time(0));

	for (int i = 1; i <= 1000; i++)
	{
		// make random ai;
		ais[i].id = i;
		for (int j = 1; j <= max_mov; j++)
			ais[i].datas[j].x = rand() % 6 + 1, ais[i].datas[j].y = rand() % 5 + 1;
	}
	/*
	FILE *load;
	load = fopen("ai.out", "r");
	for (int j = 1; j <= max_mov; j++)
		fscanf(load, "%d%d", &ais[1].datas[j].x, &ais[1].datas[j].y);

	for (int i = 2; i <= 1000; i++)
	{
		ais[i].id = i;
		for (int j = 1; j <= max_mov; j++)
			ais[i].datas[j].x = rand() % 6 + 1, ais[i].datas[j].y = rand() % 5 + 1;
	}
	*/
	const int G = 20000;
	int generations = G;
	while (generations--)
	{
		for (int i = 1; i <= 1000; i++)
		{
			// play ai
			game(ais[i]);
			ais[i].score = score;
		}
		sort(ais + 1, ais + 1000 + 1, cmp);
	//	printf("%d: %d\n", 1000 - generations, ais[1].score);
		if ((G - generations) % 100 == 0)
		{
			printf("%d: %d\n", G - generations, ais[1].score);
		}
		// natural selection
		memset(ns, 0, sizeof ns);
		int cnt = 0;
		for (int i = 1; i <= 500; i++)
		{
			int nb = ais[i].score / 750;
			for (int j = 1; j <= nb; j++)
			{
				ns[++cnt] = ais[i];
			}
		}
		ai NS[1010];
		for (int i = 1; i <= 1000; i++)
			NS[i] = ns[rand() % cnt + 1];


		// hybridization
		ai hy[1010];
		for (int i = 1; i <= 1000; i++)
		{
			ai a = NS[rand() % 1000 + 1];
			ai b = NS[rand() % 1000 + 1];
			int bk = rand() % 14 + 1;
			for (int j = 1; j <= bk; j++)
				hy[i].datas[j] = a.datas[j];
			for (int j = bk + 1; j <= 15; j++)
				hy[i].datas[j] = a.datas[j];
		}


		// variation
		const double V = 0.1;
		for (int i = 1; i <= 1000 * V; i++)
		{
			int id = rand() % 1000 + 1;
			int dataid = rand() % 15 + 1;
			hy[id].datas[dataid].x = rand() % 6 + 1;
			hy[id].datas[dataid].y = rand() % 5 + 1;
		}

		//copy to ais
		// keep the best;
		ais[1] = ais[1];
		for (int i = 2; i <= 1000; i++)
			ais[i] = hy[i];

	}
	sort(ais + 1, ais + 1000 + 1, cmp);
	FILE *output;
	output = fopen("ai.out", "w");
	for (int i = 1; i <= 15; i++)
		fprintf(output, "%d %d\n", ais[1].datas[i].x, ais[1].datas[i].y);
	fprintf(output, "%d\n", ais[1].score);
	return 0;
}
