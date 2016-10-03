#include "header.h"
#ifndef LINUX
void gotoxy(int x,int y)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p = {x,y};
    SetConsoleCursorPosition(h,p);
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
int be_input()
{
	
	HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD input_record;
	DWORD input_count;

	PeekConsoleInput(CIN, &input_record, 1, &input_count);
	return input_count;
}

int get_input_2(WORD *vkey, COORD *pos)
{
	
	HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD input_record;
	DWORD input_count;
	ReadConsoleInput(CIN, &input_record, 1, &input_count);
	switch (input_record.EventType)
	{
	case MOUSE_EVENT:
		if (pos && (input_record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			
			HANDLE COUT = GetStdHandle(STD_OUTPUT_HANDLE);
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
void set_color(char bg, char fg)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD attr = (bg <<4) | fg;
	SetConsoleTextAttribute(h,attr);
}
void select_menu()
{
	CHAR_INFO buf[24][80];
	buf_init((CHAR_INFO*)buf,0,15);
	
	set_color(0,14);
	gotoxy(0,0);
	print_slow("<    2d-creeper v1.0    >",100);
	Sleep(500);
	buf_put(buf,"[ 1. Play Game          ]",0,2);
	buf_put(buf,"[ 2. Shutdown Game      ]",0,3);
	buf_display((CHAR_INFO*)buf);
	while (1)
	{
		
		COORD pos = { 0, 0 };

		DWORD mode;
		WORD key;
		int event;
		HANDLE CIN = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE COUT = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(CIN, &mode);
		SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
		if (be_input())
		{
			event = get_input_2(&key, &pos);
		}
		if ((pos.X > 0 && pos.X < 24) && pos.Y == 2)
		{
			for(int x = 0;x<24;x++)
			{
				buf_modify_bg_color((CHAR_INFO*)buf,x,pos.Y,15);
				buf_display((CHAR_INFO*)buf);
				Sleep(20);
			}
			
			return;
		}
		else if ((pos.X > 0 && pos.X < 24) && pos.Y == 3)
		{
			exit(0);
		}
	}
}
void buf_display(CHAR_INFO *buffer)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD sz = {80,24};
    COORD xy = {0,0};
    SMALL_RECT r = {0,0,80,24};
    WriteConsoleOutput(h,buffer,sz,xy,&r);
}
#endif
