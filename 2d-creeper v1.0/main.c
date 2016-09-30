#include "header.h"
int boundary_ok(int x, int y)
{
	if(x<0||y<0 || x>=80||y>=24)
		return 0;
	return 1;
}
int wait(int ms)
{
	static clock_t old = 0;
	if(old == 0)
		old = clock();
	clock_t clocks_per_ms = CLOCKS_PER_SEC / 1000;
	clock_t now_ms = clock() / clocks_per_ms;
	clock_t old_ms = old / clocks_per_ms;
	if(now_ms <= old_ms + ms)
		return 1;
	old = clock();
	return 0;
}
int main()
{
	int creeper;
	srand(time(0));
	memset(actors,0,sizeof(Actor)*100);
	
	select_menu();

	buf_screen_cls();

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
	draw(actors);
	if(wait(500) == 0)
		tick(actors);
    }

    return 0;
}
