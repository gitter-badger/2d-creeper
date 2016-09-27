#include <Windows.h>
#include <conio.h>
typedef struct
{
    int x,y;
    char shape;
    int exist;
} Actor;
Actor actors[100];


void draw(Actor* actors)
{
    CHAR_INFO buffer[24][80];
    memset(buffer,0,sizeof(CHAR_INFO)*24*80);
    int i;
    for(i =0;i<100;i++)
    {
        if(actors[i].exist == 0)
            continue;
        int x = actors[i].x;
        int y =actors[i].y;
        buffer[y][x].Char.AsciiChar = actors[i].shape;
        buffer[y][x].Attributes = 15;//WHITE
    }
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD sz = {80,24};
    COORD xy = {0,0};
    SMALL_RECT r = {0,0,80,24};
    WriteConsoleOutput(h,buffer,sz,xy,&r);
}

void keyboard(Actor* a)
{
    if(!kbhit())
        return;
    int key = getch();
    if(key == 'w')
        a->y--;
    if(key == 's')
        a->y++;
    if(key == 'a')
        a->x--;
    if(key == 'd')
        a->x++;
}

int main()
{
    memset(actors,0,sizeof(Actor)*100);
    Actor a;
    a.x = 10;
    a.y = 10;
    a.exist = 1;
    a.shape ='@';

    actors[0] = a;

    Actor * hero = &(actors[0]);
    while(1)
    {
        keyboard(hero);
        draw(actors);
        Sleep(100);
    }

    return 0;
}
