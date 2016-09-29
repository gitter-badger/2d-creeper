#include <Windows.h>
#include <conio.h>
#include <stdio.h>

struct Actor;
typedef void (*fptr)(struct Actor*);

HANDLE COUT;
HANDLE CIN;
DWORD mode;
WORD key;
COORD pos = { 0, 0 };
int event;

typedef struct Actor
{
    int x,y;
    char shape;
    int exist;
	fptr tick_fn;
	char color;

} Actor;
Actor actors[100];

int be_input()
{
	INPUT_RECORD input_record;
	DWORD input_count;

	PeekConsoleInput(CIN, &input_record, 1, &input_count);
	return input_count;
}

int get_input_2(WORD *vkey, COORD *pos)
{
	INPUT_RECORD input_record;
	DWORD input_count;
	ReadConsoleInput(CIN, &input_record, 1, &input_count);
	switch (input_record.EventType)
	{
	case MOUSE_EVENT:
		if (pos && (input_record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(COUT, &csbi);
			*pos = input_record.Event.MouseEvent.dwMousePosition;
			pos->X -= csbi.srWindow.Left;
			pos->Y -= csbi.srWindow.Top;
			return MOUSE_EVENT;
		}
		break;
	case KEY_EVENT:
		if (vkey && (input_record.Event.KeyEvent.bKeyDown))
		{
			*vkey = input_record.Event.KeyEvent.wVirtualKeyCode;
			return KEY_EVENT;
		}
	}
	return;
}

void select_menu()
{
	Sleep(100);
	printf("[    2d-creeper v1.0    ]\n");
	Sleep(100);
	printf("[                       ]\n");
	Sleep(100);
	printf("[ 1. Play Game          ]\n");
	Sleep(100);
	printf("[ 2. Shutdown Game      ]\n");
	Sleep(100);
	printf("[                       ]\n");
	while (1)
	{
		CIN = GetStdHandle(STD_INPUT_HANDLE);
		COUT = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(CIN, &mode);
		SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
		if (be_input())
		{
			event = get_input_2(&key, &pos);
		}
		if ((pos.X > 0 && pos.X < 24) && pos.Y == 2)
		{
			return;
		}
		else if ((pos.X > 0 && pos.X < 24) && pos.Y == 3)
		{
			exit(0);
		}
	}
	exit(0);
}
void set_cursor(int visibility)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	if(visibility == 0)
		info.bVisible = 0;
	else
		info.bVisible = 1;
	SetConsoleCursorInfo(h,&info);
}
int boundary_ok(int x, int y)
{
	if(x<0||y<0 || x>=80||y>=24)
		return 0;
	return 1;
}

int in_hero_sight(Actor* hero, Actor * a)
{
	int from_x = hero->x - 5;
	int to_x = hero->x +5;
	int from_y = hero->y -5;
	int to_y = hero->y +5;
	
	if(a->x >= from_x && a->x <=to_x 
		&& a->y >=from_y && a->y <= to_y)
		return 1;
		
	return 0;
	
}

Actor* get_actor_at(Actor* actors,int x, int y)
{
	for(int i = 0;i<100;i++)
	{
		Actor* a = &(actors[i]);
		if(a->x == x && a->y == y)
			return a;
	}
	return 0;
}
void draw(Actor* actors)
{
    CHAR_INFO buffer[24][80];
    for(int cy = 0; cy<24;cy++)
	{
		for(int cx = 0; cx<80;cx++)
		{
			buffer[cy][cx].Char.AsciiChar = ' ';
			buffer[cy][cx].Attributes = (15-8) <<4;
		}
	}
	
	Actor* hero = &(actors[0]);
	for(int cy = 0;cy<10;cy++)
	{
		for(int cx = 0;cx<20;cx++)
		{
			//sight_x와 y는 hero를 가장 왼쪽 위로 해서 10x20이 아니라
			//hero의 -10왼쪽 -5위 쪽을 사각형의 왼족 위로 해서 10x20
			int sight_x = hero->x +cx -10;
			int sight_y = hero->y +cy -5;
			Actor* a = get_actor_at(actors,sight_x,sight_y);
			if(a == 0)
			{
				buffer[cy][cx].Char.AsciiChar = ' ';
				buffer[cy][cx].Attributes = 0;
			}
			else
			{				
				buffer[cy][cx].Char.AsciiChar = a->shape;
				buffer[cy][cx].Attributes = a->color;
			}
		}
	}
	
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD sz = {80,24};
    COORD xy = {0,0};
    SMALL_RECT r = {0,0,80,24};
    WriteConsoleOutput(h,buffer,sz,xy,&r);
}
void move_relative(Actor * a, int x, int y)
{
	int to_x = a->x + x;
	int to_y = a->y + y;
	if(boundary_ok(to_x, to_y))
	{
		a->x = to_x; 
		a->y = to_y;
	}
}

void keyboard(Actor* a)
{
    if(!kbhit())
        return;
    int key = getch();
    if(key == 'w')
        move_relative(a,0,-1);
    if(key == 's')
        move_relative(a,0,1);
    if(key == 'a')
        move_relative(a,-1,0);
    if(key == 'd')
        move_relative(a,1,0);
}
void move_random(Actor* a)
{
	int r = rand() % 10;
	if(r == 0 )
		move_relative(a,-1,0);
	if(r == 1 )
		move_relative(a,1,0);
	if(r == 2 )
		move_relative(a,0,-1);
	if(r == 3 )
		move_relative(a,0,1);
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
	
	select_menu();
	system("cls");
	
	set_cursor(0);
	
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
