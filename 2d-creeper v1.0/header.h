#ifndef HEADER_H
#define HEADER_H

#ifndef LINUX
#include <Windows.h>
#include <conio.h>
#else
#include "linux.h"
#define buf_display(x) buf_display_linux(x)
#define select_menu(x) select_menu_linux(x)
#define Sleep(x) Sleep_linux(x)
#define set_cursor(x) set_cursor_linux(x)
#define kbhit() kbhit_linux()
#define getch() getch_linux()
#endif



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

#endif
#ifndef CPROTO
#include "proto.h"
#endif