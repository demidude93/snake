#include <iostream>
#include <cstdlib>
#include <stdio.h>
#ifdef __linux__
#include <unistd.h>
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

#ifdef __linux__ 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
#endif

bool gameOver;
const int width = 20;
const int height = 20;
int x,y, fruitX, fruitY, score;
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;
int tailX[100], tailY[100];
int nTail;

void Setup(){
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw(){
    system("clear");
    for(int i = 0; i < width; i++){
        cout << "#";
    }
    cout << endl;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(j == 0){
                cout << "#";
            }else if(j == width-1){
                cout << "#";
            }else if(i == y && j == x){
                cout << "O";
            }else if(i == fruitY && j == fruitX){
                cout << "x";
            }else{
                bool print = false;
                for(int k = 0; k < nTail; k++){
                    if(tailX[k] == j && tailY[k] == i){
                        cout << "o";
                        print = true;
                    }
                }
                if(!print){
                    cout << " ";
                }
            }
        }
        cout << endl;
    }



    for(int i = 0; i < width; i++){
        cout << "#";
    }
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input(){
    #ifdef __linux__ 
    if(kbhit()){ 
    #endif
    #ifdef _WIN32
    if(_kbhit()){
    #endif
        switch(getchar()){
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    #ifdef __linux__
    }
    #endif
    #ifdef _WIN32
    }
    #endif
}

void Logic(){
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for(int i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;

    }

    switch(dir){
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    /*
    if(x > width || x < 0 || y > height || y < 0){
        gameOver = true;
    }
    */

    if(x >= width) x=0; else if (x < 0) x=width-1;
    if(y >= height) y=0; else if (y < 0) y=height-1;
    for(int i = 0; i < nTail; i++){
        if(tailX[i] == x && tailY[i] == y){
            gameOver = true;
        }
    }
    if(x == fruitX && y == fruitY){
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main(){
    Setup();
    while(!gameOver){
        Draw();
        Input();
        Logic();
        #ifdef __linux__
        usleep(100000);
        #endif
    }
    return 0;
}