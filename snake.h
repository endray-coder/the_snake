#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<time.h>

#define MAP_HEIGHT 20
#define MAP_WIDTH 40
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
typedef struct{
    int x,y;
}Food,Snakenode;

#define MAX_FOOD 3 // 最大食物数量
typedef struct{
    Snakenode snakeNode[1000];
    int length,speed;

}Snake;
void GotoXY(int,int);
void Hide();
int Menu();
void Help();
void About();
void InitMap();
void PrintFood();
int MoveSnake();
int IsCorrect();
void SpeedControl();