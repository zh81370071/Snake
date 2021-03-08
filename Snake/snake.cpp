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
//1 ���õ�ͼ��Χ(�߽�) �� ��
//2 ��ʼ����  ��ʼ��ʳ��
//3 ���ߺ�ʳ����ʾ����Ļ��
//4 �ߵ��ƶ�(ͨ��wasd���������ߵ��ƶ�����)
/*
a> ������ǽ
b> �������ߵ�����
c> �������ϰ���
d> ������ʳ��( �ߵ���������һ��,ԭʳ����ʧ,�����µ�ʳ��,���ӷ���)

*/
//5 �ߵ����� ����

typedef struct _body
{
	int x;
	int y;

}BODY;
typedef struct snake
{
	BODY list[WIDE*HIGH];//����.�����ÿһ�����궼��BODY����
	int size;//�ߵ������С
	BODY food;//ʳ�������
	COORD coord;//����λ����Ϣ
	int dx;//��x���ƶ��ķ���
	int dy;//��y���ƶ��ķ���
	int score;//�÷�
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
	srand(time(NULL));//�������������
	//��ʼ��ʳ������
	snake->food.x = rand() % WIDE;
	snake->food.y = rand() % HIGH;
}
void InitSnake(SNAKE  *snake)
{
	//��ʼ����ͷ����
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//��ʼ����β����
	snake->list[1].x = WIDE / 2 - 1;
	snake->list[1].y = HIGH / 2;
	//��ʼ���ߵ������С
	snake->size = 2;
	//��ʼ��ʳ�������
	InitFood(snake);
	//��ʼ���ߵ��ƶ�����
	snake->dx = 1;
	snake->dy = 0;
	//��ʼ������
	snake->score = 0;

}

void ShowUi(SNAKE  *snake)
{
	//��ʾ�� ע��: ÿ����ʾ�߻�ʳ��,��Ҫ������ʾ��λ��,(����λ��)
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
	//��ʾʳ��
	snake->coord.X = snake->food.x;
	snake->coord.Y = snake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf("��");
	//��ԭ��β�͵�λ����ʾΪ�ո�
	snake->coord.X = snake->tail.x;
	snake->coord.Y = snake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);
	printf(" ");
}


void MoveSnake(SNAKE *snake)
{
	//��¼β�͵�����
	snake->tail.x = snake->list[snake->size - 1].x;
	snake->tail.y = snake->list[snake->size - 1].y;
	//�����ߵ���������,����ͷ
	for (int i = snake->size - 1; i > 0; i--) {
		snake->list[i] = snake->list[i - 1];//�������ǰһ��Ԫ�ص��������һ��
	}
	//������ͷ
	snake->list[0].x += snake->dx;
	snake->list[0].y += snake->dy;
}

void ControlSnake(SNAKE  *snake)
{
	char  key = 0;
	while (_kbhit()) //�ж��Ƿ��°���,���²�����0 
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
	printf("��Ϸ���� �÷�Ϊ%d\n", snake->score);
	Sleep(3000);
	exit(0);//�˳�����
}


void snake_eat_body(SNAKE  *snake)
{
	//�����ͷ���ߵ����������һ������������ͬ,��Ե��Լ�����
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
	//�����ͷ��ʳ��������غ�.��Ե���ʳ��
	if (snake->list[0].x == snake->food.x &&
		snake->list[0].y == snake->food.y) {
		//ԭʳ����ʧ,�����µ�ʳ��
		InitFood(snake);
		//�ߵ���������һ��
		snake->size++;
		//��������if()
		snake->score += 10;
	}
}


void StartGame(SNAKE  *snake)
{
	while (snake->list[0].x <60 && snake->list[0].x >= 0 &&
		snake->list[0].y <20 && snake->list[0].y >= 0)
	{
		//�����ߵķ���
		ControlSnake(snake);
		//�����ߵ�����
		MoveSnake(snake);
		//���ƶ�
		//system("cls");//����
		//init_wall();
		ShowUi(snake);
		//���Ƿ������Լ�
		snake_eat_body(snake);
		//���Ƿ�����ʳ��-(����ʳ��,ԭʳ����ʧ,�����µ�ʳ��)
		snake_eat_food(snake);
		Sleep(100);//��ʱ0.5s
	}
	GameEnd(snake);
}

void hide_cur()
{

	//���ؿ���̨���
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