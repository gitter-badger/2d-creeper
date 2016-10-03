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
int check_game_over(Actor* a)
{
	if(a->hp <= 0)
		return 1;
	else
		return 0;
	
}

void print_slow(char* str,int delay)
{
	int i =0;
	while(str[i] != 0)
	{
		putchar(str[i]);
		Sleep(delay);
		i++;
	}
}

int main()
{
	while(1)
	{
		game();
		buf_screen_cls();
		gotoxy(0,0);
		set_color(0,15);
		printf("게임을 다시 시작할까요?Y/N");
		char answer;
		fflush(stdin);
		scanf("%c",&answer);
		if(answer == 'y' || answer == 'Y')
			continue;
		else
			break;
	}
	
	gotoxy(0,0);
	set_color(0,15);
	print_slow("플레이 해 주셔서 감사합니다.",100);
	Sleep(1000);
}
void game()
{
	int creeper;
	srand(time(0));
	memset(actors,0,sizeof(Actor)*100);
	
	
	select_menu();
	buf_screen_cls();

	set_cursor(0);
	
    actors[0] = new_hero(10,10);
	
	
	gotoxy(0,0);
	set_color(0,12);
	print_slow("크리퍼",100);
	set_color(0,15);
    printf("를 몇 마리 만드시겠습니까?(0~99)");
    scanf("%d", &creeper);
    for (int i = 0; i < creeper; i++)
    {
		//크리퍼를 만들때 초기 tick_fn을 creeper_branch로 해줘야함
        actors[i+1] = new_creeper(rand() % 80, rand()%24);
    }

    Actor * hero = &(actors[0]);
    while(1)
    {
		
			Sleep(50);
			draw(actors);
			tick(actors);
			if(kbhit())
			{
				keyboard(hero);
				while(kbhit()) getch();
			}
			if(check_game_over(hero))
				break;


				

    }
    return;
}
