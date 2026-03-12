#include<bits/stdc++.h>
#include "snake.h"
using namespace std;
void SetConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
Snake snake;
int MAX_FOOD=3;
char SNAKE_SKIN='*';
int MAX_OBSTACLES_COUNT=5; // 默认障碍物数量
Food *food;
char now_dir=RIGHT,direction=RIGHT;
ScoreRecord leaderboard[10];
int leaderboardCount=0;
Obstacle obstacles[MAX_OBSTACLES];
int obstacleCount=0;

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
void ShowLeaderboard(){
    SetConsoleUTF8();
    goprint(40,12,"积分排行榜");
    if(leaderboardCount==0){
        goprint(35,14,"暂无记录");
    }else{
        for(int i=0;i<leaderboardCount;i++){
            goprint(35,14+i,"第" + to_string(i+1) + "名: " + to_string(leaderboard[i].score) + "分");
        }
    }
    goprint(35,24,"按任意键返回");
    Hide();
    _getch();
    system("cls");
}

void AddScore(int score){
    if(leaderboardCount<10){
        leaderboard[leaderboardCount].score=score;
        leaderboardCount++;
    }else if(score>leaderboard[9].score){
        leaderboard[9].score=score;
    }
    for(int i=0;i<leaderboardCount-1;i++){
        for(int j=i+1;j<leaderboardCount;j++){
            if(leaderboard[i].score<leaderboard[j].score){
                ScoreRecord temp=leaderboard[i];
                leaderboard[i]=leaderboard[j];
                leaderboard[j]=temp;
            }
        }
    }
}

void Settings(){
    SetConsoleUTF8();
    goprint(40,12,"设置");
    goprint(35,14,"当前食物数量: " + to_string(MAX_FOOD));
    goprint(35,16,"当前蛇皮肤: " + string(1, SNAKE_SKIN));
    goprint(35,18,"当前障碍物数量: " + to_string(MAX_OBSTACLES_COUNT));
    goprint(35,20,"1. 设置食物数量 (1-5)");
    goprint(35,22,"2. 自定义蛇皮肤");
    goprint(35,24,"3. 设置障碍物数量 (0-10)");
    goprint(35,26,"0. 返回");
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
    }else if(choice==2){
        goprint(40,12,"自定义蛇皮肤");
        goprint(35,14,"请输入一个字符作为蛇的皮肤:");
        Hide();
        char skin_ch;
        skin_ch=_getch();
        SNAKE_SKIN=skin_ch;
        system("cls");
        Settings();
    }else if(choice==3){
        goprint(40,12,"设置障碍物数量");
        goprint(35,14,"请输入障碍物数量 (0-10):");
        Hide();
        char num_ch;
        int num=0;
        while(1){
            num_ch=_getch();
            if(num_ch>='0' && num_ch<='9'){
                num=num_ch-'0';
                break;
            }
        }
        MAX_OBSTACLES_COUNT=num;
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
    goprint(43,22,"5.积分排行榜");
    goprint(43,24,"0.退出");
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

int IsValidPosition(int x, int y) {
    for(int i=0; i<snake.length; i++) {
        if(snake.snakeNode[i].x == x && snake.snakeNode[i].y == y) {
            return 0;
        }
    }
    
    for(int i=0; i<obstacleCount; i++) {
        if(obstacles[i].x == x && obstacles[i].y == y) {
            return 0;
        }
    }
    
    if(x <= 0 || x >= MAP_WIDTH-1 || y <= 0 || y >= MAP_HEIGHT-1) {
        return 0;
    }
    
    return 1;
}

void InitMap(){
    if(food) delete[] food;
    food=new Food[MAX_FOOD];
    for(int i=0;i<MAP_HEIGHT;i++){
        for(int j=0;j<MAP_WIDTH;j++){
            if(i==0 || i==MAP_HEIGHT-1 || j==0 || j==MAP_WIDTH-1){
                goprint(j,i,"#");
            }else{
                goprint(j,i," ");
            }
        }
    }
    
    snake.length=3;
    snake.speed=200;
    for(int i=0;i<snake.length;i++){
        snake.snakeNode[i].x=MAP_WIDTH/2-i;
        snake.snakeNode[i].y=MAP_HEIGHT/2;
        goprint(snake.snakeNode[i].x,snake.snakeNode[i].y,"*");
    }
    
    obstacleCount=0;
    for(int i=0;i<MAX_OBSTACLES_COUNT;i++){
        int x,y;
        do {
            x=rand()%(MAP_WIDTH-2)+1;
            y=rand()%(MAP_HEIGHT-2)+1;
        } while(!IsValidPosition(x,y));
        
        obstacles[obstacleCount].x=x;
        obstacles[obstacleCount].y=y;
        goprint(x,y,"#");
        obstacleCount++;
    }
    
    for(int i=0;i<MAX_FOOD;i++){
        int x,y;
        do {
            x=rand()%(MAP_WIDTH-2)+1;
            y=rand()%(MAP_HEIGHT-2)+1;
        } while(!IsValidPosition(x,y));
        
        food[i].x=x;
        food[i].y=y;
        goprint(x,y,"@");
    }
}

void PrintFood(int index){
    // 生成新的食物位置
    int x,y;
    do {
        x=rand()%(MAP_WIDTH-2)+1;
        y=rand()%(MAP_HEIGHT-2)+1;
    } while(!IsValidPosition(x,y));
    
    food[index].x=x;
    food[index].y=y;
    goprint(x,y,"@");
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
    
    // 检查是否撞到障碍物
    for(int i=0;i<obstacleCount;i++){
        if(snake.snakeNode[0].x==obstacles[i].x && 
           snake.snakeNode[0].y==obstacles[i].y){
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
    
    if(!IsCorrect()){
        int score=snake.length-3;
        AddScore(score);
        goprint(MAP_WIDTH/2-8,MAP_HEIGHT/2,"游戏结束！得分: " + to_string(score));
        _getch();
        system("cls");
        return 0;
    }
    for(int i=1;i<snake.length;i++){
        temp2=snake.snakeNode[i];
        snake.snakeNode[i]=temp;
        temp=temp2;
    }
    bool ateFood = false;
    for(int i=0;i<MAX_FOOD;i++){
        if(snake.snakeNode[0].x==food[i].x && snake.snakeNode[0].y==food[i].y){
            snake.length++;
            snake.snakeNode[snake.length-1]=temp;
            PrintFood(i);
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