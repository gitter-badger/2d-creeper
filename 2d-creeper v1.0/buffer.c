#include "header.h"
void buf_screen_cls()
{
	CHAR_INFO b[24][80];
	buf_init(b,0,15);
	buf_display(b);
}
void buf_modify_bg_color(CHAR_INFO* b,int x,int y,int color)
{
	int index = y*80+x;
	if(index >= 80*24)
		return;
	//assure that color is 0~15
	color %= 16;
	b[index].Attributes = (WORD)(color <<4);
}
void buf_put_ch(CHAR_INFO* buf,char ch, int x, int y)
{
	int index = y*80+x;
	if(index >= 80*24)
		return;
	buf[index].Char.AsciiChar = ch;
}
void buf_put(CHAR_INFO* buf,char* str,int x, int y)
{
	int len = strlen(str);
	for(int i = 0;i<len;i++)
	{
		buf_put_ch(buf,str[i],x+i,y);
	}
	return;
}
void buf_init(CHAR_INFO* buf,char bg, char fg)
{
	char attr = (bg << 4) | fg;
	for(int cy = 0;cy<24;cy++)
	{
		for(int cx =0; cx<80;cx++)
		{
			int index = cy*80+cx;
			buf[index].Char.AsciiChar = ' ';
			buf[index].Attributes = attr;
		}
	}
}
void buffer_draw_hp(CHAR_INFO* buf,Actor* a, int x, int y)
{
	int max = a->max_hp;
	int hp = a->hp;
	int percent = ((double)hp/(double)max)* 100.0;
	int gauge = percent / 10;//because hp will be represented with 10 cell gauge bar
	int i;
	buf_put(buf,"HP:",x,y);
	for(i =0;i<gauge;i++)
	{
		buf_modify_bg_color(buf,x+i,y+1,4+8);//4+8 bright red 
	}
	for(;i<10;i++)	
	{
		buf_modify_bg_color(buf,x+i,y+1,4);//4 dark red color (maybe)
	}
}
void draw(Actor* actors)
{
    CHAR_INFO buffer[24][80];
    buf_init(buffer,15-8,0);
	
	Actor* hero = &(actors[0]);
	for(int cy = 0;cy<10;cy++)
	{
		for(int cx = 0;cx<20;cx++)
		{
			int sight_x = hero->x +cx -10;
			int sight_y = hero->y +cy -5;
			Actor* a = get_actor_at(actors,sight_x,sight_y);
			if(a == 0)
			{
				buffer_modify_bg_color(buffer, sight_x, sight_y, 15);
				buffer[cy][cx].Char.AsciiChar = ' ';
				buffer_modify_bg_color(buffer, sight_x, sight_y, 0);
				buffer[cy][cx].Attributes = 0;
			}
			else
			{				
				buffer[cy][cx].Char.AsciiChar = a->shape;
				buffer[cy][cx].Attributes = a->color;
			}
		}
	}
	buffer_draw_hp(buffer,hero,0,13);//display hero's hp at x ==0 y == 13
	
	buf_display((CHAR_INFO*)buffer);
}
