#include "header.h"
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
	if(key == 'q')
		a->hp -= 10;
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
void creeper_branch(Actor* a)
{
	CHAR_INFO buffer[24][80];
    buf_init(buffer,0,0);
	Actor* hero = &(actors[0]);
	//포인터 이므로 .대신 -> 만약 포인터앞에 *를 붙여서 구조체로 바꾸면 .를 써도됨
	//예: function((*a).x, (*a).y)
	int check = check_player_near(a->x, a->y);
	if (check == 1)
	{
		//각각의 크리퍼정보가 a안에 들어있음 
		a->tick_fn=creeper_blink;
		Boom(&a);
		buffer_draw_hp(buffer,hero,0,13);
	}
	else
	{
		move_random(a);
	}
	return;
}
//여기서 무한루프가 생김 
int check_player_near(int x, int y)
{
	//오타 떄문에 무한루프였음
	for (int _y = y-3; _y <= y+3; _y++)
	{
		for (int _x = x-3; _x <= x+3; _x++)
		{
			if ((actors[0].x == _x) && (actors[0].y == _y))
			{
				return 1;
			}
		}
	}
	return 0;
}
void Boom(Actor *a)
{
	//오타 떄문에 무한루프였음
	for (int _y = a->y-3; _y <= a->y+3; _y++)
	{
		for (int _x = a->x-3; _x <= a->x+3; _x++)
		{
			if ((actors[0].x == _x) && (actors[0].y == _y))
			{
				actors[0].hp -= 20;
			}
		}
	}
	return 0;
}
void creeper_blink(Actor* a)
{
	a->color = 13;
	//a가 포인터이므로 . 대신 ->를 사용.
	a->timer++;
	char shape[30];
	sprintf(shape,"%02d",a->timer);
	//그냐 처음 1문자만 사용.
	a->shape = shape[0];
	if (a->timer >= 15)
	{
		Boom(a);
		a->exist = 0;
	}
	//틱한번에 250ms를 쉬는게 아니므로 지우기. 쉬는것은 메인함수안의 wait에서 담당.
	//Sleep(250);
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
	a.max_hp = 150;
	a.hp = 150;
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
	a.tick_fn = creeper_branch;
	a.color = 11;
	a.max_hp = 300;
	a.hp = 300;
	return a;
}
