#ifdef LINUX
#include "header.h"
#include <termios.h>
#include <sys/ioctl.h>
void gotoxy(int x, int y)
{
	printf("%c[%d;%dH",0x1b,x,y);
}
int kbhit_linux()
{
	
	static int init = 0;
	if(init == 0)
	{
		struct termios term;

		tcgetattr(0,&term);//0 for STDIN
		term.c_lflag &= ~ICANON;
		term.c_lflag &= ~ECHO;
		tcsetattr(0,TCSANOW,&term);//0 for STDIN
		setbuf(stdin,0);
		init = 1;	
	}
	int bytes;
	ioctl(0,FIONREAD,&bytes);//0 for STDIN
	return bytes;
}

int getch_linux()
{
	getchar();
}

void set_cursor_linux(int v)
{
	//do 
}
void set_color_linux(char bg, char fg)
{
	char bg_str[10];
	char fg_str[10];
	if(fg >=8)
		sprintf(fg_str,"3%d;1",fg%8);
	else
		sprintf(fg_str,"3%d",fg);

	printf("%c[%sm",0x1b,fg_str);	
	if(bg >=8)
		sprintf(bg_str,"4%d;1",bg%8);
	else
		sprintf(bg_str,"4%d",bg);
	printf("%c[%sm",0x1b,bg_str);	

}
void buf_display_linux(CHAR_INFO *buffer)
{
	for(int cy =0;cy<24;cy++)
	{
		printf("%c[%d;%dH",0x1b,cy,0);
		for(int cx = 0;cx<80;cx++)
		{
			int index = cy*80+cx;
			WORD w = buffer[index].Attributes;
			char bg = (w & 0xf0) >> 4;
			char fg = w & 0x0f;
			set_color(bg,fg);
			char ch = buffer[index].Char.AsciiChar;
			putchar(ch);
		}
	}
	fflush(stdout);
	set_color(0,15);
	printf("%c[%d;%dH",0x1b,0,0);

}
void Sleep_linux(int v)
{
	usleep(v*1000);
}
void select_menu_linux()
{
	CHAR_INFO buf[24][80];
	buf_init((CHAR_INFO*)buf,0,15);
	buf_put(buf,"[    2d-creeper v1.0    ]",0,0);
	buf_put(buf,"[                       ]",0,1);
	buf_put(buf,"[ 1. Play Game          ]",0,2);
	buf_put(buf,"[ 2. Shutdown Game      ]",0,3);
	buf_put(buf,"[ Please select number  ]",0,4);
	buf_display_linux((CHAR_INFO*)buf);
	while(1)
	{
		int n;
		scanf("%d",&n);
		if(n == 1)
		{
			for(int x = 0;x<24;x++)
			{
				buf_modify_bg_color((CHAR_INFO*)buf,x,2,15);
				buf_display_linux((CHAR_INFO*)buf);
				Sleep(20);
			}
			return;
		}
		else
			exit(0);
	}
	
}








#endif
