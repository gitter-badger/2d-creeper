#include <Windows.h>
#include <conio.h>

struct Actor;
typedef void (*fptr)(struct Actor*);

typedef struct Actor
{
    int x,y;
    char shape;
    int exist;
	fptr tick_fn;
	char color;

} Actor;
Actor actors[100];

int boundary_ok(int x, int y)
{
	if(x<0||y<0 || x>=80||y>=24)
		return 0;
	return 1;
}
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
		if(boundary_ok(x,y) ==0 )
			continue;
        buffer[y][x].Char.AsciiChar = actors[i].shape;
        buffer[y][x].Attributes = actors[i].color;
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
void move_random(Actor* a)
{
	int r = rand() % 10;
	if(r == 0 )
		a->x--;
	if(r == 1 )
		a->x++;
	if(r == 2 )
		a->y--;
	if(r == 3 )
		a->y++;
	if(r == 4 )
		a->color = 13;
	else	
		a->color = 11;
}
Actor new_hero(int x, int y)
{
    Actor a;
    memset(&a,0,sizeof(Actor));
    a.x = x;
    a.y = y;
    a.exist = 1;
    a.shape ='@';
    a.color = 15;
    return a;
}
Actor new_creeper(int x, int y)
{
	Actor a;
	memset(&a,0,sizeof(Actor));
	a.x = x;
	a.y = y;
	a.exist =1;
	a.shape = '#';
	a.tick_fn = move_random;
	a.color = 11;
	return a;
}
void tick(Actor* actors)
{
	int i;
	for(i= 0;i<100;i++)
	{
		Actor * a = &(actors[i]);
		if(a->exist == 0)
			continue;
		if(a->tick_fn == 0)
			continue;
		a->tick_fn(a);
	}

}
int main()
{
    int creeper;
    srand(time(0));
    memset(actors,0,sizeof(Actor)*100);

    actors[0] = new_hero(10,10);
    printf("크리퍼를 몇 마리 만드시겠습니까? : ");
    scanf("%d", &creeper);
    for (int i = 0; i < creeper; i++)
    {
        actors[i+1] = new_creeper(rand() % 80, rand()%24);
    }

    Actor * hero = &(actors[0]);
    while(1)
    {
        keyboard(hero);
		tick(actors);
        draw(actors);
        Sleep(100);
    }

    return 0;
}
