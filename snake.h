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
#define MAX_OBSTACLES 10 // 最大障碍物数量

typedef struct{
    int x,y;
}Food,Snakenode,Obstacle;

extern int MAX_FOOD; // 食物数量
extern char SNAKE_SKIN; // 蛇的皮肤字符
extern int MAX_OBSTACLES_COUNT; // 障碍物数量
extern Obstacle obstacles[MAX_OBSTACLES]; // 障碍物数组
extern int obstacleCount; // 障碍物数量
typedef struct{
    int score;
}ScoreRecord;
typedef struct{
    Snakenode snakeNode[1000];
    int length,speed;

}Snake;
void GotoXY(int,int);
void Hide();
int Menu();
void Help();
void About();
void Settings();
void ShowLeaderboard();
void InitMap();
void PrintFood(int);
int MoveSnake();
int IsCorrect();
void SpeedControl();
void AddScore(int);