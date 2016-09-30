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
