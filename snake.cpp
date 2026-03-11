#include<bits/stdc++.h>
#include "snake.h"
using namespace std;
void SetConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
Snake snake;
int MAX_FOOD=3; // 默认食物数量
Food *food; // 食物数组指针
char now_dir=RIGHT,direction=RIGHT;

void GotoXY(int x,int y){
    HANDLE hout;
    COORD cor;
    hout=GetStdHandle(STD_OUTPUT_HANDLE);
    cor.X=x;
    cor.Y=y;
    SetConsoleCursorPosition(hout,cor);

}
void Hide(){
    HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cor_info={1,0};
    SetConsoleCursorInfo(hout,&cor_info);
}

void goprint(int x,int y,string z){
    GotoXY(x,y);
    cout<<z;
}
void Settings(){
    SetConsoleUTF8();
    goprint(40,12,"设置");
    goprint(35,14,"当前食物数量: " + to_string(MAX_FOOD));
    goprint(35,16,"1. 设置食物数量 (1-5)");
    goprint(35,18,"0. 返回");
    Hide();
    char ch;
    int choice=0;
    ch=_getch();
    choice=ch-'0';
    system("cls");
    
    if(choice==1){
        goprint(40,12,"设置食物数量");
        goprint(35,14,"请输入食物数量 (1-5):");
        Hide();
        char num_ch;
        int num=0;
        while(1){
            num_ch=_getch();
            if(num_ch>='1' && num_ch<='5'){
                num=num_ch-'0';
                break;
            }
        }
        MAX_FOOD=num;
        system("cls");
        Settings();
    }
}

int Menu(){
    SetConsoleUTF8();
    goprint(40,12,"欢迎");
    goprint(43,14,"1.开始");
    goprint(43,16,"2.帮助");
    goprint(43,18,"3.关于");
    goprint(43,20,"4.设置");
    goprint(43,22,"0.退出");
    Hide();
    char ch;
    int result=0;
    ch=_getch();
    result=ch-'0';
    system("cls");
    return result;
}

void Help(){
    SetConsoleUTF8();
    goprint(40,12,"帮助信息");
    goprint(35,14,"使用 W/A/S/D 控制蛇的移动");
    goprint(35,16,"吃到食物蛇会变长");
    goprint(35,18,"撞到墙壁或自身游戏结束");
    goprint(35,20,"按任意键返回");
    _getch();
    system("cls");
}

void About(){
    SetConsoleUTF8();
    goprint(40,12,"关于");
    goprint(35,14,"蛇游戏 v1.0");
    goprint(35,16,"一个简单的控制台蛇游戏");
    goprint(35,18,"按任意键返回");
    _getch();
    system("cls");
}

void InitMap(){
    // 释放旧的食物数组
    if(food) delete[] food;
    // 分配新的食物数组
    food=new Food[MAX_FOOD];
    
    // 初始化地图
    for(int i=0;i<MAP_HEIGHT;i++){
        for(int j=0;j<MAP_WIDTH;j++){
            if(i==0 || i==MAP_HEIGHT-1 || j==0 || j==MAP_WIDTH-1){
                goprint(j,i,"#");
            }else{
                goprint(j,i," ");
            }
        }
    }
    
    // 初始化蛇
    snake.length=3;
    snake.speed=200;
    for(int i=0;i<snake.length;i++){
        snake.snakeNode[i].x=MAP_WIDTH/2-i;
        snake.snakeNode[i].y=MAP_HEIGHT/2;
        goprint(snake.snakeNode[i].x,snake.snakeNode[i].y,"*");
    }
    
    // 初始化多个食物
    for(int i=0;i<MAX_FOOD;i++){
        food[i].x=rand()%(MAP_WIDTH-2)+1;
        food[i].y=rand()%(MAP_HEIGHT-2)+1;
        goprint(food[i].x,food[i].y,"@");
    }
}

void PrintFood(int index){
    // 生成新的食物位置
    food[index].x=rand()%(MAP_WIDTH-2)+1;
    food[index].y=rand()%(MAP_HEIGHT-2)+1;
    goprint(food[index].x,food[index].y,"@");
}

int IsCorrect(){
    // 检查是否撞墙
    if(snake.snakeNode[0].x<=0 || snake.snakeNode[0].x>=MAP_WIDTH-1 || 
       snake.snakeNode[0].y<=0 || snake.snakeNode[0].y>=MAP_HEIGHT-1){
        return 0;
    }
    
    // 检查是否撞到自身
    for(int i=1;i<snake.length;i++){
        if(snake.snakeNode[0].x==snake.snakeNode[i].x && 
           snake.snakeNode[0].y==snake.snakeNode[i].y){
            return 0;
        }
    }
    
    return 1;
}

void SpeedControl(){
    if(snake.length<=5){
        snake.speed=200;
    }else if(snake.length<=10){
        snake.speed=160;
    }else if(snake.length<=15){
        snake.speed=140;
    }else if(snake.length<=20){
        snake.speed=120;
    }else if(snake.length<=25){
        snake.speed=80;
    }else{
        snake.speed=60;
    }
}

int MoveSnake(){
    SetConsoleUTF8();
    if(_kbhit()){
        char ch=_getch();
        if(ch==UP && now_dir!=DOWN) direction=UP;
        else if(ch==DOWN && now_dir!=UP) direction=DOWN;
        else if(ch==LEFT && now_dir!=RIGHT) direction=LEFT;
        else if(ch==RIGHT && now_dir!=LEFT) direction=RIGHT;
    }
    Snakenode temp=snake.snakeNode[0];
    Snakenode temp2;
    if(direction==UP) snake.snakeNode[0].y--;
    else if(direction==DOWN) snake.snakeNode[0].y++;
    else if(direction==LEFT) snake.snakeNode[0].x--;
    else if(direction==RIGHT) snake.snakeNode[0].x++;
    
    now_dir=direction;
    
    // 检查是否死亡
    if(!IsCorrect()){
        goprint(MAP_WIDTH/2-5,MAP_HEIGHT/2,"游戏结束！");
        _getch();
        system("cls");
        return 0;
    }
    
    // 移动身体
    for(int i=1;i<snake.length;i++){
        temp2=snake.snakeNode[i];
        snake.snakeNode[i]=temp;
        temp=temp2;
    }
    
    // 检查是否吃到食物
    bool ateFood = false;
    for(int i=0;i<MAX_FOOD;i++){
        if(snake.snakeNode[0].x==food[i].x && snake.snakeNode[0].y==food[i].y){
            snake.length++;
            snake.snakeNode[snake.length-1]=temp;
            PrintFood(i); // 重新生成被吃掉的食物
            SpeedControl();
            ateFood = true;
            break;
        }
    }
    
    if(!ateFood){
        goprint(temp.x,temp.y," ");
    }
    
    // 绘制蛇
    goprint(snake.snakeNode[0].x,snake.snakeNode[0].y,"*");
    
    // 控制速度
    Sleep(snake.speed);
    
    return 1;
}