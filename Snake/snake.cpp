#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <time.h>
using namespace std;
#define WIDE  60
#define HIGH  20
//1 设置地图范围(边界) 宽 高
//2 初始化蛇  初始化食物
//3 将蛇和食物显示在屏幕上
//4 蛇的移动(通过wasd按键控制蛇的移动方向)
/*
a> 蛇碰到墙
b> 蛇碰到蛇的身体
c> 蛇碰到障碍物
d> 蛇碰到食物( 蛇的身体增长一节,原食物消失,生成新的食物,增加分数)

*/
//5 蛇的死亡 积分

typedef struct _body
{
	int x;
	int y;

}BODY;
typedef struct snake
{
	BODY list[WIDE*HIGH];//身体.身体的每一节坐标都是BODY类型
	int size;//蛇的身体大小
	BODY food;//食物的坐标
	COORD coord;//光标的位置信息
	int dx;//蛇x轴移动的方向
	int dy;//蛇y轴移动的方向
	int score;//得分
	BODY tail;
}SNAKE;




void InitWall()
{
	for (int i = 0; i <= HIGH; i++)
	{
		for (int j = 0; j <= WIDE; j++)
		{
			if (i==0 || j == 0||i == HIGH || j == WIDE)
			{
				printf("+");
			}
			else
			{
				printf(" ");
			}

		}
		printf("\n");
	}
}

void InitFood(SNAKE  *snake)
{
	srand(time(NULL));//设置随机数种子
	//初始化食物坐标
	snake->food.x = rand() % WIDE;
	snake->food.y = rand() % HIGH;
}
void InitSnake(SNAKE  *snake)
{
	//初始化蛇头坐标
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//初始化蛇尾坐标
	snake->list[1].x = WIDE / 2 - 1;
	snake->list[1].y = HIGH / 2;
	//初始化蛇的身体大小
	snake->size = 2;
	//初始化食物的坐标
	InitFood(snake);
	//初始化蛇的移动方向
	snake->dx = 1;
	snake->dy = 0;
	//初始化分数
	snake->score = 0;

}

void ShowUi(SNAKE  *snake)
{
	//显示蛇 注意: 每次显示蛇或食物,都要设置显示的位置,(光标的位置)
	for (int i = 0; i < snake->size; i++) {
		snake->coord.X = snake->list[i].x;
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
		if (0 == i) {
			printf("#");
		} else {
			printf("*");
		}
	}
	//显示食物
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("。");
	//将原来尾巴的位置显示为空格
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}


void MoveSnake(SNAKE *snake)
{
	//记录尾巴的坐标
	snake->tail.x = snake->list[snake->size - 1].x;
	snake->tail.y = snake->list[snake->size - 1].y;
	//更新蛇的身体坐标,除蛇头
	for (int i = snake->size - 1; i > 0; i--) {
		snake->list[i] = snake->list[i - 1];//将数组的前一个元素的坐标给后一节
	}
	//更新蛇头
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

void ControlSnake(SNAKE  *snake)
{
	char  key = 0;
	while (_kbhit()) //判断是否按下按键,按下不等于0 
	{
		key = _getch();
	}
	switch (key)
	{
	case 'a':
		snake->dx = -1;
		snake->dy = 0;
		break;
	case 'w':
		snake->dx = 0;
		snake->dy = -1;
		break;
	case 's':
		snake->dx = 0;
		snake->dy = 1;
		break;
	case 'd':
		snake->dx = 1;
		snake->dy = 0;
		break;
	}

}
void GameEnd(SNAKE  *snake)
{
	snake->coord.X = 25;
	snake->coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("游戏结束 得分为%d\n", snake->score);
	Sleep(3000);
	exit(0);//退出进程
}


void snake_eat_body(SNAKE  *snake)
{
	//如果蛇头和蛇的身体的任意一节身体坐标相同,则吃到自己身体
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x &&
			snake->list[0].y == snake->list[i].y)
		{
			GameEnd(snake);

		}

	}

}
void snake_eat_food(SNAKE  *snake)
{
	//如果蛇头和食物的坐标重合.则吃到了食物
	if (snake->list[0].x == snake->food.x &&
		snake->list[0].y == snake->food.y) {
		//原食物消失,生成新的食物
		InitFood(snake);
		//蛇的身体增长一节
		snake->size++;
		//分数增加if()
		snake->score += 10;
	}
}


void StartGame(SNAKE  *snake)
{
	while (snake->list[0].x <60 && snake->list[0].x >= 0 &&
		snake->list[0].y <20 && snake->list[0].y >= 0)
	{
		//控制蛇的方向
		ControlSnake(snake);
		//更新蛇的坐标
		MoveSnake(snake);
		//蛇移动
		//system("cls");//清屏
		//init_wall();
		ShowUi(snake);
		//蛇是否碰到自己
		snake_eat_body(snake);
		//蛇是否碰到食物-(碰到食物,原食物消失,产生新的食物)
		snake_eat_food(snake);
		Sleep(100);//延时0.5s
	}
	GameEnd(snake);
}

void hide_cur()
{

	//隐藏控制台光标
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}


int main()
{
	SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));
	InitSnake(snake);
	InitWall();
	ShowUi(snake);
	hide_cur();
	StartGame(snake);
	system("pause");
}