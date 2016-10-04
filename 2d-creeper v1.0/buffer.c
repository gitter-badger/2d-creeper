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
	WORD attr = b[index].Attributes;
	attr = attr & 0x000f;
	attr = attr | (WORD)(color <<4);
	b[index].Attributes = attr;
}
void buf_modify_fg_color(CHAR_INFO* b,int x,int y,int color)
{
	int index = y*80+x;
	if(index >= 80*24)
		return;
	//assure that color is 0~15
	color %= 16;
	WORD attr = b[index].Attributes;
	attr = attr & 0xfff0;
	attr = attr | (WORD)(color);
	b[index].Attributes = attr;
}
void buf_put_ch(CHAR_INFO* buf,char ch, int x, int y)
{
	int index = y*80+x;
	if(index >= 80*24)
		return;
	buf[index].Char.AsciiChar = ch;
}
void buf_put_ch_with_color(CHAR_INFO* buf, int x, int y, char ch,char bg_color,char fg_color)
{
	buf_put_ch(buf,ch,x,y);
	buf_modify_bg_color(buf,x,y,bg_color);
	buf_modify_fg_color(buf,x,y,fg_color);
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
void buf_put_with_color(CHAR_INFO* buf,int x, int y,char* str,char bg, char fg)
{
	int len = strlen(str);
	for(int i = 0;i<len;i++)
	{
		buf_put_ch_with_color(buf,x+i,y,str[i],bg,fg);
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
void buffer_draw_coord(CHAR_INFO* buf, Actor* a, int x, int y)
{
	char str[32];
	sprintf(str,"x:%d, y:%d",a->x,a->y);
	buf_put_with_color(buf,x,y,str,0,15);
}
void buffer_draw_hp(CHAR_INFO* buf,Actor* a, int x, int y)
{
	int max = a->max_hp;
	int hp = a->hp;
	int percent = ((double)hp/(double)max)* 100.0;
	int gauge = percent / 10;//because hp will be represented with 10 cell gauge bar
	int i;
	char hp_str[32];
	sprintf(hp_str,"HP:%d/%d",hp,max);
	buf_put_with_color(buf,x,y,hp_str,0,4+8);
	
	for(i =0;i<gauge;i++)
	{
		buf_modify_bg_color(buf,x+i,y+1,4+8);//4+8 bright red 
	}
#ifdef LINUX
	return;
#endif
	for(;i<10;i++)	
	{
		buf_modify_bg_color(buf,x+i,y+1,4);//4 dark red color (maybe)
	}
}
void buffer_draw_frame(CHAR_INFO* buf,int x, int y, int width, int height, char color)
{
	for(int cy = 0;cy<height;cy++)
	{
		for(int cx = 0; cx<width;cx++)
		{
			buf_modify_bg_color(buf,cx+x,cy+y,color);
		}
	}
}
void draw(Actor* actors)
{
    CHAR_INFO buffer[24][80];
    buf_init(buffer,0,0);
	
	Actor* hero = &(actors[0]);
	
	//parameters: buffer,x,y,width,height,color
	buffer_draw_frame(buffer,0,0,22,12,15);
	
	for(int cy = 0;cy<10;cy++)
	{
		for(int cx = 0;cx<20;cx++)
		{
			int sight_x = hero->x +cx -10;
			int sight_y = hero->y +cy -5;
			Actor* a = get_actor_at(actors,sight_x,sight_y);
			//방금a가 0일때 a->exist를 참조할수없어서 에러남.
			if(a == 0)
			{
				buf_put_ch_with_color(buffer,cx+1,cy+1,' ',0,0);
			}
			else if(a->exist == 0)
			{
				buf_put_ch_with_color(buffer,cx+1,cy+1,' ',0,0);
			}
			else
			{
				char ch = a->shape;
				char fg = a->color;
				//+1 for frame
				buf_put_ch_with_color(buffer,cx+1,cy+1,ch,0,fg);
			}
		}
	}
	buffer_draw_coord(buffer,hero,0,12);
	buffer_draw_hp(buffer,hero,0,13);//display hero's hp at x ==0 y == 13
	
	buf_display((CHAR_INFO*)buffer);
}
